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

#include <cassert>
#include <cstring>
#include <vector>

#include "gut/details/directory.h"
#include "gut/details/retry_eintr.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/utils/tmp_dir.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

bool remove_dir(int parent_fd, const std::string_view& dir_name, const std::function<bool()>& is_stop_fn)
{
    assert(__GUT_IS_OPEN_FD(parent_fd));

    if (is_stop_fn()) {
        return false;
    }

    int dir_fd = RETRY_EINTR(::openat(parent_fd, dir_name.data(), O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC));
    if (dir_fd < 0) {
        return (errno == ENOENT);
    }
    if (! __GUT_LOCK_FD(dir_fd, true)) {
        ::close(dir_fd);
        return false;
    }

    bool ok = true;
    dir_reader reader;
    if (reader.opendir(dir_fd)) {
        while (struct ::dirent* p_ent = reader.readdir()) {
            const std::string_view sub_dir_name = p_ent->d_name;
            if (sub_dir_name == "." || sub_dir_name == "..") {
                continue;
            }

            if (p_ent->d_type == DT_DIR) {
                ok &= remove_dir(dir_fd, p_ent->d_name, is_stop_fn);
            } else {
                if (::unlinkat(dir_fd, p_ent->d_name, 0) != 0 && errno != ENOENT) {
                    ok = false;
                }
            }
        }
    } else {
        ok = false;
    }

    __GUT_UNLOCK_FD(dir_fd);
    ::close(dir_fd);

    if (ok && ::unlinkat(parent_fd, dir_name.data(), AT_REMOVEDIR) != 0 && errno != ENOENT) {
        ok = false;
    }
    return ok;
}

bool remove_file(int parent_fd, const std::string_view& file_name, int file_gap_seconds)
{
    assert(__GUT_IS_OPEN_FD(parent_fd));

    int file_fd = RETRY_EINTR(::openat(parent_fd, file_name.data(), O_RDONLY | O_NONBLOCK | O_CLOEXEC));
    if (file_fd < 0) {
        if (errno == ENOENT) {
            return true;
        }
        if (errno == EMFILE || errno == ENFILE || errno == ENOMEM) {
            return false;
        }
        return (::unlinkat(parent_fd, file_name.data(), 0) == 0 || errno == ENOENT);
    }

    if (! __GUT_LOCK_FD(file_fd, true)) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            ::close(file_fd);
            return false;
        }
        ::close(file_fd);
        return (::unlinkat(parent_fd, file_name.data(), 0) == 0 || errno == ENOENT);
    }

    struct ::stat64 info;
    if (RETRY_EINTR(::fstat64(file_fd, &info)) != 0) {
        __GUT_UNLOCK_FD(file_fd);
        ::close(file_fd);
        return false;
    }

    if (info.st_nlink < 1 || (::time(nullptr) - info.st_mtime < file_gap_seconds)) {
        __GUT_UNLOCK_FD(file_fd);
        ::close(file_fd);
        return (info.st_nlink < 1);
    }

    __GUT_UNLOCK_FD(file_fd);
    ::close(file_fd);
    return (::unlinkat(parent_fd, file_name.data(), 0) == 0 || errno == ENOENT);
}

bool cleanup_active_dir(const tmp_dir::ptr& p_parent, bool remove_files, int file_gap_seconds, const std::function<bool()>& is_stop_fn)
{
    assert(p_parent->is_open());

    if (is_stop_fn()) {
        return false;
    }

    dir_reader reader;
    if (! reader.opendir(p_parent->fd())) {
        return (errno == ENOENT);
    }

    bool can_cleanup = true;
    while (struct ::dirent* p_ent = reader.readdir()) {
        const std::string_view entry_name = p_ent->d_name;
        if (entry_name == "." || entry_name == "..") {
            continue;
        }

        if (p_ent->d_type == DT_DIR) {
            can_cleanup &= remove_dir(p_parent->fd(), entry_name, is_stop_fn);
        } else if (p_ent->d_type == DT_REG) {
            if (entry_name.size() != gk_filename_length) {
                can_cleanup &= remove_file(p_parent->fd(), entry_name, 0);
            } else if (remove_files) {
                can_cleanup &= remove_file(p_parent->fd(), entry_name, file_gap_seconds);
            } else {
                can_cleanup = false;
            }
        } else {
            can_cleanup &= ::unlinkat(p_parent->fd(), entry_name.data(), 0) == 0 || errno == ENOENT;
        }
    }
    return can_cleanup;
}

