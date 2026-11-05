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

#include <cassert>
#include <atomic>
#include <thread>

#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

bool is_acceptable_dir(const tmp_dir::ptr& p_dir)
{
    /**
     *  \brief  Upper limit of the system directory file size to trigger rotation.
     *  \details    In Linux (ext4), a directory is a special type of file that
     *      stores records of nested files ('ext4_dir_entry_2' structures). The
     *      size of an empty folder is a multiple of the OS memory page size (4 KB).
     *
     *      The 64 KB limit (16 blocks of 4 KB each) is chosen as a balance for
     *      NVMe/SSD storage:
     *      - allows keeping around 1500–2000 paired queue files in a single
     *          directory;
     *      - this volume of metadata fits into the CPU's L1/L2 cache, minimizing
     *          file lookup latencies;
     *      - limits directory inode bloat;
     *      - reduces the frequency of 'mkdir' system calls.
     */
    //constexpr size_t k_dir_size_limt = 64 * 1024; // 64 kB
    constexpr size_t k_dir_size_limt = 32 * 1024; // 32 kB

    if (p_dir.get()) {
        return (p_dir->size() < k_dir_size_limt);
    }
    return false;
}

size_t monotime()
{
    struct ::timespec tv;
    if (::clock_gettime(CLOCK_MONOTONIC, &tv) == 0) {
        return static_cast<size_t>(tv.tv_sec);
    }

    return static_cast<size_t>(::time(nullptr));
}

bool rotate_tmp_dir(const int patent_fd, tmp_dir::ptr& p_dir)
{
    assert(utils::is_open(patent_fd));
    p_dir = std::make_shared<tmp_dir>(patent_fd);
    if (! p_dir->create() || ! p_dir->lock(false)) {
        p_dir.reset();
        return false;
    }

    return true;
}

} // <anonymous> namespace

queue::queue(const std::string_view& root_dir, const cleaner::ptr& p_cleaner, const is_stop_fn_t& is_stop_fn)
    : m_is_stop_fn(is_stop_fn)
    , m_root_dir(root_dir)
    , m_p_cleaner(p_cleaner)
    , m_fetcher(m_root_dir, m_p_cleaner, m_is_stop_fn)
{}

bool queue::create_item(entry& e, const std::string_view& name)
{
    assert(! e.is_alive());
    tmp_dir::ptr p_active_dir = rotate(false);
    if (! p_active_dir) {
        return false;
    }

    return e.create(p_active_dir, name);
}

bool queue::open()
{
    if (! m_root_dir.is_open() && ! m_root_dir.open()) {
        return false;
    }

    return m_root_dir.lock(false);
}

bool queue::pop_item(entry& e, queue& target)
{
    assert(! e.is_alive());

    std::string filename;
    tmp_dir::ptr p_file_dir;

    while (! m_is_stop_fn()) {
        if (m_fetcher.fetch(filename, p_file_dir)) {
            if (! e.open(p_file_dir, filename)) {
                continue;
            }

            if (target.push_item(e)) {
                return true;
            } else {
                e.close();
                return false;
            }
        }
        const size_t cur_time = monotime();
        if (m_cleaner_ts < cur_time) {
            m_p_cleaner->step(5, 120);
            m_cleaner_ts = cur_time;
        }
        return false;
    }

    return false;
}

bool queue::push_item(entry& e)
{
    assert(e.is_alive());
    tmp_dir::ptr p_dir = rotate(false);
    if (! p_dir) {
        return false;
    }
    if (e.move(p_dir)) {
        return true;
    }
    if (errno == EEXIST) {
        p_dir = rotate(true);
        return e.move(p_dir);
    }
    return false;
}

tmp_dir::ptr queue::rotate(bool force_rotation)
{
    if (! force_rotation) {
        tmp_dir::ptr p_active_dir = std::atomic_load(&m_p_active_dir);
        if (p_active_dir && is_acceptable_dir(p_active_dir)) {
            return p_active_dir;
        }
    }

    std::unique_lock<std::mutex> lock(m_dir_mutex);

    if (m_is_rotating_process) {
        m_dir_cv.wait(lock, [this]() { return ! m_is_rotating_process; });
        return std::atomic_load(&m_p_active_dir);
    }

    if (! force_rotation && is_acceptable_dir(m_p_active_dir)) {
        return std::atomic_load(&m_p_active_dir);
    }

    m_is_rotating_process = true;
    m_p_active_dir.reset();

    for (size_t i = 0; ! is_acceptable_dir(m_p_active_dir) && ! (m_is_stop_fn() || i > 100); ++i) {
        if (! m_root_dir.is_open()) {
            assert(! m_p_primary_dir.get());
            assert(! m_p_active_dir.get());
            if (! m_root_dir.open() || ! m_root_dir.lock(false)) {
                break;
            }
        }

        if (is_acceptable_dir(m_p_primary_dir) || rotate_tmp_dir(m_root_dir.fd(), m_p_primary_dir)) {
            m_p_active_dir.reset();
            if (rotate_tmp_dir(m_p_primary_dir->fd(), m_p_active_dir)) {
                break;
            }
        }

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        lock.lock();
    }

    m_is_rotating_process = false;
    m_dir_cv.notify_all();

    return std::atomic_load(&m_p_active_dir);
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
