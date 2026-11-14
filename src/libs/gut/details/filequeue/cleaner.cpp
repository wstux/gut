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
#include <sys/types.h>

#include <cstring>

#include "gut/details/retry_eintr.h"
#include "gut/details/utils.h"
#include "gut/details/filequeue/cleaner.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

cleaner::cleaner(std::function<bool()> is_stop_fn)
    : m_is_stop_fn(std::move(is_stop_fn))
{}

bool cleaner::perform(const tmp_dir::ptr& p_dir, int file_gap_seconds)
{
    dir_reader reader;
    if (! reader.opendir(p_dir->fd())) {
        return true;
    }

    bool done = true;
    m_busy_pool_size = 0;

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

            tmp_dir::ptr child_dir = std::make_shared<tmp_dir>(p_ent->d_name, p_dir->fd());
            push(child_dir);
        } else if (p_ent->d_type == DT_REG) {
            if (m_busy_pool_size >= k_max_files_per_dir) {
                return false;
            }

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

            if (! utils::lock(file_fd, true)) {
                int saved_errno = errno;
                ::close(file_fd);

                if (saved_errno == EWOULDBLOCK || saved_errno == EAGAIN) {
                    busy_record_t& record = m_busy_pool[m_busy_pool_size++];
                    assert(::strlen(p_ent->d_name) == record.size() - 1);
                    ::memcpy(record.data(), p_ent->d_name, record.size() - 1);
                    record.back() = '\0';
                    done = false;
                } else {
                    done &= remove_file(reader, p_ent->d_name);
                }
                continue;
            }

            struct ::stat64 info;
            if (RETRY_EINTR(::fstat64(file_fd, &info)) != 0) {
                utils::unlock(file_fd);
                ::close(file_fd);
                done &= remove_file(reader, p_ent->d_name);
                continue;
            }

            if (info.st_nlink < 1 || ::time(nullptr) - info.st_mtime < file_gap_seconds) {
                busy_record_t& record = m_busy_pool[m_busy_pool_size++];
                //::strncpy(record.data(), p_ent->d_name, record.size() - 1);
                std::memcpy(record.data(), p_ent->d_name, record.size() - 1);
                record.back() = '\0';

                if (info.st_nlink >= 1) {
                    done = false;
                }

                utils::unlock(file_fd);
                ::close(file_fd);
                continue;
            }

            utils::unlock(file_fd);
            ::close(file_fd);

            if (info.st_size < 1) {
                done &= remove_file(reader, p_ent->d_name);
                continue;
            }

            done &= remove_file(reader, p_ent->d_name);
        }
    }

    if (done && p_dir->lock(true)) {
        if (! p_dir->remove()) {
            p_dir->lock(false);
            done = false;
        }
    }
    p_dir->close();
    return done;
}

bool cleaner::pop(tmp_dir::ptr& p_dir)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (! m_dir_queue.empty()) {
        p_dir = m_dir_queue.front();
        m_dir_queue.pop_front();
        return true;
    }

    p_dir.reset();
    return false;
}

bool cleaner::push(const tmp_dir::ptr& p_dir)
{
    if (! p_dir) {
        return false;
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_dir_queue.full()) {
        constexpr size_t k_overflow_batch_size = 8;

        size_t items_to_evict = std::min(k_overflow_batch_size, m_dir_queue.size());
        std::array<tmp_dir::ptr, k_overflow_batch_size> eviction_buffer{};

        for (size_t i = 0; i < items_to_evict; ++i) {
            eviction_buffer[i] = std::move(m_dir_queue.front());
            m_dir_queue.pop_front();
        }

        lock.unlock();

        for (size_t i = 0; i < items_to_evict; ++i) {
            if (eviction_buffer[i]) {
                perform(eviction_buffer[i], 0);
                eviction_buffer[i].reset();
            }
        }

        lock.lock();
    }

    m_dir_queue.push_back(p_dir);
    return true;
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
    tmp_dir::ptr dir;
    std::vector<tmp_dir::ptr> deferred_dirs;

    while (pop(dir)) {
        if (! dir->open()) {
            if (errno == ENOENT) continue;

            if (dir->remove()) {
                continue;
            }
            return false;
        }

        if ((dir_gap_seconds > 0) && (::time(nullptr) - dir->mtime() < dir_gap_seconds)) {
            deferred_dirs.push_back(dir);
            continue;
        }

        if (! perform(dir, file_gap_seconds)) {
            deferred_dirs.push_back(dir);
        }
    }

    for (size_t i = 0; i < deferred_dirs.size(); ++i) {
        push(deferred_dirs[i]);
    }

    std::lock_guard<std::mutex> locker(m_mutex);
    return m_dir_queue.empty();
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