bool cleanup_primary_dir(cleaner& clnr, const tmp_dir::ptr& p_parent, bool remove_files,
                         int file_gap_seconds, const std::function<bool()>& is_stop_fn)
{
    assert(p_parent->is_open());

    if (is_stop_fn()) {
        return false;
    }

    dir_reader reader;
    if (! reader.opendir(p_parent->fd())) {
        return (errno == ENOENT);
    }

    bool can_cleanup = true;
    while (struct ::dirent* p_ent = reader.readdir()) {
        const std::string_view entry_name = p_ent->d_name;
        if (entry_name == "." || entry_name == "..") {
            continue;
        }

        if (p_ent->d_type == DT_DIR) {
            if (entry_name.size() != gk_name_length) {
                can_cleanup &= remove_dir(p_parent->fd(), entry_name, is_stop_fn);
            } else {
                tmp_dir::ptr p_dir = std::make_shared<tmp_dir>(entry_name, p_parent);
                if (p_dir->open() && p_dir->lock(true)) {
                    if (cleanup_active_dir(p_dir, remove_files, file_gap_seconds, is_stop_fn)) {
                        clnr.push(p_dir);
                    } else {
                        can_cleanup = false;
                    }
                } else {
                    can_cleanup = false;
                }
            }
        } else {
            can_cleanup &= ::unlinkat(p_parent->fd(), entry_name.data(), 0) == 0 || errno == ENOENT;
        }
    }
    return can_cleanup;
}

} // <anonymous> namespace

cleaner::cleaner(std::function<bool()> is_stop_fn)
    : m_is_stop_fn(std::move(is_stop_fn))
{
    static_assert(std::is_same<tmp_dir_ptr_t, tmp_dir::ptr>::value);
}

/**
 *  \details    Iterates up to the initial queue size, invoking step() on each entry
 *      until the queue is empty, a full pass completes, or is_stop_fn signals shutdown.
 */
bool cleaner::cleanup(int dir_gap_seconds, int file_gap_seconds)
{
    size_t dirs_count = 0;
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        dirs_count = m_dir_queue.size();
    }

    for (; dirs_count > 0; --dirs_count) {
        if (m_is_stop_fn() || step(dir_gap_seconds, file_gap_seconds)) {
            break;
        }
    }

    std::lock_guard<std::mutex> locker(m_mutex);
    return m_dir_queue.empty();
}

/**
 *  \details Scans nook_path and removes malformed entries (names not matching internal
 *      length rules), stray root files, and abandoned empty rotation trees.
 *
 *      - When remove_files is false, valid message files are preserved.
 *      - When remove_files is true, unlocked files older than file_gap_seconds are purged.
 */
bool cleaner::cleanup(const std::string_view& nook_path, int dir_gap_seconds, int file_gap_seconds,
                      bool remove_files, const std::function<bool()>& is_stop_fn)
{
    directory root_dir(nook_path);
    if (! root_dir.open()) {
        return (errno == ENOENT);
    }

    dir_reader reader;
    if (! reader.opendir(root_dir.fd())) {
        return false;
    }

    bool is_cleaned = true;

    cleaner clnr(is_stop_fn);
    while (struct ::dirent* p_ent = reader.readdir()) {
        const std::string_view entry_name = p_ent->d_name;
        if (entry_name == "." || entry_name == "..") {
            continue;
        }

        if (p_ent->d_type == DT_DIR) {
            if (entry_name.size() != gk_name_length) {
                is_cleaned &= remove_dir(root_dir.fd(), entry_name, is_stop_fn);
            } else {
                tmp_dir::ptr p_dir = std::make_shared<tmp_dir>(entry_name, root_dir.fd());
                if (p_dir->open() && p_dir->lock(true)) {
                    if (cleanup_primary_dir(clnr, p_dir, remove_files, file_gap_seconds, is_stop_fn)) {
                        clnr.push(p_dir);
                    } else if (remove_files) {
                        is_cleaned = false;
                    }
                } else {
                    is_cleaned = false;
                }
            }
        } else if (p_ent->d_type == DT_REG) {
            is_cleaned &= fq::remove_file(root_dir.fd(), entry_name, 0);
        } else {
            is_cleaned &= ::unlinkat(root_dir.fd(), entry_name.data(), 0) == 0 || errno == ENOENT;
        }
    }
    if (! clnr.m_dir_queue.empty()) {
        is_cleaned &= clnr.cleanup(dir_gap_seconds, file_gap_seconds);
    }
    return is_cleaned;
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

/**
 *  \details    Pops a single directory from the front of m_dir_queue, verifies its age
 *      against dir_gap_seconds, purges unlocked stale files inside it, and removes the
 *      directory itself if empty and unreferenced (use_count() == 1). Otherwise,
 *      re-queues it.
 */
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
