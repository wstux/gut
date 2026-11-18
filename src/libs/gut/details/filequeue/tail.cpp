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

#include <linux/limits.h> // for PATH_MAX
#include <sys/inotify.h>

#include <time.h>
#include <cassert>
#include <type_traits>

#include "gut/details/logging.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/filequeue/tail.h"
#include "gut/details/utils/tmp_dir.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

/**
 *  \brief  Leaf-level directory watcher and sorted file cache for a single active subdirectory.
 *
 *  \details    Represents the lowest directory level (active directories) in the two-tier
 *      rotation hierarchy. Monitors incoming regular files via ::inotify (IN_CREATE | IN_MOVED_TO)
 *      and maintains an in-memory lexicographically sorted set of available item filenames,
 *      eliminating repeated directory scans during consumption.
 *
 *  \warning    Class instances are **not thread-safe** and are synchronized externally
 *      by mutex. Also note that move operations are deleted/unsafe if watch desctriptor
 *      opened because the ::inotify callback captures this.
 */
class active_tier final
{
public:
    using ptr = std::unique_ptr<active_tier>;

public:
    /// \brief  Constructs an active tier watcher and loads all existing files from nook directory.
    /// \param  nook_dir - the nook directory.
    /// \param  p_parent_dir - the enclosing primary directory.
    /// \param  p_dir - the opened active directory to monitor.
    /// \param  notifier - the shared ::inotify dispatcher.
    /// \param  p_logger - a pointer to a logging handler.
    active_tier(const directory& nook_dir, const tmp_dir::ptr& p_parent_dir, const tmp_dir::ptr& p_dir,
                notifier& notifier, const logging_handler::ptr& p_logger)
        : m_p_dir(p_dir)
        , m_p_logger(p_logger)
        , m_notifier(notifier)
    {
        assert(m_p_dir && m_p_dir->is_open());

        char path[PATH_MAX];
        [[maybe_unused]] const int len = ::snprintf(path, sizeof(path), "%.*s/%.*s/%.*s",
            static_cast<int>(nook_dir.path().size()), nook_dir.path().data(),
            static_cast<int>(p_parent_dir->name().size()), p_parent_dir->name().data(),
            static_cast<int>(m_p_dir->name().size()), m_p_dir->name().data());
        assert(len > 0 && len < static_cast<int>(sizeof(path)));

        m_wd = m_notifier.add_watch(path, IN_CREATE | IN_MOVED_TO,
            [this](uint32_t mask, const std::string_view& name) {
                if (name.empty()) {
                    m_files.clear();
                    reload();
                } else if (mask & (IN_CREATE | IN_MOVED_TO)) {
                    m_files.emplace(name);
                }
            });
        if (m_wd < 0) {
            GUT_LOG_WARN(m_p_logger, "Failed to add notify to active directory '%s'. Errno %d", path, errno);
        }
        reload();
    }

    /// \brief  Unregisters the ::inotify watch descriptor if active.
    ~active_tier()
    {
        if (m_wd >= 0) {
            m_notifier.remove_watch(m_wd);
            m_wd = -1;
        }
    }

    active_tier(const active_tier&) = delete;
    active_tier& operator=(const active_tier&) = delete;
    active_tier(active_tier&&) noexcept = delete;
    active_tier& operator=(active_tier&&) noexcept = delete;

    /// \brief  Returns the underlying opened directory handle.
    /// \return Current active directory.
    const tmp_dir::ptr& dir() const { return m_p_dir; }

    /// \brief  Checks whether any unconsumed filenames remain in the in-memory cache.
    /// \return true if the in-memory cache is empty; false otherwise.
    bool empty() const { return m_files.empty(); }

    /// \brief  Removes the oldest (lexicographically smallest) filename from the cache.
    void pop()
    {
        if (! m_files.empty()) {
            m_files.erase(m_files.begin());
        }
    }

