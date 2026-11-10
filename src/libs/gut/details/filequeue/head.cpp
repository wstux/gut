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

#include <cassert>
#include <chrono>
#include <thread>
#include <type_traits>

#include "gut/details/dir.h"
#include "gut/details/filequeue/head.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

template<typename T>
bool create_tmp_dir(const T& patent, tmp_dir::ptr& p_dir)
{
    tmp_dir::ptr p_tmp_dir = std::make_shared<tmp_dir>(patent);
    if (! p_tmp_dir->create() || ! p_tmp_dir->lock(false)) {
        p_tmp_dir.reset();
        return false;
    }

    std::atomic_store(&p_dir, p_tmp_dir);
    return true;
}

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

}

head::head(const dir_t& root_dir, const is_stop_fn_t& is_stop_fn)
    : m_root_dir(root_dir)
    , m_is_stop_fn(is_stop_fn)
{
    static_assert(std::is_same<tmp_dir_ptr_t, tmp_dir::ptr>::value);
}

tmp_dir::ptr head::get_active_dir(bool force_rotation)
{
    assert(m_root_dir.is_open());

    if (! force_rotation) {
        tmp_dir::ptr p_active_dir = std::atomic_load(&m_p_active_dir);
        if (p_active_dir && is_acceptable_dir(p_active_dir)) {
            return p_active_dir;
        }
    }

    return rotate(force_rotation);
}

tmp_dir::ptr head::rotate(bool force_rotation)
{
    std::unique_lock<std::mutex> lock(m_dir_mutex);

    if (! force_rotation) {
        tmp_dir::ptr p_active_dir = std::atomic_load(&m_p_active_dir);
        if (p_active_dir && is_acceptable_dir(p_active_dir)) {
            return p_active_dir;
        }
    }

    std::atomic_store(&m_p_active_dir, tmp_dir::ptr());
    tmp_dir::ptr p_active_dir;
    for (size_t i = 0; ! is_acceptable_dir(p_active_dir) && i < 100 && ! m_is_stop_fn(); ++i) {
        assert(m_root_dir.is_open());

        if (is_acceptable_dir(m_p_primary_dir) || create_tmp_dir(m_root_dir.fd(), m_p_primary_dir)) {
            if (create_tmp_dir(m_p_primary_dir, m_p_active_dir)) {
                break;
            }
        }

        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        lock.lock();
    }

    return std::atomic_load(&m_p_active_dir);
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
