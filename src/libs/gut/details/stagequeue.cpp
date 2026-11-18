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
#include <utility>

#include "gut/stagequeue.h"
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

stagequeue::stagequeue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_logger(std::move(p_logger))
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_input(make_queue_path(path, "input"), m_p_cleaner, m_p_logger, m_is_stop_fn)
    , m_proc(make_queue_path(path, "proc"), m_p_logger, m_is_stop_fn)
    , m_output(make_queue_path(path, "output"), m_p_logger, m_is_stop_fn)
{
    static_assert(std::is_same<details::fq::cleaner_ptr_t, details::fq::cleaner::ptr>::value);
}

stagequeue::stagequeue(const std::string_view& path, const std::string_view& out_path,
                       const std::function<bool()>& is_stop_fn, logging_handler::ptr&& p_logger)
    : m_path(path)
    , m_is_stop_fn(is_stop_fn)
    , m_p_logger(std::move(p_logger))
    , m_p_cleaner(std::make_shared<details::fq::cleaner>(m_is_stop_fn))
    , m_input(make_queue_path(path, "input"), m_p_cleaner, m_p_logger, m_is_stop_fn)
    , m_proc(make_queue_path(path, "proc"), m_p_logger, m_is_stop_fn)
    , m_output(make_queue_path(out_path, "input"), m_p_logger, m_is_stop_fn)
{
    static_assert(std::is_same<details::fq::cleaner_ptr_t, details::fq::cleaner::ptr>::value);
}

/**
 *  \details    Removes abandoned empty rotation directories across 'input', 'output' and
 *      purges unlocked stale files in 'proc', and deletes any malformed entries whose
 *      names do not conform to internal naming rules.
 */
bool stagequeue::cleanup_garbage(int secs_offset, const std::string_view& path,
                                 const std::function<bool()>& is_stop_fn, const std::string_view& out_path)
{
    bool is_ok = true;
    is_ok &= details::fq::cleaner::cleanup(make_queue_path(path, "input"), 0, secs_offset, false, is_stop_fn);
    is_ok &= details::fq::cleaner::cleanup(make_queue_path(path, "proc"), 0, secs_offset, true, is_stop_fn);
    if (! out_path.empty()) {
        is_ok &= details::fq::cleaner::cleanup(make_queue_path(out_path, "input"), 0, secs_offset, false, is_stop_fn);
    } else {
        is_ok &= details::fq::cleaner::cleanup(make_queue_path(path, "output"), 0, secs_offset, false, is_stop_fn);
    }
    return is_ok;
}

/**
 *  \details    The newly created file is exclusively locked and invisible to consumers
 *      until item::commit() or item::rollback() is called. If an item instance is
 *      destroyed without an explicit call to the item::commit() or item::rollback(),
 *      it will remain in the 'proc' directory.
 */
