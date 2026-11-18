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

#ifndef _LIBS_GUT_STAGEITEM_H_
#define _LIBS_GUT_STAGEITEM_H_

#include <memory>
#include <string_view>

#include "gut/logging_handler.h"
#include "gut/details/entry.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

class queue;
class sink_queue;

} // namespace fw
} // namespace details

class stagequeue;

/**
 *  \brief  Transactional item of a file queue.
 *
 *  \details    An item encapsulates an open file descriptor locked exclusively inside
 *      the 'proc' stage. It guarantees atomic state transitions via POSIX 'renameat'.
 *
 *      If an item instance is destroyed without an explicit call to the commit() or
 *      rollback() methods, it will remain in the 'proc' directory.
 *
 *  Example usage
 *  \code{.cpp}
 *  // Writing data to a new item
 *  if (stageitem::ptr p_item = input_queue->create()) {
 *      const std::string payload = "Hello, FileQueue!";
 *      if (p_item->write(payload.data(), payload.size(), 0) == static_cast<ssize_t>(payload.size())) {
 *          p_item->commit();
 *      }
 *  }
 *
 *  // Reading data via memory mapping
 *  if (stageitem::ptr p_item = output_queue->pop(500)) {
 *      if (const void* ptr = p_item->map()) {
 *          // Process r_item->size() bytes at ptr...
 *          r_item->commit();
 *      }
 *  }
 *  \endcode
 *
 *  \warning    Individual item instances are **not thread-safe**. Do not share a
 *      single item across multiple threads without external synchronization.
 */
class stageitem final
{
public:
    using ptr = std::unique_ptr<stageitem>;

public:
    ~stageitem();

    /// \brief  Commits the item and moves it to the target output stage.
    /// \return true if the element was successfully committed, otherwise false.
    bool commit();

    /// \brief  Checks whether the item still holds an active.
    /// \return true if the item is alive and has not been removed; false otherwise.
    bool is_alive() const;

    /// \brief  Checks whether the item's file is currently mapped into memory.
    /// \return true if a memory mapping created by map() is active; false otherwise.
    bool is_mapped() const;

    /// \brief  Maps the entire contents of the item's file into read-only virtual memory.
    /// \return Pointer to the mapped memory region on success, or nullptr if 'mmap()' failed.
    const void* map();

    /// \brief  Maps a specific byte range of the item's file into read-only virtual memory.
    /// \param  offset - starting byte offset within the file.
    /// \param  size - number of bytes to map.
    /// \return Pointer to the mapped memory region on success, or nullptr if 'mmap()' failed.
    const void* map(const off64_t offset, const size_t size);

    /// \brief  Reads up to size bytes from the item's file starting at offset.
    /// \param  offset - byte offset in the file from which to begin reading.
    /// \param  p_buffer - destination buffer to receive the data (must not be nullptr).
    /// \param  size - maximum number of bytes to read.
    /// \return Number of bytes actually read, or -1 on error.
    ssize_t read(const off64_t offset, char* p_buffer, const size_t size);

    /// \brief  Permanently deletes the item's underlying file from disk.
    /// \return true if the file was successfully removed; false on error.
    bool remove();

    /// \brief  Rollbacks the processing of an element.
    /// \return true if the element was successfully rolled back, otherwise false.
    bool rollback();

    /// \brief  Returns the current size of the item's file in bytes.
    /// \return File size in bytes on success, or -1 if the item is not alive or 'fstat()' failed.
    ssize_t size() const;

    /// \brief  Resizes the item's file to exactly size bytes.
    /// \param  size - desired file size in bytes.
    /// \return true if the file was successfully resized; false on error.
    bool truncate(const size_t size);

    /// \brief  Releases the active memory mapping created by map().
    /// \see map(), is_mapped()
    void unmap();

    /// \brief  Writes size bytes from p_data into the item's file starting at offset.
    /// \param  p_data - the source data buffer (must not be nullptr).
    /// \param  size - number of bytes to write.
    /// \param  offset - byte offset in the file at which to begin writing.
    /// \return Number of bytes actually written on success, or -1 on error.
    ssize_t write(const char* p_data, const ssize_t size, const off64_t offset);

private:
    friend class stagequeue;
    friend ptr std::make_unique<stageitem>(::wstux::gut::details::fq::sink_queue&,
        ::wstux::gut::details::fq::queue&, const ::wstux::gut::logging_handler::ptr&);

    stageitem(details::fq::sink_queue& commit_queue, details::fq::queue& rollback_queue, const logging_handler::ptr& p_logger);

private:
    const logging_handler::ptr& m_p_logger;
    details::entry m_entry;

    details::fq::sink_queue& m_commit_queue;
    details::fq::queue& m_rollback_queue;
    bool m_is_moved = false;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_STAGEITEM_H_ */
