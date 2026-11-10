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
#include <filesystem>

#include "gut/queue.h"

namespace wstux {
namespace gut {
namespace {

std::string make_queue_path(const std::string_view& root_path, const std::string_view& queue_name)
{
    assert(root_path.size() > 0);

    std::filesystem::path path = root_path;
    path = path / queue_name;
    return path.string();
}

}

queue::queue(const std::string_view& path, const is_stop_fn_t& is_stop_fn)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_input(make_queue_path(path, "input"), m_p_cleaner, m_is_stop_fn)
    , m_proc(make_queue_path(path, "proc"), m_p_cleaner, m_is_stop_fn)
    , m_output(make_queue_path(path, "output"), m_p_cleaner, m_is_stop_fn)
{}

queue::queue(const std::string_view& path, const std::string_view& out_path, const is_stop_fn_t& is_stop_fn)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_input(make_queue_path(path, "input"), m_p_cleaner, m_is_stop_fn)
    , m_proc(make_queue_path(path, "proc"), m_p_cleaner, m_is_stop_fn)
    , m_output(make_queue_path(out_path, "input"), m_p_cleaner, m_is_stop_fn)
{}

item::ptr queue::create()
{
    item::ptr p_item = std::make_unique<item>(&m_output, &m_input, m_is_stop_fn);

    if (! m_proc.create_item(p_item->m_entry)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

queue::ptr queue::create_queue(const std::string_view& path, const is_stop_fn_t& is_stop_fn)
{
    namespace fs = std::filesystem;

    if (path.empty()) {
        return nullptr;
    }

    std::error_code error;
    fs::create_directories(path, error);
    if (error) {
        return nullptr;
    }

    fs::perms perms = fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read;
    fs::permissions(path, perms, error);
    if (error) {
        return nullptr;
    }
    //return std::make_unique<queue>(path, is_stop_fn);
    queue::ptr p_queue = std::make_unique<queue>(path, is_stop_fn);
    if (! p_queue->m_input.start()) {
        return nullptr;
    }
    if (! p_queue->m_proc.start()) {
        return nullptr;
    }
    if (! p_queue->m_output.start()) {
        return nullptr;
    }
    return p_queue;
}

queue::ptr queue::create_queue(const std::string_view& path, const std::string_view& out_path, const is_stop_fn_t& is_stop_fn)
{
    namespace fs = std::filesystem;

    if (path.empty() || out_path.empty()) {
        return nullptr;
    }

    {
        // Create queue dirs
        std::error_code error;
        if (! fs::create_directories(path, error)) {
            return nullptr;
        }

        fs::perms perms = fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read;
        fs::permissions(path, perms, error);
        if (error) {
            return nullptr;
        }
    }

    {
        // Create output dirs
        std::error_code error;
        const fs::path out_dir = fs::path(out_path) / "input";
        fs::create_directories(out_dir, error);
        if (error) {
            return nullptr;
        }

        fs::perms perms = fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read;
        fs::permissions(out_path, perms, error);
        if (error) {
            return nullptr;
        }
        fs::permissions(out_dir, perms, error);
        if (error) {
            return nullptr;
        }
    }
    queue::ptr p_queue = std::make_unique<queue>(path, out_path, is_stop_fn);
    if (! p_queue->m_input.start()) {
        return nullptr;
    }
    if (! p_queue->m_proc.start()) {
        return nullptr;
    }
    if (! p_queue->m_output.start()) {
        return nullptr;
    }
    return p_queue;
}

item::ptr queue::pop(size_t /*wait_ms*/)
{
    item::ptr p_item = std::make_unique<item>(&m_output, &m_input, m_is_stop_fn);

    if (! m_input.pop_item(p_item->m_entry, m_proc)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

} // namespace gut
} // namespace wstux
