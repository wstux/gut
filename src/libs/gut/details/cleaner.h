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

#ifndef _LIBS_GUT_DETAILS_CLEANER_H_
#define _LIBS_GUT_DETAILS_CLEANER_H_

#include <memory>
#include <mutex>

#include "gut/details/circular_buffer.h"
#include "gut/details/dir.h"
#include "gut/details/types.h"

namespace wstux {
namespace gut {
namespace details {

class cleaner final
{
public:
    using ptr = std::shared_ptr<cleaner>;

    explicit cleaner(is_stop_fn_t is_stop_fn);

    /**
     *  \brief  Adds a directory to the queue for deferred cleanup.
     *  \param  p_dir - pointer to the directory.
     *  \return true if successfully added, false if already in the queue.
     */
    bool push(const tmp_dir::ptr& p_dir);

    /**
     *  \brief  Performs one complete cleanup cycle for all directories accumulated
     *      in the buffer.
     *  \param  dir_gap_seconds - the minimum "inactivity" period for a directory
     *      before it can be deleted (in seconds).
     *  \param  file_gap_seconds - the minimum "inactivity" period for a file before
     *      it can be deleted (in seconds).
     *  \return true - the circular task buffer is completely empty, and no residual
     *      data remains on the disk. false - some folders remain in the buffer,
     *      having been deferred due to files being in use.
     */
    bool step(int dir_gap_seconds, int file_gap_seconds);

private:
    using busy_record_t = std::array<char, gk_name_buffer_size>;

private:
    bool perform(const tmp_dir::ptr& p_dir, int file_gap_seconds);

    bool pop(tmp_dir::ptr& p_dir);

    bool remove_file(dir_reader& reader, const char* name);

private:
    static constexpr size_t k_max_files_per_dir = 3000;
    static constexpr size_t k_queue_size = 4096;

    const is_stop_fn_t m_is_stop_fn;

    std::mutex m_mutex;

    std::array<busy_record_t, k_max_files_per_dir> m_busy_pool;
    size_t m_busy_pool_size = 0;

    circular_buffer<tmp_dir::ptr, k_queue_size> m_dir_queue;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_CLEANER_H_ */
