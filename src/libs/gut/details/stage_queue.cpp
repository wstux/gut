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

#include "gut/stage_queue.h"
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
        GUT_LOG_ERROR(p_logger, "Failed to create stage queue directory %s", path.c_str());
        return false;
    }
    fs::perms perms = fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read;
    fs::permissions(path, perms, error);
    if (error) {
        GUT_LOG_ERROR(p_logger, "Failed to set permissions for stage queue directory %s", path.c_str());
        return false;
    }
    return true;
}

std::string make_queue_path(const std::string_view& root_path, const std::string_view& queue_name)
{
    assert(root_path.size() > 0);

    std::filesystem::path path = root_path;
    path = path / queue_name;
    return path.string();
}

} // <anonymous> namespace

stage_queue::stage_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_input(make_queue_path(path, "input"), m_p_cleaner, m_is_stop_fn)
    , m_proc(make_queue_path(path, "proc"), m_is_stop_fn)
    , m_output(make_queue_path(path, "output"), m_is_stop_fn)
    , m_p_logger(std::move(p_logger))
{
    static_assert(std::is_same<details::fq::cleaner_ptr_t, details::fq::cleaner::ptr>::value);
}

stage_queue::stage_queue(const std::string_view& path, const std::string_view& out_path,
                         const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_input(make_queue_path(path, "input"), m_p_cleaner, m_is_stop_fn)
    , m_proc(make_queue_path(path, "proc"), m_is_stop_fn)
    , m_output(make_queue_path(out_path, "input"), m_is_stop_fn)
    , m_p_logger(std::move(p_logger))
{
    static_assert(std::is_same<details::fq::cleaner_ptr_t, details::fq::cleaner::ptr>::value);
}

stage_queue::item::ptr stage_queue::create()
{
    item::ptr p_item = std::make_unique<item>(m_output, m_input);

    if (! m_proc.create_item(p_item->m_entry)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

stage_queue::ptr stage_queue::create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr p_logger)
{
    if (path.empty()) {
        GUT_LOG_ERROR(p_logger, "Stage queue directory is empty");
        return nullptr;
    }

    if (! create_queue_dir(path, p_logger)) {
        return nullptr;
    }
    stage_queue::ptr p_queue = std::make_unique<stage_queue>(path, is_stop_fn, std::move(p_logger));
    if (! p_queue->open()) {
        GUT_LOG_ERROR(p_queue->m_p_logger, "Failed to open stage queue directory %s", p_queue->m_path.c_str());
        return nullptr;
    }
    GUT_LOG_DEBUG(p_queue->m_p_logger, "Inited stage queue int directory %s", p_queue->m_path.c_str());
    return p_queue;
}

stage_queue::ptr stage_queue::create_queue(const std::string_view& path, const std::string_view& out_path,
                                           const std::function<bool()>& is_stop_fn, logging_handler::ptr p_logger)
{
    namespace fs = std::filesystem;

    if (path.empty() || out_path.empty()) {
        GUT_LOG_ERROR(p_logger, "Stage queue directory or output directory is empty");
        return nullptr;
    }

    if (! create_queue_dir(path, p_logger)) {
        return nullptr;
    }

    // Create output dirs
    std::error_code error;
    fs::permissions(out_path, fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read, error);
    if (error) {
        GUT_LOG_ERROR(p_logger, "Failed to set permissions for stage queue output directory %s", path.data());
        return nullptr;
    }
    if (! create_queue_dir(fs::path(out_path) / "input", p_logger)) {
        return nullptr;
    }

    stage_queue::ptr p_queue = std::make_unique<stage_queue>(path, out_path, is_stop_fn, std::move(p_logger));
    if (! p_queue->open()) {
        GUT_LOG_ERROR(p_queue->m_p_logger, "Failed to open stage queue directory %s", p_queue->m_path.c_str());
        return nullptr;
    }
    GUT_LOG_DEBUG(p_queue->m_p_logger, "Inited stage queue int directory %s", p_queue->m_path.c_str());
    return p_queue;
}

bool stage_queue::open()
{
    if (! m_input.open()) {
        GUT_LOG_ERROR(m_p_logger, "Failed to open input directory");
        return false;
    }
    if (! m_proc.open()) {
        GUT_LOG_ERROR(m_p_logger, "Failed to open processing directory");
        return false;
    }
    if (! m_output.open()) {
        GUT_LOG_ERROR(m_p_logger, "Failed to open output directory");
        return false;
    }
    return true;
}

stage_queue::item::ptr stage_queue::pop(size_t wait_ms)
{
    item::ptr p_item = std::make_unique<item>(m_output, m_input);

    if (! m_input.pop_item(p_item->m_entry, m_proc, wait_ms)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

} // namespace gut
} // namespace wstux
