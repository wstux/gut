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

#ifndef _LIBS_GUT_FILEQUEUE_H_
#define _LIBS_GUT_FILEQUEUE_H_

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "gut/fileitem.h"
#include "gut/logging_handler.h"
#include "gut/details/fwd.h"
#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {

class sink_filequeue final
{
public:
    using item = fileitem;
    using ptr = std::unique_ptr<sink_filequeue>;

public:
    /// \brief  Creates a new, empty item in the queue's working directory (`proc`).
    /// \return a pointer to the created element, or nullptr in the event of a file
    ///     system error or if the stop flag is triggered.
    item::ptr create();

    /// \brief  Returns the root path of the current queue on the file system.
    /// \return Thre current queue root path.
    const std::string& path() const { return m_path; }

    /// \brief  Add an element to the queue.
    /// \param  p_item - an element that needs to be placed into the queue.
    /// \return true if the element was successfully pushed, otherwise false.
    bool push(const item::ptr& p_item);

    /// \brief  Creating a queue.
    /// \param  path - the path to the queue's root directory.
    /// \param  is_stop_fn - a functor for interrupting blocking operations when
    ///     an application shuts down.
    /// \return A pointer to an initialized queue, or nullptr in case of an error.
    /// \details    Creates a directory structure at the specified path, configures
    ///     access permissions, and opens them.
    static ptr create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn,
                            logging_handler::ptr p_logger = logging_handler::ptr());

private:
    friend filequeue;
    friend ptr std::make_unique<sink_filequeue>(const std::string_view&, const std::function<bool()>&,
        ::wstux::gut::logging_handler::ptr&&);

    sink_filequeue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger);

    bool open();

private:
    const std::string m_path;
    const std::function<bool()> m_is_stop_fn;

    details::fq::sink_queue m_queue;

    logging_handler::ptr m_p_logger;
};

class filequeue final
{
public:
    using item = fileitem;
    using ptr = std::unique_ptr<filequeue>;

public:
    /// \brief  Creates a new, empty item in the queue's working directory (`proc`).
    /// \return a pointer to the created element, or nullptr in the event of a file
    ///     system error or if the stop flag is triggered.
    item::ptr create();

    /// \brief  Returns the root path of the current queue on the file system.
    /// \return Thre current queue root path.
    const std::string& path() const { return m_path; }

    /// \brief  Retrieves the next available item from the incoming queue.
    /// \param  queue - target queue to which the element will be moved.
    /// \param  wait_ms - the maximum time to wait for a new element to appear, in
    ///     milliseconds. Pass `0` for a non-blocking check.
    /// \return A pointer to the retrieved element, or nullptr if the queue is empty
    ///     upon expiration of the timeout or if a stop signal is received.
    /// \details    Atomically moves a file from input directory to the working directory
    ///     and acquires an exclusive lock on the file. If the incoming queue is empty,
    ///     the current thread blocks until a new file appears or a timeout occurs.
    item::ptr pop(filequeue& queue, size_t wait_ms);

    /// \brief  Retrieves the next available item from the incoming queue.
    /// \param  queue - target queue to which the element will be moved.
    /// \param  wait_ms - the maximum time to wait for a new element to appear, in
    ///     milliseconds. Pass `0` for a non-blocking check.
    /// \return A pointer to the retrieved element, or nullptr if the queue is empty
    ///     upon expiration of the timeout or if a stop signal is received.
    /// \details    Atomically moves a file from input directory to the working directory
    ///     and acquires an exclusive lock on the file. If the incoming queue is empty,
    ///     the current thread blocks until a new file appears or a timeout occurs.
    item::ptr pop(sink_filequeue& queue, size_t wait_ms);

    /// \brief  Add an element to the queue.
    /// \param  p_item - an element that needs to be placed into the queue.
    /// \return true if the element was successfully pushed, otherwise false.
    bool push(const item::ptr& p_item);

    /// \brief  Creating a queue.
    /// \param  path - the path to the queue's root directory.
    /// \param  is_stop_fn - a functor for interrupting blocking operations when
    ///     an application shuts down.
    /// \return A pointer to an initialized queue, or nullptr in case of an error.
    /// \details    Creates a directory structure at the specified path, configures
    ///     access permissions, and opens them.
    static ptr create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn,
                            logging_handler::ptr p_logger = logging_handler::ptr());

private:
    friend ptr std::make_unique<filequeue>(const std::string_view&, const std::function<bool()>&,
        ::wstux::gut::logging_handler::ptr&&);

    filequeue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger);

    bool open();

private:
    const std::string m_path;
    const std::function<bool()> m_is_stop_fn;

    details::fq::cleaner_ptr_t m_p_cleaner;

    details::fq::queue m_queue;

    logging_handler::ptr m_p_logger;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_FILEQUEUE_H_ */