    /// \brief  Returns the oldest available filename in this directory without removing it.
    /// \return Reference to the first filename in the in-memory cache.
    const std::string& top() const { return *m_files.begin(); }

private:
    /// \brief Scans active directory and populates in-memory cache with all regular filenames.
    void reload()
    {
        dir_reader scanner;
        if (scanner.opendir(m_p_dir->fd())) {
            while (struct ::dirent* p_ent = scanner.readdir()) {
                if (p_ent->d_type == DT_REG) {
                    std::string_view name = p_ent->d_name;
                    if (name != "." && name != "..") {
                        m_files.emplace(name);
                    }
                } else {
                    GUT_LOG_WARN(m_p_logger, "Unexpected file '%s' in active directory '%s'", p_ent->d_name, m_p_dir->name().data());
                }
            }
            scanner.close();
        }
    }

private:
    tmp_dir::ptr m_p_dir;
    const logging_handler::ptr& m_p_logger;
    notifier& m_notifier;
    int m_wd = -1;
    std::set<std::string> m_files;
};

/*size_t monotime_ms()
{
    struct ::timespec tv;
    if (::clock_gettime(CLOCK_MONOTONIC, &tv) == 0) {
        return static_cast<size_t>(tv.tv_sec) * 1000 + static_cast<size_t>(tv.tv_nsec) / 1000000;
    }

    return static_cast<size_t>(::time(nullptr)) * 1000;
}*/

} // <anonymous> namespace

////////////////////////////////////////////////////////////////////////////////
// class tail::primary_tier

/**
 *  \brief  Mid-level directory manager performing K-way merge across bounded active
 *      subdirectories.
 *
 *  \details    Represents a first-level rotation directory. Watches for newly created
 *      active subdirectories via ::inotify, stores all discovered subdirectory names in
 *      the in-memory cache, and keeps at most limit (16) active instances open
 *      simultaneously in a sliding window to conserve file descriptors.
 *
 *      On each get() call, merges the heads of all open active tier instances in
 *      chronological order and evicts exhausted directories to cleaner.
 */
class tail::primary_tier final
{
public:
    /// \brief  Constructs a primary tier manager and scans existing active subdirectories.
    /// \param  parent_dir - reference to the parent directory.
    /// \param  p_dir - the opened primary directory.
    /// \param  notifier - the shared ::inotify dispatcher.
    /// \param  p_logger - a pointer to a logging handler.
    primary_tier(const directory& parent_dir, const tmp_dir::ptr& p_dir, notifier& notifier, const logging_handler::ptr& p_logger)
        : m_parent_dir(parent_dir)
        , m_p_logger(p_logger)
        , m_p_dir(p_dir)
        , m_notifier(notifier)
    {
        assert(m_p_dir && m_p_dir->is_open());

        char path[PATH_MAX];
        [[maybe_unused]] const int len = ::snprintf(path, sizeof(path), "%.*s/%.*s",
            static_cast<int>(parent_dir.path().size()), parent_dir.path().data(),
            static_cast<int>(m_p_dir->name().size()), m_p_dir->name().data());
        assert(len > 0 && len < static_cast<int>(sizeof(path)));

        m_wd = m_notifier.add_watch(path, IN_CREATE | IN_MOVED_TO,
            [this](uint32_t mask, const std::string_view& name) {
                if (name.empty()) {
                    m_active_pool.clear();
                    reload();
                } else if (mask & (IN_CREATE | IN_MOVED_TO)) {
                    m_active_pool.emplace(name);
                }
            });
        if (m_wd < 0) {
            GUT_LOG_WARN(m_p_logger, "Failed to add notify to primary directory '%s'. Errno %d", path, errno);
        }
        reload();
    }

    /// \brief  Unregisters the ::inotify watch descriptor and destroys all managed active instances.
    ~primary_tier()
    {
        if (m_wd >= 0) {
            m_notifier.remove_watch(m_wd);
            m_wd = -1;
        }
    }

    primary_tier(const primary_tier&) = delete;
    primary_tier& operator=(const primary_tier&) = delete;
    primary_tier(primary_tier&&) noexcept = delete;
    primary_tier& operator=(primary_tier&&) noexcept = delete;

    /// \brief  Returns the underlying opened primary directory handle.
    /// \return Current primary directory.
    const tmp_dir::ptr& dir() const { return m_p_dir; }

