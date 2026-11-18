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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_QUEUE_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_QUEUE_H_

#include <atomic>
#include <string_view>

#include "gut/details/directory.h"
#include "gut/details/entry.h"
#include "gut/logging_handler.h"
#include "gut/details/filequeue/head.h"
#include "gut/details/filequeue/tail.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

/**
 *  \brief  Write-only queue stage managing item creation and atomic ingestion.
 *
 *  \details    The queue encapsulates a single queue directory equipped only with a
 *      writing head. It creates new temporary files and atomically moves existing
 *      entries into its rotated active subdirectories without maintaining a reading
 *      tail.
 *
 *  \note   Thread-safe for concurrent create_item() and push_item() calls once open()
 *      succeeds.
 */
class sink_queue final
{
public:
    /// \brief  Constructs a write-only queue stage bound to nook_dir.
    /// \param  nook_dir - absolute path to the target stage directory.
    /// \param  p_logger - the logging handler pointer.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    sink_queue(const std::string_view& nook_dir, const logging_handler::ptr& p_logger, const std::function<bool()>& is_stop_fn);

    /// \brief  Closes the underlying directory descriptor and releases head resources.
    ~sink_queue();

    /// \brief  Creates a new empty, exclusively locked file inside the active directory.
    /// \param  e - output entry handle initialized with the newly created file descriptor.
    /// \param  name - optional explicit filename; if empty, a unique chronological name is generated.
    /// \return true if the file was created and locked; false on error or shutdown.
    bool create_item(entry& e, const std::string_view& name = std::string_view());

    /// \brief  Opens or creates the root stage directory and initializes its writing head.
    /// \return true if the directory and head are ready for writes; false on error.
    bool open();

    /// \brief  Atomically moves an existing entry from another stage into this queue.
    /// \param  e - valid, locked entry handle to be transferred.
    /// \return true if the item was successfully moved; false on error.
    bool push_item(entry& e);

private:
    directory m_nook_dir;
    const logging_handler::ptr& m_p_logger;

    head m_queue_head;
};

/**
 *  \brief  Transactional file queue stage supporting both reading and writing.
 *
 *  \details    Combines a writing head, a K-way merging reading tail, and periodic
 *      background garbage collection via cleaner. Used for stages that require both
 *      ingestion and consumption.
 *
 *  \note   All public I/O operations are **thread-safe**.
 */
class queue final
{
public:
    /// \brief  Constructs a read-write queue stage.
    /// \param  nook_dir - absolute path to the target stage directory.
    /// \param  p_cleaner - shared cleaner instance for removing exhausted rotation directories.
    /// \param  p_logger - the logging handler pointer.
    /// \param  is_stop_fn - a functor for interrupting operations when an application shuts down.
    queue(const std::string_view& nook_dir, const cleaner_ptr_t& p_cleaner,
          const logging_handler::ptr& p_logger, const std::function<bool()>& is_stop_fn);

    /// \brief  Shuts down the reading tail and closes the underlying stage directory.
    ~queue();

    /// \brief  Creates a new empty, exclusively locked file inside the active directory.
    /// \param  e - output entry handle initialized with the newly created file descriptor.
    /// \param  name - optional explicit filename; if empty, a unique chronological name is generated.
    /// \return true if the file was created and locked; false on error or shutdown.
    bool create_item(entry& e, const std::string_view& name = std::string_view());

    /// \brief  Opens or creates the root stage directory and initializes its writing head and reading tail.
    /// \return true if the directory and head are ready for writes; false on error.
    bool open();

    /// \brief  Pops the oldest available item and atomically moves it into target.
    /// \param  e - output entry handle populated with the popped and locked item.
    /// \param  target - destination queue stage receiving the item during processing.
    /// \param  wait_ms - maximum time to wait for an item in milliseconds (0 for non-blocking).
    /// \return true if an item was popped and moved to target; false on timeout, shutdown, or error.
    /// \tparam TTargetQueue - destination queue type (queue or sink_queue).
    template<typename TTargetQueue>
    bool pop_item(entry& e, TTargetQueue& target, const size_t wait_ms);

    /// \brief  Atomically moves an existing entry from another stage into this queue.
    /// \param  e - valid, locked entry handle to be transferred.
    /// \return true if the item was successfully moved; false on error.
    bool push_item(entry& e);

private:
    const std::function<bool()> m_is_stop_fn;

    directory m_nook_dir;
    const logging_handler::ptr& m_p_logger;

    cleaner_ptr_t m_p_cleaner;
    std::atomic_size_t m_cleaner_ts = 0;

    head m_queue_head;
    tail m_queue_tail;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_QUEUE_H_ */
