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

#include <time.h>

#include <cassert>
#include <atomic>
#include <thread>

#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

bool create_queue_item(entry& e, const std::string_view& name, head& queue_head)
{
    assert(! e.is_alive());
    tmp_dir::ptr p_active_dir = queue_head.get_active_dir(false);
    if (! p_active_dir) {
        return false;
    }

    return e.create(p_active_dir, name);
}

size_t monotime()
{
    struct ::timespec tv;
    if (::clock_gettime(CLOCK_MONOTONIC, &tv) == 0) {
        return static_cast<size_t>(tv.tv_sec);
    }

    return static_cast<size_t>(::time(nullptr));
}

bool open_queue(dir_t& root_dir)
{
    assert(! root_dir.is_open());

    if (! root_dir.open()) {
        return false;
    }
    if (! root_dir.lock(false)) {
        return false;
    }
    return true;
}

bool push_queue_item(entry& e, head& queue_head)
{
    assert(e.is_alive());
    tmp_dir::ptr p_dir = queue_head.get_active_dir(false);
    if (! p_dir) {
        return false;
    }
    if (e.move(p_dir)) {
        return true;
    }
    if (errno == EEXIST) {
        p_dir = queue_head.get_active_dir(true);
        return e.move(p_dir);
    }
    return false;
}

} // <anonymous> namespace

////////////////////////////////////////////////////////////////////////////////
// class sink_queue

sink_queue::sink_queue(const std::string_view& root_dir, const std::function<bool()>& is_stop_fn)
    : m_root_dir(root_dir)
    , m_queue_head(m_root_dir, is_stop_fn)
{}

sink_queue::~sink_queue()
{
    if (m_root_dir.is_open()) {
        m_root_dir.unlock();
    }
}

bool sink_queue::create_item(entry& e, const std::string_view& name)
{
    return create_queue_item(e, name, m_queue_head);
}

bool sink_queue::open()
{
    return open_queue(m_root_dir);
}

bool sink_queue::push_item(entry& e)
{
    return push_queue_item(e, m_queue_head);
}

////////////////////////////////////////////////////////////////////////////////
// class queue

queue::queue(const std::string_view& root_dir, const cleaner::ptr& p_cleaner, const std::function<bool()>& is_stop_fn)
    : m_is_stop_fn(is_stop_fn)
    , m_root_dir(root_dir)
    , m_p_cleaner(p_cleaner)
    , m_queue_head(m_root_dir, m_is_stop_fn)
    , m_queue_tail(m_root_dir, m_p_cleaner, m_is_stop_fn)
{}

queue::~queue()
{
    if (m_root_dir.is_open()) {
        m_root_dir.unlock();
    }
}

bool queue::create_item(entry& e, const std::string_view& name)
{
    return create_queue_item(e, name, m_queue_head);
}

bool queue::open()
{
    assert(! m_root_dir.is_open());

    if (! open_queue(m_root_dir)) {
        return false;
    }
    if (! m_queue_tail.start()) {
        return false;
    }
    return true;
}

template<typename TTargetQueue>
bool queue::pop_item(entry& e, TTargetQueue& target, const size_t wait_ms)
{
    assert(! e.is_alive());

    std::string filename;
    tmp_dir::ptr p_file_dir;

    while (! m_is_stop_fn()) {
        if (m_queue_tail.get(filename, p_file_dir, wait_ms)) {
            if (! e.open(p_file_dir, filename)) {
                continue;
            }

            if (target.push_item(e)) {
                return true;
            } else {
                e.close();
                return false;
            }
        }
        const size_t cur_time = monotime();
        if (m_cleaner_ts < cur_time) {
            m_p_cleaner->step(5, 120);
            m_cleaner_ts = cur_time;
        }
        //size_t expected_ts = m_cleaner_ts.load(std::memory_order_relaxed);
        //if (expected_ts < cur_time && m_cleaner_ts.compare_exchange_strong(expected_ts, cur_time, std::memory_order_acq_rel)) {
        //    m_p_cleaner->step(5, 120);
        //}
        return false;
    }

    return false;
}

bool queue::push_item(entry& e)
{
    return push_queue_item(e, m_queue_head);
}

template bool queue::pop_item<sink_queue>(entry&, sink_queue&, size_t);
template bool queue::pop_item<queue>(entry&, queue&, size_t);

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
