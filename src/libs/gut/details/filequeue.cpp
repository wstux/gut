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

#include "gut/filequeue.h"
#include "gut/details/logging.h"
#include "gut/details/filequeue/cleaner.h"

namespace wstux {
namespace gut {
namespace {

bool create_queue_dir(const std::filesystem::path& path, const logging_handler::ptr& p_logger)
{
    namespace fs = std::filesystem;

    std::error_code error;
    fs::create_directories(path, error);
    if (error) {
        GUT_LOG_ERROR(p_logger, "Failed to create filequeue directory %s", path.c_str());
        return false;
    }
    fs::perms perms = fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read;
    fs::permissions(path, perms, error);
    if (error) {
        GUT_LOG_ERROR(p_logger, "Failed to set permissions for filequeue directory %s", path.c_str());
        return false;
    }
    return true;
}

} // <anonymous> namespace

////////////////////////////////////////////////////////////////////////////////
// class sink_filequeue

sink_filequeue::sink_filequeue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_logger(std::move(p_logger))
    , m_queue(path, m_p_logger, m_is_stop_fn)
{
    static_assert(std::is_same<details::fq::cleaner_ptr_t, details::fq::cleaner::ptr>::value);
}

sink_filequeue::item::ptr sink_filequeue::create()
{
    item::ptr p_item = std::make_unique<item>(m_p_logger);

    if (! m_queue.create_item(p_item->m_entry)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

sink_filequeue::ptr sink_filequeue::create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn,
                                                 logging_handler::ptr p_logger)
{
    if (path.empty()) {
        GUT_LOG_ERROR(p_logger, "Sink filequeue directory path is empty");
        return nullptr;
    }

    if (! create_queue_dir(path, p_logger)) {
        return nullptr;
    }
    //return std::make_unique<queue>(path, is_stop_fn);
    sink_filequeue::ptr p_queue = std::make_unique<sink_filequeue>(path, is_stop_fn, std::move(p_logger));
    if (! p_queue->open()) {
        GUT_LOG_ERROR(p_queue->m_p_logger, "Failed to open sink filequeue directory %s", p_queue->m_path.c_str());
        return nullptr;
    }
    GUT_LOG_DEBUG(p_queue->m_p_logger, "Inited sink filequeue in directory %s", p_queue->m_path.c_str());
    return p_queue;
}

bool sink_filequeue::open()
{
    if (! m_queue.open()) {
        GUT_LOG_ERROR(m_p_logger, "Failed to open queue directory");
        return false;
    }
    return true;
}

bool sink_filequeue::push(const item::ptr& p_item)
{
    assert(p_item);

    const bool rc = m_queue.push_item(p_item->m_entry);
    if (rc) {
        assert(p_item->m_entry.is_alive());
    }
    return rc;
}

////////////////////////////////////////////////////////////////////////////////
// class filequeue

filequeue::filequeue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_logger(std::move(p_logger))
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_queue(path, m_p_cleaner, m_p_logger, m_is_stop_fn)
{
    static_assert(std::is_same<details::fq::cleaner_ptr_t, details::fq::cleaner::ptr>::value);
}

bool filequeue::cleanup_garbage(int secs_offset, const std::string_view& path, const std::function<bool()>& is_stop_fn, bool remove_files)
{
    return details::fq::cleaner::cleanup(path, 0, secs_offset, remove_files, is_stop_fn);
}

filequeue::item::ptr filequeue::create()
{
    item::ptr p_item = std::make_unique<item>(m_p_logger);

    if (! m_queue.create_item(p_item->m_entry)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

filequeue::ptr filequeue::create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn,
                                       logging_handler::ptr p_logger)
{
    if (path.empty()) {
        GUT_LOG_ERROR(p_logger, "Filequeue directory path is empty");
        return nullptr;
    }

    if (! create_queue_dir(path, p_logger)) {
        return nullptr;
    }
    //return std::make_unique<queue>(path, is_stop_fn);
    filequeue::ptr p_queue = std::make_unique<filequeue>(path, is_stop_fn, std::move(p_logger));
    if (! p_queue->open()) {
        GUT_LOG_ERROR(p_queue->m_p_logger, "Failed to open filequeue directory %s", p_queue->m_path.c_str());
        return nullptr;
    }
    GUT_LOG_DEBUG(p_queue->m_p_logger, "Inited filequeue in directory %s", p_queue->m_path.c_str());
    return p_queue;
}

bool filequeue::open()
{
    if (! m_queue.open()) {
        GUT_LOG_ERROR(m_p_logger, "Failed to open queue directory");
        return false;
    }
    return true;
}

filequeue::item::ptr filequeue::pop(filequeue& queue, size_t wait_ms)
{
    item::ptr p_item = std::make_unique<item>(m_p_logger);

    if (! m_queue.pop_item(p_item->m_entry, queue.m_queue, wait_ms)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

filequeue::item::ptr filequeue::pop(sink_filequeue& queue, size_t wait_ms)
{
    item::ptr p_item = std::make_unique<item>(m_p_logger);

    if (! m_queue.pop_item(p_item->m_entry, queue.m_queue, wait_ms)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

bool filequeue::push(const item::ptr& p_item)
{
    assert(p_item);

    const bool rc = m_queue.push_item(p_item->m_entry);
    if (rc) {
        assert(p_item->m_entry.is_alive());
    }
    return rc;
}

} // namespace gut
} // namespace wstux