    /// \brief  Checks whether both the pending subdirectory pool and all active tiers are empty.
    /// \return true if no subdirectories or files remain in this primary tier; false otherwise.
    bool empty() const { return m_active_pool.empty() && m_active_tiers.empty(); }

    /// \brief  Retrieves the oldest available item across all managed active subdirectories.
    /// \param  filename - output string populated with the selected item filename.
    /// \param  p_dir - output pointer populated with the open active directory containing file.
    /// \param  p_cleaner - shared cleaner instance for removing exhausted rotation directories.
    /// \return true if an item was retrieved; false if all active tiers are currently empty.
    /// \details Refills the active tiers sliding window from pool up to limit, performs a K-way
    ///     merge across top() filenames of open tiers, pops the winning entry, and evicts any
    ///     exhausted active directory to cleaner if not locked by a writer.
    bool get(std::string& filename, tmp_dir::ptr& p_dir, const cleaner::ptr& p_cleaner)
    {
        constexpr size_t k_tiers_limit = 16;
        while (m_active_tiers.size() < k_tiers_limit && ! m_active_pool.empty()) {
            const std::string active_dir_name = *m_active_pool.begin();
            m_active_pool.erase(m_active_pool.begin());

            tmp_dir::ptr p_active_dir = std::make_shared<tmp_dir>(active_dir_name, m_p_dir);
            if (p_active_dir->open() && p_active_dir->lock(false)) {
                active_tier::ptr p_child = std::make_unique<active_tier>(m_parent_dir, m_p_dir, p_active_dir, m_notifier, m_p_logger);
                if (! p_child->empty() || ! p_active_dir->lock(true)) {
                    m_active_tiers.emplace(active_dir_name, std::move(p_child));
                } else {
                    //if (p_active_dir->lock(true)) {
                    p_cleaner->push(p_active_dir);
                    //}
                }
            }
        }

        // K-way Merge
        std::map<std::string, active_tier::ptr>::iterator cur_it = m_active_tiers.end();
        for (std::map<std::string, active_tier::ptr>::iterator it = m_active_tiers.begin(); it != m_active_tiers.end(); ++it) {
            //if (! it->second.empty() && ((cur_it == m_active_tiers.end()) || it->second.top() < cur_it->first)) {
            if (! it->second->empty() && ((cur_it == m_active_tiers.end()) || it->second->top() < cur_it->second->top())) {
                cur_it = it;
            }
        }

        if (cur_it != m_active_tiers.end()) {
            filename = cur_it->second->top();
            p_dir = cur_it->second->dir();
            cur_it->second->pop();

            if (cur_it->second->empty()) {
                if (cur_it->second->dir()->lock(true)) {
                    p_cleaner->push(cur_it->second->dir());
                    m_active_tiers.erase(cur_it);
                }
            }
            return true;
        }
        return false;
    }

private:
    /// \brief  Scans current primary directory and populates in-memory cache with all subdirectory names.
    void reload()
    {
        dir_reader scanner;
        if (scanner.opendir(m_p_dir->fd())) {
            while (struct ::dirent* p_ent = scanner.readdir()) {
                if (p_ent->d_type == DT_DIR) {
                    std::string_view name = p_ent->d_name;
                    if (name != "." && name != "..") {
                        m_active_pool.emplace(name);
                    }
                } else {
                    GUT_LOG_WARN(m_p_logger, "Unexpected file '%s' in primary directory '%s'", p_ent->d_name, m_p_dir->name().data());
                }
            }
            scanner.close();
        }
    }

private:
    const directory& m_parent_dir;
    const logging_handler::ptr& m_p_logger;
    tmp_dir::ptr m_p_dir;
    notifier& m_notifier;
    int m_wd = -1;
    std::set<std::string> m_active_pool;
    std::map<std::string, active_tier::ptr> m_active_tiers;
};

////////////////////////////////////////////////////////////////////////////////
// class tail

