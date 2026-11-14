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

#ifndef _LIBS_GUT_QUEUE_H_
#define _LIBS_GUT_QUEUE_H_

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "gut/item.h"
#include "gut/details/fwd.h"
#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {

/**
 *  \brief  Transactional, thread-safe, and inter-process file-based queue.
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
 *      1. Creating an item: the file is created within the internal `proc` directory.
 *         It remains invisible to readers until `item::commit()` is called.
 *      2. Popping an item: the reader identifies the chronologically oldest file within
 *         `input` (using a K-way merge algorithm across rotation subdirectories),
 *         atomically moves it to `proc`, and returns a pointer to the `item`. If the
 *         queue is empty, the thread sleeps-waiting via `poll()` - for a duration of
 *         `wait_ms`.
 *      3. Processing complete:
 *         - `commit` moves an element from `proc` to `output` (or to the `input` of
 *           the target queue);
 *         - `rollback` returns an item from `proc` back to the `input` of the current queue.
 */
class queue final
{
public:
    using ptr = std::unique_ptr<queue>;

public:
    /// \brief  Creates a new, empty item in the queue's working directory (`proc`).
    /// \return a pointer to the created element, or nullptr in the event of a file
    ///     system error or if the stop flag is triggered.
    item::ptr create();

    /// \brief  Returns the root path of the current queue on the file system.
    /// \return Thre current queue root path.
    const std::string& path() const { return m_path; }

    /// \brief  Retrieves the next available item from the incoming queue.
    /// \param  wait_ms - the maximum time to wait for a new element to appear, in
    ///     milliseconds. Pass `0` for a non-blocking check.
    /// \return A pointer to the retrieved element, or nullptr if the queue is empty
    ///     upon expiration of the timeout or if a stop signal is received.
    /// \details    Atomically moves a file from input directory to the working directory
    ///     and acquires an exclusive lock on the file. If the incoming queue is empty,
    ///     the current thread blocks until a new file appears or a timeout occurs.
    item::ptr pop(size_t wait_ms);

    /// \brief  Creating a queue.
    /// \param  path - the path to the queue's root directory.
    /// \param  is_stop_fn - a functor for interrupting blocking operations when
    ///     an application shuts down.
    /// \return A pointer to an initialized queue, or nullptr in case of an error.
    /// \details    Creates a directory structure at the specified path, configures
    ///     access permissions, and opens them.
    static ptr create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn);

    /// \brief  Creating a queue associated with a destination output queue.
    /// \param  path - the path to the queue's root directory.
    /// \param  out_path - the path to the root directory of the target recipient queue.
    /// \param  is_stop_fn - a functor for interrupting blocking operations when
    ///     an application shuts down.
    /// \return A pointer to an initialized queue, or nullptr in case of an error.
    /// \details    Creates a directory structure at the specified path, configures
    ///     access permissions, and opens them.
    ///     Instead of a local output directory, it uses the input directory of a
    ///     queue located at the provided path. Calling item::commit() on items in
    ///     this queue will deliver them directly into the input pool of the target queue.
    static ptr create_queue(const std::string_view& path, const std::string_view& out_path, const std::function<bool()>& is_stop_fn);

private:
    friend std::unique_ptr<queue> std::make_unique<queue>(const std::string_view&, const std::function<bool()>&);
    friend std::unique_ptr<queue> std::make_unique<queue>(const std::string_view&, const std::string_view&, const std::function<bool()>&);

    queue(const std::string_view& path, const std::function<bool()>& is_stop_fn);

    queue(const std::string_view& path, const std::string_view& out_path, const std::function<bool()>& is_stop_fn);

    details::fq::queue& input_queue() { return m_input; }

    bool open();

private:
    const std::string m_path;
    const std::function<bool()> m_is_stop_fn;

    details::fq::cleaner_ptr_t m_p_cleaner;

    details::fq::queue m_input;
    details::fq::sink_queue m_proc;
    details::fq::sink_queue m_output;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_QUEUE_H_ */
