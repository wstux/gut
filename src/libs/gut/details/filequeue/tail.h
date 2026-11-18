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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_TAIL_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_TAIL_H_

#include <memory>
#include <set>
#include <string>

#include "gut/logging_handler.h"
#include "gut/details/directory.h"
#include "gut/details/fwd.h"
#include "gut/details/filequeue/notifier.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

/**
 *  \brief  Thread-safe chronological reader and K-way merger for the queue's stage.
 *
 *  \details    Monitors the stage nook directory via Linux ::inotify and maintains a
 *      bounded sliding window of active primary_tier instances, backed by a full sorted
 *      pool of discovered primary directory names.
 *
 *      Merges items across active tiers in strict chronological order, automatically
 *      evicts exhausted subdirectories to cleaner, and blocks efficiently when the
 *      queue is empty until new filesystem events arrive or the wait timeout expires.
 *
 *  \note   All public member functions are **thread-safe**.
 */
class tail final
{
public:
    using cleaner_ptr_t = std::shared_ptr<cleaner>;

public:
    /// \brief  Constructs a reading tail bound to the stage nook directory.
    /// \param  nook_dir - absolute path to the target stage directory.
    /// \param  p_cleaner - shared cleaner instance for removing exhausted rotation directories.
    /// \param  p_logger - the logging handler pointer.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    tail(const directory& nook_dir, const cleaner_ptr_t& p_cleaner, const logging_handler::ptr& p_logger,
         const std::function<bool()>& is_stop_fn);

    /// \brief  Unregisters filesystem watches and releases all held directory handles.
    ~tail();

    /// \brief  Retrieves the oldest available item from the disk-based queue.
    /// \param  filename - output string populated with the name of the discovered task file.
    /// \param  p_dir - output pointer populated with the open directory handle containing file.
    /// \param  wait_ms - maximum time to wait for a new item in milliseconds (0 for non-blocking poll).
    /// \return true if an item was successfully retrieved; false otherwise.
    bool get(std::string& filename, tmp_dir_ptr_t& p_dir, size_t wait_ms);

    /// \brief  Initializes ::inotify monitoring on the root directory and loads existing tiers.
    /// \return true if the watch was registered and initial state loaded; false on error.
    bool start();

private:
    class primary_tier;
    using primary_tier_ptr_t = std::shared_ptr<primary_tier>;

private:
    /// \brief Rescans nook directory to recover from ::inotify queue overflows (IN_Q_OVERFLOW)
    ///     and refills the sliding window of active primary tiers.
    bool reload();

private:
    const directory& m_nook_dir;
    const logging_handler::ptr& m_p_logger;
    const std::function<bool()> m_is_stop_fn;

    cleaner_ptr_t m_p_cleaner;

    tmp_dir_ptr_t m_p_active_dir;

    std::mutex m_tail_mutex;
    notifier m_notifier;
    int m_root_wd = -1;
    std::set<std::string> m_primary_pool;
    std::map<std::string, primary_tier_ptr_t> m_primary_tiers;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_TAIL_H_ */
