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

#include <type_traits>

#include "gut/details/dir.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/filequeue/tail.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

class active_tier final
{
public:
    using ptr = std::unique_ptr<active_tier>;

public:
    active_tier(const dir_t& root_dir, const tmp_dir::ptr& p_parent_dir, const tmp_dir::ptr& p_dir, notifier* p_notifier)
        : m_p_dir(p_dir)
        , m_p_notifier(p_notifier)
    {
        assert(m_p_dir && m_p_dir->is_open());

        char path[PATH_MAX];
        [[maybe_unused]] const int len = ::snprintf(path, sizeof(path), "%.*s/%.*s/%.*s",
            static_cast<int>(root_dir.path().size()), root_dir.path().data(),
            static_cast<int>(p_parent_dir->name().size()), p_parent_dir->name().data(),
            static_cast<int>(m_p_dir->name().size()), m_p_dir->name().data());
        assert(len > 0 && len < static_cast<int>(sizeof(path)));

        m_wd = m_p_notifier->add_watch(path, IN_CREATE | IN_MOVED_TO,
            [this](uint32_t mask, const std::string_view& name) {
                if (name.empty()) {
                    m_files.clear();
                    reload();
                } else if (mask & (IN_CREATE | IN_MOVED_TO)) {
                    m_files.emplace(name);
                }
            });
        reload();
    }

    ~active_tier()
    {
        if (m_wd >= 0 && m_p_notifier) {
            m_p_notifier->remove_watch(m_wd);
            m_wd = -1;
        }
    }

    active_tier(const active_tier&) = delete;
    active_tier& operator=(const active_tier&) = delete;
    active_tier(active_tier&&) noexcept = default;
    active_tier& operator=(active_tier&&) noexcept = default;

    const tmp_dir::ptr& dir() const { return m_p_dir; }

    bool empty() const { return m_files.empty(); }

    void pop()
    {
        if (! m_files.empty()) {
            m_files.erase(m_files.begin());
        }
    }

    const std::string& top() const { return *m_files.begin(); }

private:
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
                }
            }
            scanner.close();
        }
    }

private:
    tmp_dir::ptr m_p_dir;
    notifier* m_p_notifier;
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

template<typename TParent>
bool reload_dir(const TParent& parent, std::set<tmp_dir::ptr>& pool, const std::string cur_name = std::string())
{
    assert(pool.empty());

    dir_reader scanner;
    if (! scanner.opendir(parent)) {
        return false;
    }

    while (struct ::dirent* p_ent = scanner.readdir()) {
        if (p_ent->d_type == DT_DIR) {
            const std::string_view dir_name = p_ent->d_name;
            if (dir_name == "." || dir_name == "..") {
                continue;
            }
            if (! cur_name.empty() && dir_name <= cur_name) {
                continue;
            }
            assert(dir_name.size() == gk_name_length);
            pool.emplace(std::make_shared<tmp_dir>(dir_name, parent));
        }
    }
    scanner.close();
    return true;
}

} // <anonymous> namespace

////////////////////////////////////////////////////////////////////////////////
// class tail::primary_tier

class tail::primary_tier final
{
public:
    primary_tier(const dir_t& parent_dir, const tmp_dir::ptr& p_dir, notifier* p_notifier)
        : m_parent_dir(parent_dir)
        , m_p_dir(p_dir)
        , m_p_notifier(p_notifier)
    {
        assert(m_p_dir && m_p_dir->is_open());

        char path[PATH_MAX];
        [[maybe_unused]] const int len = ::snprintf(path, sizeof(path), "%.*s/%.*s",
            static_cast<int>(parent_dir.path().size()), parent_dir.path().data(),
            static_cast<int>(m_p_dir->name().size()), m_p_dir->name().data());
        assert(len > 0 && len < static_cast<int>(sizeof(path)));

        m_wd = m_p_notifier->add_watch(path, IN_CREATE | IN_MOVED_TO,
            [this](uint32_t mask, const std::string_view& name) {
                if (name.empty()) {
                    m_active_pool.clear();
                    reload();
                } else if (mask & (IN_CREATE | IN_MOVED_TO)) {
                    m_active_pool.emplace(name);
                }
            });
        reload();
    }

    ~primary_tier()
    {
        if (m_wd >= 0 && m_p_notifier) {
            m_p_notifier->remove_watch(m_wd);
            m_wd = -1;
        }
    }

    const tmp_dir::ptr& dir() const { return m_p_dir; }

    bool empty() const { return m_active_pool.empty() && m_active_tiers.empty(); }

    bool get(std::string& filename, tmp_dir::ptr& p_dir, const cleaner::ptr& p_cleaner)
    {
        constexpr size_t k_tiers_limit = 16;
        while (m_active_tiers.size() < k_tiers_limit && ! m_active_pool.empty()) {
            const std::string active_dir_name = *m_active_pool.begin();
            m_active_pool.erase(m_active_pool.begin());

            tmp_dir::ptr p_active_dir = std::make_shared<tmp_dir>(active_dir_name, m_p_dir);
            if (p_active_dir->open() && p_active_dir->lock(false)) {
                active_tier::ptr p_child = std::make_unique<active_tier>(m_parent_dir, m_p_dir, p_active_dir, m_p_notifier);
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
                }
            }
            scanner.close();
        }
    }

private:
    const dir_t& m_parent_dir;
    tmp_dir::ptr m_p_dir;
    notifier* m_p_notifier;
    int m_wd = -1;
    std::set<std::string> m_active_pool;
    std::map<std::string, active_tier::ptr> m_active_tiers;
};

////////////////////////////////////////////////////////////////////////////////
// class tail

tail::tail(const dir_t& root_dir, const cleaner::ptr& p_cleaner, const is_stop_fn_t& is_stop_fn)
    : m_root_dir(root_dir)
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

bool tail::get(std::string& filename, tmp_dir::ptr& p_dir, size_t wait_ms)
{
    assert(m_root_dir.is_open());
    if (! m_root_dir.is_open()) {
        return false;
    }

    std::unique_lock<std::mutex> lock(m_tail_mutex);

    m_notifier.fetch(0);
    while (! m_is_stop_fn()) {
        while (m_primary_tiers.size() < 4 && ! m_primary_pool.empty()) {
            std::string primary_name = *m_primary_pool.begin();
            m_primary_pool.erase(m_primary_pool.begin());

            tmp_dir::ptr p_primary_dir = std::make_shared<tmp_dir>(primary_name, m_root_dir.fd());
            if (p_primary_dir->open() && p_primary_dir->lock(false)) {
                m_primary_tiers.emplace(primary_name, std::make_shared<primary_tier>(m_root_dir, p_primary_dir, &m_notifier));
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
    if (scanner.opendir(m_root_dir.fd())) {
        while (struct ::dirent* p_ent = scanner.readdir()) {
            if (p_ent->d_type == DT_DIR) {
                std::string_view name = p_ent->d_name;
                if (name != "." && name != "..") {
                    m_primary_pool.emplace(name);
                }
            }
        }
        scanner.close();
        return true;
    }
    return false;
}

bool tail::start()
{
    m_root_wd = m_notifier.add_watch(m_root_dir.path(), IN_CREATE | IN_MOVED_TO,
        [this](uint32_t mask, const std::string_view& name) {
            if (name.empty()) {
                m_primary_pool.clear();
                reload();
            } else if (mask & (IN_CREATE | IN_MOVED_TO)) {
                m_primary_pool.emplace(name);
            }
        });
    if (m_root_wd < 0) {
        return false;
    }
    return reload();
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
