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

#include <functional>
#include <memory>
#include <mutex>
#include <set>

#include "gut/details/utils/tmp_dir.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

/**
 *  \brief  Thread-safe incremental and full garbage collector for queue directories.
 *
 *  \details    Manages deferred removal of exhausted rotation directories (primary_dir
 *      and active_dir) in chronological order using a deduplicated std::set. Ensures
 *      that directories or files actively locked by writers or consumers (::flock`),
 *      or modified too recently, are safely skipped and retried on subsequent steps.
 *
 *      Also provides a static cleanup() routine for deep, offline or startup spool maintenance.
 *
 *  \note   All public member functions of cleaner are **thread-safe**.
 */
class cleaner final
{
public:
    using ptr = std::shared_ptr<cleaner>;

    /// \brief  Constructs a cleaner instance with a shutdown predicate.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    explicit cleaner(std::function<bool()> is_stop_fn);

    /// \brief  Executes a bounded pass over all directories currently queued in the cleaner.
    /// \param  dir_gap_seconds - minimum age in seconds before an empty directory can be removed.
    /// \param  file_gap_seconds - minimum age in seconds before an unlocked file can be removed.
    /// \return true if the internal directory queue is completely empty after the pass; false otherwise.
    bool cleanup(int dir_gap_seconds, int file_gap_seconds);

    /// \brief  Adds a directory to the queue for deferred cleanup.
    /// \param  p_dir - pointer to an open temporary directory.
    /// \return true if successfully added, false if already in the queue.
    bool push(const tmp_dir::ptr& p_dir);

    /// \brief  Processes the oldest directory in the queue.
    /// \param  dir_gap_seconds - minimum age in seconds before an empty directory can be removed.
    /// \param  file_gap_seconds - minimum age in seconds before an unlocked file can be removed.
    /// \return true if the cleaner queue is now completely empty; false otherwise.
    bool step(int dir_gap_seconds, int file_gap_seconds);

    /// \brief  Performs deep recursive garbage collection on a queue stage directory.
    /// \param  nook_dir - absolute path to the target stage directory to clean (the root itself is never removed).
    /// \param  dir_gap_seconds - minimum age in seconds before an empty directory can be removed.
    /// \param  file_gap_seconds - minimum age in seconds before an unlocked file can be removed (when remove_files is true).
    /// \param  remove_files - deletes unlocked stale files or removes only directories.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    /// \return true if all eligible garbage was removed; false otherwise.
    static bool cleanup(const std::string_view& nook_path, int dir_gap_seconds, int file_gap_seconds,
                        bool remove_files, const std::function<bool()>& is_stop_fn);
private:
    using busy_record_t = std::array<char, gk_name_buffer_size>;

    struct dir_less final {
        bool operator()(const tmp_dir::ptr& lhs, const tmp_dir::ptr& rhs) const noexcept {
            return lhs->name() < rhs->name();
        }
    };

private:
    /// \brief  Scans p_dir, queues subdirectories, removes eligible files, and unlinks p_dir if empty.
    bool perform(const tmp_dir::ptr& p_dir, int file_gap_seconds);

    /// \brief  Unlinks a single file relative to reader, treating ENOENT as success.
    bool remove_file(dir_reader& reader, const char* name);

private:
    const std::function<bool()> m_is_stop_fn;

    std::mutex m_mutex;

    std::set<tmp_dir::ptr, dir_less> m_dir_queue;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_CLEANER_H_ */
