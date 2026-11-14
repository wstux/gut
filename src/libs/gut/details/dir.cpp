/*
 * The MIT License
 *
 * Copyright 2026 Chistyakov Alexander.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <cassert>
#include <cstring>
#include <filesystem>
#include <utility>

#include "gut/details/dir.h"
#include "gut/details/generator.h"
#include "gut/details/retry_eintr.h"
#include "gut/details/utils.h"

namespace wstux {
namespace gut {
namespace details {
namespace {

int open_at(int dir_fd, const char* name, int flags)
{
    int fd = RETRY_EINTR(::openat(dir_fd, name, flags, 0));
    if (! IS_OPEN_FD(fd)) {
        switch (errno) {
        case EINVAL:
            if (flags & O_CLOEXEC) {
                flags &= ~O_CLOEXEC;
                fd = open_at(dir_fd, name, flags);
                if (IS_OPEN_FD(fd)) {
                    flags = RETRY_EINTR(::fcntl(fd, F_GETFD));
                    if ((flags == -1) || RETRY_EINTR(::fcntl(fd, F_SETFD, FD_CLOEXEC | flags))) {
                        close(fd);
                    }
                }
                return fd;
            } else {
                return fd;
            }
            break;
        case EPERM:
            if (flags & O_NOATIME) {
                flags &= ~O_NOATIME;
                return open_at(dir_fd, name, flags);
            } else {
                return fd;
            }
            break;
        default:
            return fd;
        }
    }
    return fd;
}

bool open_dir(int& fd, const int parent_fd, const char* p_path)
{
    if(IS_OPEN_FD(fd)) {
        return true;
    }
    while (! IS_OPEN_FD(fd)) {
        constexpr int openat_flags = O_RDONLY | O_CLOEXEC | O_DIRECTORY | O_NOATIME | O_NOCTTY;
        fd = open_at(parent_fd, p_path, openat_flags);
        if (IS_OPEN_FD(fd)) {
            break;
        }

        assert(errno != EINTR);
        if (errno != ENOENT) {
            return false;
        }
        constexpr int mkdirat_flags = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
        const int rc = RETRY_EINTR(::mkdirat(parent_fd, p_path, mkdirat_flags));
        if ((rc != 0) && (errno != EEXIST)) {
            return false;
        }
    }
    assert(IS_OPEN_FD(fd));
    return true;
}

} // <anonymous> namespace

////////////////////////////////////////////////////////////////////////////////
// class tmp_dir

tmp_dir::tmp_dir(int parent_fd)
    : m_parent_fd(parent_fd)
{
    assert(IS_OPEN_FD(m_parent_fd));
    m_name[0] = '\0';
}

tmp_dir::tmp_dir(const ptr& p_parent)
    : m_p_parent(p_parent)
    , m_parent_fd(m_p_parent->fd())
{
    assert(IS_OPEN_FD(m_parent_fd));
    m_name[0] = '\0';
}

tmp_dir::tmp_dir(const char* p_name, int parent_fd)
    : m_parent_fd(parent_fd)
{
    assert(::strlen(p_name) == gk_name_length);
    ::memcpy(m_name, p_name, gk_name_length);
    m_name[gk_name_length] = '\0';
}

tmp_dir::tmp_dir(const std::string_view& name, int parent_fd)
    : m_parent_fd(parent_fd)
{
    assert(name.size() == gk_name_length);
    ::memcpy(m_name, name.data(), gk_name_length);
    m_name[gk_name_length] = '\0';
}

tmp_dir::tmp_dir(const std::string_view& name, const ptr& p_parent)
    : m_p_parent(p_parent)
    , m_parent_fd(m_p_parent->fd())
{
    assert(IS_OPEN_FD(m_parent_fd));
    assert(name.size() == gk_name_length);
    ::memcpy(m_name, name.data(), gk_name_length);
    m_name[gk_name_length] = '\0';
}

tmp_dir::~tmp_dir()
{
    if (utils::lock(m_fd, true)) {
        if (! remove() ) {
            utils::unlock(m_fd);
        }
    }
    utils::close(m_fd);
}

void tmp_dir::close()
{
    utils::close(m_fd);
}

bool tmp_dir::create()
{
    assert(! IS_OPEN_FD(m_fd));

    while (true) {
        generate_name(m_name, sizeof(m_name));

        constexpr int mkdirat_flags = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
        const int rc = RETRY_EINTR(::mkdirat(m_parent_fd, m_name, mkdirat_flags));
        if (rc != 0) {
            if (errno != EEXIST) {
                return false;
            }
        }

        constexpr int openat_flags = O_RDONLY | O_CLOEXEC | O_DIRECTORY | O_NOATIME | O_NOCTTY;
        m_fd = open_at(m_parent_fd, m_name, openat_flags);
        if (IS_OPEN_FD(m_fd)) {
            return true;
        }
        if (errno != ENOENT) {
            return false;
        }
    }

    return false;
}

bool tmp_dir::is_open() const
{
    return IS_OPEN_FD(m_fd);
}

bool tmp_dir::lock(bool is_exclusive) const
{
    return utils::lock(m_fd, is_exclusive);
}

::time_t tmp_dir::mtime() const
{
    assert(IS_OPEN_FD(m_fd));
    struct ::stat64 info;
    const int rc = RETRY_EINTR(::fstat64(m_fd, &info));
    return (rc == 0) ? info.st_mtime : -1;
}

bool tmp_dir::open()
{
    assert(m_name[0] != '\0');
    assert(IS_OPEN_FD(m_parent_fd));
    return open_dir(m_fd, m_parent_fd, m_name);
}

bool tmp_dir::remove()
{
    if (m_name[0] == '\0') {
        return true;
    }

    assert(IS_OPEN_FD(m_parent_fd));
    const int rc = RETRY_EINTR(::unlinkat(m_parent_fd, m_name, AT_REMOVEDIR));
    if ((rc != 0) && (errno != ENOENT)) {
        if ((errno == ENOTEMPTY) || (errno == EBUSY)) {
            // \todo Need to do something
        }
        return false;
    }

    m_name[0] = '\0';
    return true;
}

size_t tmp_dir::size() const
{
    struct ::stat64 info;
    const int rc = RETRY_EINTR(::fstat64(m_fd, &info));
    if (rc == 0) {
        return info.st_size;
    }
    return static_cast<size_t>(-1);
}

void tmp_dir::unlock() const
{
    utils::unlock(m_fd);
}

////////////////////////////////////////////////////////////////////////////////
// class dir_t

dir_t::dir_t(std::string dir_path)
    : m_path(std::filesystem::absolute(dir_path).string())
    , m_fd(-1)
{}

dir_t::dir_t(std::string_view dir_path)
    : m_path(std::filesystem::absolute(dir_path).string())
    , m_fd(-1)
{}

dir_t::~dir_t()
{
    utils::close(m_fd);
}

bool dir_t::is_open() const
{
    return IS_OPEN_FD(m_fd);
}

bool dir_t::lock(bool is_exclusive) const
{
    return utils::lock(m_fd, is_exclusive);
}

bool dir_t::open()
{
    return open_dir(m_fd, AT_FDCWD, m_path.c_str());
}

void dir_t::unlock() const
{
    utils::unlock(m_fd);
}

////////////////////////////////////////////////////////////////////////////////
// class dir_reader

dir_reader::dir_reader(dir_reader&& other) noexcept
    : m_p_scanner(std::exchange(other.m_p_scanner, nullptr))
{}

dir_reader& dir_reader::operator=(dir_reader&& other) noexcept
{
    if (this != &other) {
        close();
        m_p_scanner = std::exchange(other.m_p_scanner, nullptr);
    }
    return *this;
}

void dir_reader::close() noexcept
{
    if (m_p_scanner) {
        ::closedir(m_p_scanner);
        m_p_scanner = nullptr;
    }
}

bool dir_reader::opendir(const char* p_path) noexcept
{
    if (is_opened()) {
        return false;
    }

    m_p_scanner = ::opendir(p_path);
    if (! m_p_scanner) {
        return false;
    }

    ::fcntl(dirfd(), F_SETFD, FD_CLOEXEC);
    return true;
}

bool dir_reader::opendir(int fd) noexcept
{
    if (is_opened()) {
        return false;
    }

    int duplicated_fd = RETRY_EINTR(::fcntl(fd, F_DUPFD_CLOEXEC, 0));
    if (duplicated_fd < 0) {
        return false;
    }

    m_p_scanner = ::fdopendir(duplicated_fd);
    if (! m_p_scanner) {
        ::close(duplicated_fd);
        return false;
    }

    return true;
}

struct ::dirent* dir_reader::readdir() const noexcept
{
    if (! is_opened()) {
        return nullptr;
    }

    struct ::dirent* p_ent = ::readdir(m_p_scanner);
    if (! p_ent) {
        return nullptr;
    }

    if (p_ent->d_type == DT_UNKNOWN) {
        struct ::stat info;
        if (::fstatat(::dirfd(m_p_scanner), p_ent->d_name, &info, AT_SYMLINK_NOFOLLOW) == 0) {
            if (S_ISDIR(info.st_mode)) {
                p_ent->d_type = DT_DIR;
            }
            if (S_ISREG(info.st_mode)) {
                p_ent->d_type = DT_REG;
            }
        }
    }

    return p_ent;
}

void dir_reader::rewinddir() const noexcept
{
    if (is_opened()) {
        ::rewinddir(m_p_scanner);
    }
}

} // namespace details
} // namespace gut
} // namespace wstux
