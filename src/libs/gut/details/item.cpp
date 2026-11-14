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

#include "gut/item.h"
#include "gut/queue.h"

namespace wstux {
namespace gut {

item::item(details::fq::sink_queue* p_commit_queue, details::fq::queue* p_rollback_queue, const std::function<bool()>& is_stop_fn)
    : m_is_stop_fn(is_stop_fn)
    , m_p_commit_queue(p_commit_queue)
    , m_p_rollback_queue(p_rollback_queue)
{}

bool item::commit()
{
    assert(m_entry.is_alive());
    assert(m_p_commit_queue);
    //assert(! m_p_current_queue);
    assert(! m_is_moved);

    const bool rc = m_p_commit_queue->push_item(m_entry);
    if (rc) {
        //m_p_current_queue = m_p_commit_queue;
        m_is_moved = true;
        m_entry.close();
        return true;
    }

    return false;
}

bool item::is_alive() const
{
    return m_entry.is_alive();
}

bool item::is_mapped() const
{
    return m_entry.is_mapped();
}

const void* item::map()
{
    const off64_t file_size = size();
    if (file_size > 0) {
        return m_entry.map(0, file_size);
    }
    return nullptr;
}

const void* item::map(const off64_t offset, const size_t size)
{
    if (size > 0 && ((off64_t)(offset + size) > 0) && (item::size() >= (off64_t)(offset + size))) {
        return m_entry.map(offset, size);
    }
    return nullptr;
}

ssize_t item::read(const off64_t offset, char* p_buffer, const size_t size)
{
    return m_entry.read(offset, p_buffer, size);
}

bool item::remove()
{
    if (is_mapped()) {
        return false;
    }
    return m_entry.remove();
}

bool item::rollback()
{
    assert(m_entry.is_alive());
    assert(m_p_rollback_queue);
    //assert(! m_p_current_queue);
    assert(! m_is_moved);

    const bool rc = m_p_rollback_queue->push_item(m_entry);
    if (rc) {
        //m_p_current_queue = m_p_rollback_queue;
        m_is_moved = true;
        m_entry.close();
        return true;
    }

    return false;
}

ssize_t item::size() const
{
    return m_entry.size();
}

bool item::truncate(const size_t size)
{
    return m_entry.truncate(size);
}

void item::unmap()
{
    m_entry.unmap();
}

ssize_t item::write(const char* p_data, const ssize_t size, const off64_t offset)
{
    return m_entry.write(p_data, size, offset);
}

} // namespace gut
} // namespace wstux
