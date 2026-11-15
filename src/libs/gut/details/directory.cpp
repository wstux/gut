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

#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <cassert>
#include <filesystem>

#include "gut/details/directory.h"
#include "gut/details/retry_eintr.h"

namespace wstux {
namespace gut {
namespace details {

directory::directory(std::string_view dir_path)
    : m_path(std::filesystem::absolute(dir_path).string())
    , m_fd(-1)
{}

directory::~directory()
{
    __GUT_CLOSE_FD(m_fd);
}

bool directory::is_open() const
{
    return __GUT_IS_OPEN_FD(m_fd);
}

bool directory::lock(bool is_exclusive) const
{
    return __GUT_LOCK_FD(m_fd, is_exclusive);
}

bool directory::open()
{
    return open_dir(m_fd, AT_FDCWD, m_path.c_str());
}

int directory::openat(int dir_fd, const std::string_view& dir_name, int flags)
{
    int fd = RETRY_EINTR(::openat(dir_fd, dir_name.data(), flags, 0));
    if (! __GUT_IS_OPEN_FD(fd)) {
        switch (errno) {
        case EINVAL:
            if (flags & O_CLOEXEC) {
                flags &= ~O_CLOEXEC;
                fd = directory::openat(dir_fd, dir_name, flags);
                if (__GUT_IS_OPEN_FD(fd)) {
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
                return directory::openat(dir_fd, dir_name, flags);
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

bool directory::open_dir(int& fd, const int parent_fd, const std::string_view& dir_name)
{
    if(__GUT_IS_OPEN_FD(fd)) {
        return true;
    }
    while (! __GUT_IS_OPEN_FD(fd)) {
        constexpr int openat_flags = O_RDONLY | O_CLOEXEC | O_DIRECTORY | O_NOATIME | O_NOCTTY;
        fd = directory::openat(parent_fd, dir_name, openat_flags);
        if (__GUT_IS_OPEN_FD(fd)) {
            break;
        }

        assert(errno != EINTR);
        if (errno != ENOENT) {
            return false;
        }
        constexpr int mkdirat_flags = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; // 0755
        const int rc = RETRY_EINTR(::mkdirat(parent_fd, dir_name.data(), mkdirat_flags));
        if ((rc != 0) && (errno != EEXIST)) {
            return false;
        }
    }
    assert(__GUT_IS_OPEN_FD(fd));
    return true;
}

void directory::unlock() const
{
    __GUT_UNLOCK_FD(m_fd);
}

} // namespace details
} // namespace gut
} // namespace wstux
