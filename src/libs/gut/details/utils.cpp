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

#include <time.h>
#include <unistd.h>
#include <sys/file.h>

#include "gut/details/utils.h"

namespace wstux {
namespace gut {
namespace details {
namespace utils {

bool close(int& fd)
{
    if (is_open(fd)) {
        const int rc = ::close(fd);
        if (rc != 0) {
            return false;
        }
        fd = -1;
    }
    return true;
}

bool is_alive(int fd)
{
    stat_t info;
    const int rc = RETRY_EINTR(::fstat64(fd, &info));
    return (rc == 0) ? info.st_nlink > 0 : false;
}

bool lock(const int fd, bool is_exclusive)
{
    const int op = is_exclusive ? LOCK_EX : LOCK_SH;
    const int rc = RETRY_EINTR(::flock(fd, LOCK_NB | op));
    if (rc != 0) {
        return false;
    }
    return true;
}

::time_t mtime(int fd)
{
    stat_t info;
    const int rc = RETRY_EINTR(::fstat64(fd, &info));
    return (rc == 0) ? info.st_mtime : -1;
}

int open_at(int dir_fd, const char* name, int flags, mode_t mode)
{
    int fd = RETRY_EINTR(::openat(dir_fd, name, flags, mode));
    if (! is_open(fd)) {
        switch (errno) {
        case EINVAL:
            if (flags & O_CLOEXEC) {
                flags &= ~O_CLOEXEC;
                fd = open_at(dir_fd, name, flags, mode);
                if (is_open(fd)) {
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
                return open_at(dir_fd, name, flags, mode);
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

void unlock(const int fd)
{
    const int rc = RETRY_EINTR(::flock(fd, LOCK_NB | LOCK_UN));
    if (rc != 0) {
        // \todo Need to do something
        return;
    }
}

} // namespace utils
} // namespace details
} // namespace gut
} // namespace wstux
