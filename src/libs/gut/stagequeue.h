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

#ifndef _LIBS_GUT_STAGEQUEUE_H_
#define _LIBS_GUT_STAGEQUEUE_H_

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "gut/logging_handler.h"
#include "gut/stageitem.h"
#include "gut/details/fwd.h"
#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {

/**
 *  \brief  Thread-safe and inter-process transactional file queue.
 *
 *  \details    The class provides a high-level interface for exchanging events via the
 *      file system. The queue ensures data integrity in the event of failures and
 *      supports concurrent operation by multiple writer processes and reader threads.
 *
 *      Within the root path, a queue manages three subdirectories:
 *      - input: incoming items awaiting processing. Tracked via inotify;
 *      - proc: a workspace where items are located while being created or processed;
 *      - output: outgoing items following a successful call to item::commit().
 *
 *      Operational logic and data lifecycle:
 *      1. Creating an item: the file is created within the internal 'proc' directory.
 *         It remains invisible to readers until 'item::commit()' is called.
 *      2. Popping an item: the reader identifies the chronologically oldest file within
 *         'input' (using a K-way merge algorithm across rotation subdirectories),
 *         atomically moves it to 'proc', and returns a pointer to the 'item'. If the
 *         queue is empty, the thread sleeps-waiting via 'poll()' - for a duration of
 *         'wait_ms'.
 *      3. Processing complete:
 *         - 'commit' moves an element from 'proc' to 'output' (or to the 'input' of
 *           the target queue);
 *         - 'rollback' returns an item from 'proc' back to the 'input' of the current queue.
 *
 *      To prevent filesystem degradation when accumulating hundreds of thousands of files,
 *      'stagequeue' automatically rotates subdirectories ('primary_dir / active_dir') once
 *      directory metadata reaches 32 KB (~1,500 entries). Readers merge active streams
 *      chronologically using a **K-way Merge** algorithm with a bounded sliding window
 *      of open directories.
 *
 *  Message Lifecycle
 *  \code{.text}
 *    [stagequeue::create()] ──► proc/ ──[item::commit()]──► output/
 *                                 │
 *                         [item::rollback()]
 *                                 ▼
 *                              input/ ──[stagequeue::pop()]──► proc/
 *  \endcode
 *
 *  Example usage
 *  \code{.cpp}
 *  auto stop_fn = []() { return false; };
 *  auto rx = wstux::gut::stagequeue::create_queue("/var/spool/app/rx", stop_fn);
 *  auto tx = wstux::gut::stagequeue::create_queue("/var/spool/app/tx", rx->path(), stop_fn);
 *
 *  // Produce an item
 *  if (auto w_item = tx->create()) {
 *      // Write payload to w_item...
 *      w_item->commit(); // Atomically moves item into rx/input
 *  }
 *
 *  // Consume an item (wait up to 500 ms)
 *  if (auto r_item = rx->pop(500)) {
 *      if (process(*r_item)) {
 *          r_item->commit();
 *      } else {
 *          r_item->rollback(); // Returns item back to rx/input
 *      }
 *  }
 *  \endcode
 *
 *  \note   All public member functions of 'stagequeue' are **thread-safe** and **process-safe**.
 *
 *  \attention  The library implements a one-message-per-file architecture optimized
 *      for medium to large payloads (several kilobytes to tens of megabytes). It is
 *      not recommended for tiny payloads (< 1–4 KB) due to SSD page/block write
 *      amplification and inode exhaustion.
 */
class stagequeue final
{
public:
    using item = stageitem;
    using ptr = std::unique_ptr<stagequeue>;

public:
    /// \brief  Creates a new, empty item in the queue's working directory ('proc').
    /// \return A pointer to the created element, or nullptr in the event of a file
    ///     system error or if the stop flag is triggered.
    item::ptr create();

    /// \brief  Returns the root path of the current queue on the file system.
    /// \return Thre current queue root path.
    const std::string& path() const { return m_path; }

    /// \brief  Retrieves the oldest available item from the input stage.
    /// \param  wait_ms - the time to wait for a new element to appear, in milliseconds.
    ///     Pass '0' for a non-blocking check.
    /// \return A pointer to the retrieved element, or nullptr if the queue is empty
    ///     upon expiration of the timeout or if a stop signal is received.
    item::ptr pop(size_t wait_ms);

    /// \brief  Performs deep garbage collection across the queue's directory hierarchy.
    /// \param  secs_offset - minimum age in seconds before an unlocked file in 'proc' can be purged.
    /// \param  path - the path to the queue's root directory.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    /// \return true if all eligible garbage was removed; false otherwise.
    static bool cleanup_garbage(int secs_offset, const std::string_view& path,
                                const std::function<bool()>& is_stop_fn, const std::string_view& out_path = std::string_view());

    /// \brief  Creating a queue.
    /// \param  path - the path to the queue's root directory.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    /// \param  p_logger - a pointer to a logging handler.
    /// \return A pointer to an initialized queue, or nullptr in case of an error.
    static ptr create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn,
                            logging_handler::ptr p_logger = logging_handler::ptr());

    /// \brief  Creating a queue associated with a destination output queue.
    /// \param  path - the path to the queue's root directory.
    /// \param  out_path - the path to the root directory of the target recipient queue.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    /// \param  p_logger - a pointer to a logging handler.
    /// \return A pointer to an initialized queue, or nullptr in case of an error.
    static ptr create_queue(const std::string_view& path, const std::string_view& out_path,
                            const std::function<bool()>& is_stop_fn, logging_handler::ptr p_logger = logging_handler::ptr());

    /// \brief  Restores dangling items from the proc directory back to input.
    /// \param  path - the root path of the queue.
    /// \param  rollback_fn - predicate for processing the element before rolling back.
    /// \param  p_logger - a pointer to a logging handler.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    /// \return true if all found elements were successfully rolled back, otherwise, false.
    static bool rollback(const std::string_view& path, const std::function<bool(item::ptr&)>& rollback_fn,
                         const logging_handler::ptr& p_logger, const std::function<bool()>& is_stop_fn);

private:
    friend ptr std::make_unique<stagequeue>(const std::string_view&,
        const std::function<bool()>&, ::wstux::gut::logging_handler::ptr&&);
    friend ptr std::make_unique<stagequeue>(const std::string_view&,
        const std::string_view&, const std::function<bool()>&, ::wstux::gut::logging_handler::ptr&&);

    stagequeue(const std::string_view& path, const std::function<bool()>& is_stop_fn,
               logging_handler::ptr&& p_logger);

    stagequeue(const std::string_view& path, const std::string_view& out_path,
               const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger);

    bool open();

private:
    const std::string m_path;
    const std::function<bool()> m_is_stop_fn;

    logging_handler::ptr m_p_logger;

    details::fq::cleaner_ptr_t m_p_cleaner;

    details::fq::queue m_input;
    details::fq::sink_queue m_proc;
    details::fq::sink_queue m_output;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_STAGEQUEUE_H_ */
