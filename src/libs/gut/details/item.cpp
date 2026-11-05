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

item::item(details::fq::queue* p_commit_queue, details::fq::queue* p_rollback_queue, const details::is_stop_fn_t& is_stop_fn)
    : m_is_stop_fn(is_stop_fn)
    , m_p_commit_queue(p_commit_queue)
    , m_p_rollback_queue(p_rollback_queue)
{}

bool item::commit()
{
    assert(m_entry.is_alive());
    assert(m_p_commit_queue);
    assert(! m_p_current_queue);

    const bool rc = m_p_commit_queue->push_item(m_entry);
    if (rc) {
        m_p_current_queue = m_p_commit_queue;
        m_entry.close();
        return true;
    }

    return false;
}

bool item::rollback()
{
    assert(m_entry.is_alive());
    assert(m_p_rollback_queue);
    assert(! m_p_current_queue);

    const bool rc = m_p_rollback_queue->push_item(m_entry);
    if (rc) {
        m_p_current_queue = m_p_rollback_queue;
        m_entry.close();
        return true;
    }

    return false;
}

} // namespace gut
} // namespace wstux
