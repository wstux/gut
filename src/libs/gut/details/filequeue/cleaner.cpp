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
#include <sys/types.h>

#include <cstring>

#include "gut/details/retry_eintr.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/utils/tmp_dir.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

cleaner::cleaner(std::function<bool()> is_stop_fn)
    : m_is_stop_fn(std::move(is_stop_fn))
{
    static_assert(std::is_same<tmp_dir_ptr_t, tmp_dir::ptr>::value);
}

bool cleaner::perform(const tmp_dir::ptr& p_dir, int file_gap_seconds)
{
    dir_reader reader;
    if (! reader.opendir(p_dir->fd())) {
        return true;
    }

    bool done = true;

    while (true) {
        if (m_is_stop_fn()) {
            return false;
        }

        struct ::dirent* p_ent = reader.readdir();
        if (! p_ent) {
            break;
        }

        if (p_ent->d_type == DT_DIR) {
            if (::strcmp(p_ent->d_name, ".") == 0 || ::strcmp(p_ent->d_name, "..") == 0) {
                continue;
            }

            tmp_dir::ptr p_child_dir = std::make_shared<tmp_dir>(p_ent->d_name, p_dir);
            if (p_child_dir->open()) {
                push(p_child_dir);
            }
            done = false;
        } else if (p_ent->d_type == DT_REG) {
            int file_fd = RETRY_EINTR(::openat(p_dir->fd(), p_ent->d_name, O_RDONLY | O_NONBLOCK | O_CLOEXEC));
            if (file_fd < 0) {
                if (errno == EMFILE || errno == ENFILE || errno == ENOMEM) {
                    return false;
                }
                if (errno != ENOENT) {
                    done &= remove_file(reader, p_ent->d_name);
                }
                continue;
            }

            if (! __GUT_LOCK_FD(file_fd, true)) {
                const int saved_errno = errno;
                ::close(file_fd);

                if (saved_errno == EWOULDBLOCK || saved_errno == EAGAIN) {
                    done = false;
                } else {
                    done &= remove_file(reader, p_ent->d_name);
                }
                continue;
            }

            struct ::stat64 info;
            if (RETRY_EINTR(::fstat64(file_fd, &info)) != 0) {
                __GUT_UNLOCK_FD(file_fd);
                ::close(file_fd);
                done &= remove_file(reader, p_ent->d_name);
                continue;
            }

            if (info.st_nlink < 1 || (::time(nullptr) - info.st_mtime < file_gap_seconds)) {
                if (info.st_nlink >= 1) {
                    done = false;
                }
                __GUT_UNLOCK_FD(file_fd);
                ::close(file_fd);
                continue;
            }

            __GUT_UNLOCK_FD(file_fd);
            ::close(file_fd);

            done &= remove_file(reader, p_ent->d_name);
        } else {
            done &= remove_file(reader, p_ent->d_name);
        }
    }

    if (done && (p_dir.use_count() == 1) && p_dir->lock(true)) {
        if (! p_dir->remove()) {
            p_dir->lock(false);
            done = false;
        }
    }

    return done;
}

bool cleaner::push(const tmp_dir::ptr& p_dir)
{
    if (! p_dir) {
        return false;
    }

    std::lock_guard<std::mutex> locker(m_mutex);
    return m_dir_queue.insert(p_dir).second;
}

bool cleaner::remove_file(dir_reader& reader, const char* name)
{
    if (::unlinkat(reader.dirfd(), name, 0) == 0) {
        return true;
    }
    return errno == ENOENT;
}

bool cleaner::step(int dir_gap_seconds, int file_gap_seconds)
{
    tmp_dir::ptr p_dir;

    {
        std::lock_guard<std::mutex> locker(m_mutex);
        if (m_dir_queue.empty()) {
            return true;
        }
        std::set<tmp_dir::ptr, dir_less>::iterator it = m_dir_queue.begin();
        p_dir = std::move(*it);
        m_dir_queue.erase(it);
    }

    if (m_is_stop_fn()) {
        push(p_dir);
        return false;
    }

    if (! p_dir->is_open() && ! p_dir->open()) {
        if (errno != ENOENT) {
            push(p_dir);
        }
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_dir_queue.empty();
    }

    if ((dir_gap_seconds > 0) && (::time(nullptr) - p_dir->mtime() < dir_gap_seconds)) {
        push(p_dir);
        return false;
    }

    if (! perform(p_dir, file_gap_seconds)) {
        push(p_dir);
    }

    std::lock_guard<std::mutex> locker(m_mutex);
    return m_dir_queue.empty();
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
