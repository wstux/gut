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

#include <type_traits>

#include "gut/details/dir.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/filequeue/tail.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {
namespace {

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

tail::tail(const dir_t& root_dir, const cleaner::ptr& p_cleaner, const is_stop_fn_t& is_stop_fn)
    : m_root_dir(root_dir)
    , m_is_stop_fn(is_stop_fn)
    , m_p_cleaner(p_cleaner)
{
    static_assert(std::is_same<cleaner_ptr_t, cleaner::ptr>::value);
    static_assert(std::is_same<tmp_dir_ptr_t, tmp_dir::ptr>::value);
}

bool tail::get(std::string& filename, tmp_dir::ptr& p_dir, size_t /*wait_ms*/)
{
    assert(m_root_dir.is_open());
    if (! m_root_dir.is_open()) {
        return false;
    }

    filename.clear();
    p_dir.reset();

    //const size_t now_ms = monotime_ms() + wait_ms;
    while (! m_is_stop_fn()) { // && ((now_ms + wait_ms) > monotime_ms())) {
        if (! m_files_pool.empty()) {
            //assert(m_p_primary_dir && m_p_active_dir);
            //assert(m_p_primary_dir < m_p_active_dir);

            filename = *m_files_pool.begin();
            m_files_pool.erase(m_files_pool.begin());
            p_dir = m_p_active_dir;
            return true;
        }

        //if (m_p_active_dir && reload_dir(m_p_active_dir->fd(), DT_REG, m_files_pool)) {
        //    continue;
        //}

        if (reload()) {
            continue;
        }

        /*if (change_active_dir()) {
            continue;
        }
        if (change_primary_dir()) {
            continue;
        }*/

        return false;
    }
    return false;
}

bool tail::load_files()
{
    if (! m_p_active_dir) {
        return false;
    }

    assert(utils::is_open(m_p_active_dir->fd()));
    assert(m_files_pool.empty());

    dir_reader scanner;
    if (! scanner.opendir(m_p_active_dir)) {
        return false;
    }

    while (struct ::dirent* p_ent = scanner.readdir()) {
        if (p_ent->d_type == DT_REG) {
            const std::string_view file_name = p_ent->d_name;
            if (file_name == "." || file_name == "..") {
                continue;
            }
            assert(file_name.size() == gk_filename_length);
            m_files_pool.emplace(file_name);
        }
    }
    scanner.close();
    return (! m_files_pool.empty());
}

/*bool tail::load_primary_dirs()
{
    dir_reader root_scanner;
    if (! root_scanner.opendir(m_root_dir.fd())) {
        return false;
    }

    while (struct ::dirent* p_ent = root_scanner.readdir()) {
        if (p_ent->d_type == DT_DIR) {
            const std::string_view dir_name = p_ent->d_name;
            if (dir_name == "." || dir_name == "..") {
                continue;
            }
            assert(dir_name.size() == gk_name_length);
            m_primary_pool.emplace(dir_name);
        }
    }
    root_scanner.close();
    return (! m_primary_pool.empty());
}*/

bool tail::reload()
{
    while (true) {
        if (load_files()) {
            return true;
        }
        if (m_active_pool.empty()) {
            if (reload_active_pool()) {
                continue;
            }
        } else {
            m_p_active_dir = *m_active_pool.begin();
            m_active_pool.erase(m_active_pool.begin());
            continue;
        }
        if (m_primary_pool.empty()) {
            if (reload_dir(m_root_dir.fd(), m_primary_pool)) {
                if (! m_primary_pool.empty()) {
                    continue;
                }
            }
        } else {
            m_primary_pool.erase(m_primary_pool.begin());
            continue;
        }
        return false;
    }
    return false;
}

bool tail::reload_active_pool()
{
    if (m_primary_pool.empty()) {
        return false;
    }

    const tmp_dir::ptr& p_dir = *m_primary_pool.begin();
    if (! p_dir->is_open()) {
        if (! p_dir->open()) {
            return false;
        }
        if (! p_dir->lock(false)) {
            p_dir->close();
            return false;
        }
    }

    if (reload_dir(p_dir, m_active_pool)) {
        if (! m_active_pool.empty()) {
            m_p_active_dir = *m_active_pool.begin();
            m_active_pool.erase(m_active_pool.begin());
            if (! m_p_active_dir->open()) {
                m_p_active_dir.reset();
                return false;
            }
            if (! m_p_active_dir->lock(false)) {
                m_p_active_dir.reset();
                return false;
            }
            return true;
        }
    }
    return false;
}

/*bool tail::reload_dir(const int parend_fd, const int content_type, std::set<std::string>& pool, const std::string cur_name)
{
    assert(utils::is_open(parend_fd));
    assert(pool.empty());

    dir_reader scanner;
    if (! scanner.opendir(parend_fd)) {
        return false;
    }

    while (struct ::dirent* p_ent = scanner.readdir()) {
        if (p_ent->d_type == content_type) {
            const std::string_view dir_name = p_ent->d_name;
            if (dir_name == "." || dir_name == "..") {
                continue;
            }
            if (! cur_name.empty() && dir_name <= cur_name) {
                continue;
            }
            assert(dir_name.size() == gk_name_length);
            pool.emplace(dir_name);
        }
    }
    scanner.close();
    return (! pool.empty());
}*/

bool tail::start()
{
    if (! reload_dir(m_root_dir.fd(), m_primary_pool)) {
        return false;
    }
    if (! m_primary_pool.empty()) {
        if (! reload_dir(*m_primary_pool.begin(), m_active_pool)) {
            return false;
        }
    }
    if (! m_active_pool.empty()) {
        m_p_active_dir = *m_active_pool.begin();
        m_active_pool.erase(m_active_pool.begin());
    }
    if (m_p_active_dir) {
        if (! load_files()) {
            return false;
        }
    }
    return true;
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