stagequeue::item::ptr stagequeue::create()
{
    item::ptr p_item = std::make_unique<item>(m_output, m_input, std::as_const(m_p_logger));

    if (! m_proc.create_item(p_item->m_entry)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

/**
 *  \details    Creates a directory structure ('input', 'proc' and 'output' subdirectories)
 *      at the specified path, configures access permissions, and opens them.
 */
stagequeue::ptr stagequeue::create_queue(const std::string_view& path, const std::function<bool()>& is_stop_fn, logging_handler::ptr p_logger)
{
    if (path.empty()) {
        GUT_LOG_ERROR(p_logger, "Stage queue directory path is empty");
        return nullptr;
    }

    if (! create_queue_dir(path, p_logger)) {
        return nullptr;
    }
    stagequeue::ptr p_queue = std::make_unique<stagequeue>(path, is_stop_fn, std::move(p_logger));
    if (! p_queue->open()) {
        GUT_LOG_ERROR(p_queue->m_p_logger, "Failed to open stage queue directory %s", p_queue->m_path.c_str());
        return nullptr;
    }
    GUT_LOG_DEBUG(p_queue->m_p_logger, "Inited stage queue in directory %s", p_queue->m_path.c_str());
    return p_queue;
}

/**
 *  \details    Creates a directory structure ('input', 'proc' and 'output' subdirectories)
 *      at the specified path, configures access permissions, and opens them.
 *
 *      Instead of a local output directory, it uses the input directory of a queue located
 *      at the provided path. Calling item::commit() on items in this queue will deliver
 *      them directly into the input pool of the target queue.
 */
stagequeue::ptr stagequeue::create_queue(const std::string_view& path, const std::string_view& out_path,
                                         const std::function<bool()>& is_stop_fn, logging_handler::ptr p_logger)
{
    namespace fs = std::filesystem;

    if (path.empty() || out_path.empty()) {
        GUT_LOG_ERROR(p_logger, "Stage queue directory path or output directory path is empty");
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

    stagequeue::ptr p_queue = std::make_unique<stagequeue>(path, out_path, is_stop_fn, std::move(p_logger));
    if (! p_queue->open()) {
        GUT_LOG_ERROR(p_queue->m_p_logger, "Failed to open stage queue directory %s", p_queue->m_path.c_str());
        return nullptr;
    }
    GUT_LOG_DEBUG(p_queue->m_p_logger, "Inited stage queue in directory %s", p_queue->m_path.c_str());
    return p_queue;
}

bool stagequeue::open()
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

/**
 *  \details    Atomically moves a file from input directory to the working directory
 *      and acquires an exclusive lock on the file. If the incoming queue is empty, the
 *      current thread blocks until a new file appears or a timeout expires.
 *
 * \warning     The caller **must** explicitly call item::commit() or item::rollback()
 *      when done. If an item instance is destroyed without an explicit call to the
 *      item::commit() or item::rollback(), it will remain in the 'proc' directory.
 */
stagequeue::item::ptr stagequeue::pop(size_t wait_ms)
{
    item::ptr p_item = std::make_unique<item>(m_output, m_input, std::as_const(m_p_logger));

    if (! m_input.pop_item(p_item->m_entry, m_proc, wait_ms)) {
        assert(! p_item->m_entry.is_alive());
        p_item.reset();
    } else {
        assert(p_item->m_entry.is_alive());
    }
    return p_item;
}

/**
 *  \details    Scans the proc directory for unlocked items left over after a crash or
 *      improper shutdown. Each found item is passed to rollback_fn. If the predicate
 *      returns true, the item is moved back to the input directory. Otherwise, it is
 *      moved to the dead directory (created on demand) for further inspection and
 *      analysis.
 */
bool stagequeue::rollback(const std::string_view& path, const std::function<bool(item::ptr&)>& rollback_fn,
                          const logging_handler::ptr& p_logger, const std::function<bool()>& is_stop_fn)
{
    if (path.empty()) {
        GUT_LOG_ERROR(p_logger, "Stage queue directory path is empty");
        return false;
    }

    details::fq::cleaner::ptr p_cleaner = std::make_shared<details::fq::cleaner>(is_stop_fn);

    details::fq::sink_queue input(make_queue_path(path, "input"), p_logger, is_stop_fn);
    details::fq::queue dead(make_queue_path(path, "dead"), p_cleaner, p_logger, is_stop_fn);
    details::fq::queue proc(make_queue_path(path, "proc"), p_cleaner, p_logger, is_stop_fn);

    if (! input.open() || ! proc.open() || ! dead.open()) {
        GUT_LOG_ERROR(p_logger, "Failed to open input, proc or dead directory");
        return false;
    }

    bool all_rolled_back = true;
    while (! is_stop_fn()) {
        item::ptr p_item = std::make_unique<item>(input, dead, p_logger);

        if (! proc.pop_item(p_item->m_entry, proc, 0)) {
            break;
        }

        if (rollback_fn(p_item)) {
            if (! p_item->commit()) {
                GUT_LOG_ERROR(p_logger, "Failed to move item %s to input directory", p_item->m_entry.name().data());
                all_rolled_back = false;
            }
        } else {
            if (! p_item->rollback()) {
                GUT_LOG_ERROR(p_logger, "Failed to move item %s to dead directory", p_item->m_entry.name().data());
            }
            all_rolled_back = false;
        }
    }

    if (is_stop_fn()) {
        return false;
    }

    p_cleaner->cleanup(0, 0);
    return all_rolled_back;
}

} // namespace gut
} // namespace wstux
