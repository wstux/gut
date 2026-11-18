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
#include <sys/file.h>
#include <sys/stat.h>

#include <cassert>
#include <cstring>
#include <utility>

#include "gut/details/directory.h"
#include "gut/details/retry_eintr.h"
#include "gut/details/utils/generator.h"
#include "gut/details/utils/tmp_dir.h"

namespace wstux {
namespace gut {
namespace details {

////////////////////////////////////////////////////////////////////////////////
// class tmp_dir

tmp_dir::tmp_dir(int parent_fd)
    : m_parent_fd(parent_fd)
{
    assert(__GUT_IS_OPEN_FD(m_parent_fd));
    m_name[0] = '\0';
}

tmp_dir::tmp_dir(const ptr& p_parent)
    : m_p_parent(p_parent)
    , m_parent_fd(m_p_parent->fd())
{
    assert(__GUT_IS_OPEN_FD(m_parent_fd));
    m_name[0] = '\0';
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
    assert(__GUT_IS_OPEN_FD(m_parent_fd));
    assert(name.size() == gk_name_length);
    ::memcpy(m_name, name.data(), gk_name_length);
    m_name[gk_name_length] = '\0';
}

tmp_dir::~tmp_dir()
{
    if (__GUT_LOCK_FD(m_fd, true)) {
        if (! remove() ) {
            __GUT_UNLOCK_FD(m_fd);
        }
    }
    __GUT_CLOSE_FD(m_fd);
}

void tmp_dir::close() noexcept
{
    __GUT_CLOSE_FD(m_fd);
}

bool tmp_dir::create() noexcept
{
    assert(! __GUT_IS_OPEN_FD(m_fd));

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
        m_fd = directory::openat(m_parent_fd, m_name, openat_flags);
        if (__GUT_IS_OPEN_FD(m_fd)) {
            return true;
        }
        if (errno != ENOENT) {
            return false;
        }
    }

    return false;
}

bool tmp_dir::is_open() const noexcept
{
    return __GUT_IS_OPEN_FD(m_fd);
}

bool tmp_dir::lock(bool is_exclusive) const noexcept
{
    return __GUT_LOCK_FD(m_fd, is_exclusive);
}

::time_t tmp_dir::mtime() const noexcept
{
    assert(__GUT_IS_OPEN_FD(m_fd));
    struct ::stat64 info;
    const int rc = RETRY_EINTR(::fstat64(m_fd, &info));
    return (rc == 0) ? info.st_mtime : -1;
}

bool tmp_dir::open() noexcept
{
    assert(m_name[0] != '\0');
    assert(__GUT_IS_OPEN_FD(m_parent_fd));
    return directory::open_dir(m_fd, m_parent_fd, m_name);
}

bool tmp_dir::remove() noexcept
{
    if (m_name[0] == '\0') {
        return true;
    }

    assert(__GUT_IS_OPEN_FD(m_parent_fd));
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

size_t tmp_dir::size() const noexcept
{
    struct ::stat64 info;
    const int rc = RETRY_EINTR(::fstat64(m_fd, &info));
    if (rc == 0) {
        return info.st_size;
    }
    return static_cast<size_t>(-1);
}

////////////////////////////////////////////////////////////////////////////////
// class dir_reader

void dir_reader::close() noexcept
{
    if (m_p_scanner) {
        ::closedir(m_p_scanner);
        m_p_scanner = nullptr;
    }
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

} // namespace details
} // namespace gut
} // namespace wstux
