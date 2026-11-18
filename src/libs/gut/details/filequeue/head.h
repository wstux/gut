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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_HEAD_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_HEAD_H_

#include <functional>
#include <mutex>

#include "gut/logging_handler.h"
#include "gut/details/directory.h"
#include "gut/details/fwd.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

/**
 *  \brief  Thread-safe directory writer and rotator for incoming queue items.
 *
 *  \details    Manages the active two-level directory hierarchy (primary / active
 *      directories) where new items are created or moved into. Monitors directory
 *      metadata size and automatically rotates active_dir (and primary_dir when
 *      needed) once the 32 KB directory threshold is reached, preventing linear
 *      lookup degradation in Linux filesystems.
 *
 *      Holds a shared advisory lock (LOCK_SH) on active directories to prevent
 *      cleaner from prematurely deleting directories currently in use by writers.
 *
 *  \note   All public member functions are **thread-safe**.
 */
class head final
{
public:
    /// \brief  Constructs a writing head bound to the stage nook directory.
    /// \param  nook_dir - absolute path to the target stage directory.
    /// \param  p_logger - a pointer to a logging handler.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    head(const directory& nook_dir, const logging_handler::ptr& p_logger, const std::function<bool()>& is_stop_fn);

    /// \brief  Returns the currently active subdirectory for writing, rotating if necessary.
    /// \param  force_rotation - if true, forces immediate creation of a new active directory.
    /// \return An opened and shared-locked active directory, or nullptr if directory creation failed.
    tmp_dir_ptr_t get_active_dir(bool force_rotation = false);

private:
    /// \brief  Performs thread-safe rotation of m_p_active_dir and, if full, m_p_primary_dir.
    tmp_dir_ptr_t rotate(bool force_rotation);

private:
    const directory& m_nook_dir;
    const logging_handler::ptr& m_p_logger;
    const std::function<bool()> m_is_stop_fn;

    std::mutex m_dir_mutex;
    tmp_dir_ptr_t m_p_primary_dir;
    tmp_dir_ptr_t m_p_active_dir;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_HEAD_H_ */
