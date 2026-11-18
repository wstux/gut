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

#include "gut/stageitem.h"
#include "gut/details/logging.h"
#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {

stageitem::stageitem(details::fq::sink_queue& commit_queue, details::fq::queue& rollback_queue, const logging_handler::ptr& p_logger)
    : m_p_logger(p_logger)
    , m_entry(m_p_logger)
    , m_commit_queue(commit_queue)
    , m_rollback_queue(rollback_queue)
{}

stageitem::~stageitem()
{
    m_entry.close();
}

/**
 *  \details    Atomically moves a file from the working directory to the target output
 *      directory and releases the resources being used. Following a successful call,
 *      the object becomes inactive.
 */
bool stageitem::commit()
{
    assert(m_entry.is_alive());
    assert(! m_is_moved);

    if (m_is_moved) {
        GUT_LOG_ERROR(m_p_logger, "Item '%s' already has been commited", m_entry.name().data());
        return false;
    }

    const bool rc = m_commit_queue.push_item(m_entry);
    if (rc) {
        m_is_moved = true;
        m_entry.close();
        return true;
    }

    return false;
}

bool stageitem::is_alive() const
{
    return m_entry.is_alive();
}

bool stageitem::is_mapped() const
{
    return m_entry.is_mapped();
}

/**
 *  \details    Convenience overload that maps size() bytes starting at offset 0. If a
 *      mapping already exists, it is replaced or reused depending on implementation.
 *
 *  \note   The returned pointer remains valid until unmap(), commit(), rollback(),
 *      remove(), or the destructor is called.
 */
const void* stageitem::map()
{
    const off64_t file_size = size();
    if (file_size > 0) {
        return m_entry.map(0, file_size);
    }
    return nullptr;
}

const void* stageitem::map(const off64_t offset, const size_t size)
{
    if (size > 0 && ((off64_t)(offset + size) > 0) && (stageitem::size() >= (off64_t)(offset + size))) {
        return m_entry.map(offset, size);
    }
    return nullptr;
}

ssize_t stageitem::read(const off64_t offset, char* p_buffer, const size_t size)
{
    return m_entry.read(offset, p_buffer, size);
}

/**
 *  \details    Unlinks the file from 'proc', and closes the descriptor. After a successful
 *      call, is_alive() becomes 'false'.
 */
bool stageitem::remove()
{
    if (is_mapped()) {
        return false;
    }
    return m_entry.remove();
}

/**
 *  \details    Returns a file from the working directory back to the incoming directory
 *      of the current queue, making it available for retrieval once again.
 */
bool stageitem::rollback()
{
    assert(m_entry.is_alive());
    assert(! m_is_moved);

    if (m_is_moved) {
        GUT_LOG_ERROR(m_p_logger, "Item '%s' already has been rollbacked", m_entry.name().data());
        return false;
    }

    if (m_rollback_queue.push_item(m_entry)) {
        m_is_moved = true;
        m_entry.close();
        return true;
    }

    return false;
}

ssize_t stageitem::size() const
{
    return m_entry.size();
}

/**
 *  \details    If the file was larger than size, extra data is discarded. If it was
 *      smaller, it is extended and zero-filled. Any active memory mapping is unmapped
 *      prior to resizing.
 */
bool stageitem::truncate(const size_t size)
{
    return m_entry.truncate(size);
}

/**
 *  \details    Safe to call even if is_mapped() is false (no-op in that case).
 */
void stageitem::unmap()
{
    m_entry.unmap();
}

ssize_t stageitem::write(const char* p_data, const ssize_t size, const off64_t offset)
{
    return m_entry.write(p_data, size, offset);
}

} // namespace gut
} // namespace wstux