tail::tail(const directory& nook_dir, const cleaner::ptr& p_cleaner, const logging_handler::ptr& p_logger,
           const std::function<bool()>& is_stop_fn)
    : m_nook_dir(nook_dir)
    , m_p_logger(p_logger)
    , m_is_stop_fn(is_stop_fn)
    , m_p_cleaner(p_cleaner)
{
    static_assert(std::is_same<cleaner_ptr_t, cleaner::ptr>::value);
    static_assert(std::is_same<tmp_dir_ptr_t, tmp_dir::ptr>::value);
}

tail::~tail()
{
    if (m_root_wd >= 0) {
        m_notifier.remove_watch(m_root_wd);
    }
}

/**
 *  \details    Queries active primary tiers in chronological order. If no items are
 *      currently cached, drains pending ::inotify events or sleeps up to wait_ms
 *      milliseconds waiting for new files or subdirectories to appear. Exhausted tiers
 *      are evicted to m_p_cleaner and replaced from m_primary_pool.
 */
bool tail::get(std::string& filename, tmp_dir::ptr& p_dir, size_t wait_ms)
{
    assert(m_nook_dir.is_open());
    if (! m_nook_dir.is_open()) {
        return false;
    }

    std::unique_lock<std::mutex> lock(m_tail_mutex);

    m_notifier.fetch(0);
    while (! m_is_stop_fn()) {
        while (m_primary_tiers.size() < 4 && ! m_primary_pool.empty()) {
            std::string primary_name = *m_primary_pool.begin();
            m_primary_pool.erase(m_primary_pool.begin());

            tmp_dir::ptr p_primary_dir = std::make_shared<tmp_dir>(primary_name, m_nook_dir.fd());
            if (p_primary_dir->open() && p_primary_dir->lock(false)) {
                m_primary_tiers.emplace(primary_name, std::make_shared<primary_tier>(m_nook_dir, p_primary_dir, m_notifier, m_p_logger));
            }
        }

        // K-way Merge
        for (std::map<std::string, primary_tier_ptr_t>::iterator it = m_primary_tiers.begin(); it != m_primary_tiers.end();) {
            if (it->second->get(filename, p_dir, m_p_cleaner)) {
                return true;
            }

            if (it->second->empty()) {
                const tmp_dir::ptr& target_dir = it->second->dir();
                if (target_dir->lock(true)) {
                    m_p_cleaner->push(target_dir);
                    it = m_primary_tiers.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }

        if (wait_ms > 0) {
            lock.unlock();
            const bool rc = m_notifier.wait(wait_ms);
            lock.lock();

            if (rc) {
                m_notifier.fetch();
            }

            wait_ms = 0;
            continue;
        }

        return false;
    }
    return false;
}

bool tail::reload()
{
    dir_reader scanner;
    if (scanner.opendir(m_nook_dir.fd())) {
        while (struct ::dirent* p_ent = scanner.readdir()) {
            if (p_ent->d_type == DT_DIR) {
                std::string_view name = p_ent->d_name;
                if (name != "." && name != "..") {
                    m_primary_pool.emplace(name);
                }
            } else {
                GUT_LOG_WARN(m_p_logger, "Unexpected file '%s' in nook directory '%s'", p_ent->d_name, m_nook_dir.path().data());
            }
        }
        scanner.close();
        return true;
    }
    GUT_LOG_ERROR(m_p_logger, "Failed to opendir '%s'", m_nook_dir.path().data());
    return false;
}

/**
 *  \details Registers a watch on m_nook_dir for subdirectory creation/moves and populates
 *      the initial m_primary_pool and active m_primary_tiers sliding window.
 */
bool tail::start()
{
    assert(m_root_wd == -1);

    m_root_wd = m_notifier.add_watch(m_nook_dir.path(), IN_CREATE | IN_MOVED_TO,
        [this](uint32_t mask, const std::string_view& name) {
            if (name.empty()) {
                m_primary_pool.clear();
                reload();
            } else if (mask & (IN_CREATE | IN_MOVED_TO)) {
                m_primary_pool.emplace(name);
            }
        });
    if (m_root_wd < 0) {
        GUT_LOG_WARN(m_p_logger, "Failed to add notify to directory '%s'. Errno %d", m_nook_dir.path().data(), errno);
        return false;
    }
    return reload();
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
