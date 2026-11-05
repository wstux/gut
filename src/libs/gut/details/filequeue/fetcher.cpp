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

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/inotify.h>

#include <cstring>
#include <array>
#include <filesystem>

#include "gut/details/filequeue/fetcher.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

fetcher::fetcher(dir_t& root_dir, const cleaner::ptr& p_cleaner, const is_stop_fn_t& is_stop_fn)
    : m_is_stop_fn(is_stop_fn)
    , m_root_dir(root_dir)
    , m_p_cleaner(p_cleaner)
{
    if (! m_root_dir.is_open()) {
        m_root_dir.open();
    }
    m_root_dir.lock(false);
}

fetcher::~fetcher()
{}

bool fetcher::change_active_dir()
{
    if (m_active_pool.empty()) {
        return false;
    }
    if (! m_p_primary_dir) {
        return false;
    }

    assert(m_files_pool.empty());
    assert(m_p_primary_dir.get() != nullptr);

    if (m_scanner.is_opened()) {
        m_scanner.close();
    }

    m_p_cleaner->push(m_p_active_dir);
    //m_p_active_dir = std::make_shared<tmp_dir>(m_active_pool.front().c_str(), m_p_primary_dir->fd());
    m_p_active_dir = std::make_shared<tmp_dir>(m_active_pool.begin()->c_str(), m_p_primary_dir->fd());
    //m_active_pool.pop_front();
    m_active_pool.erase(m_active_pool.begin());

    if (m_p_active_dir->open() && m_scanner.opendir(m_p_active_dir->fd())) {
        return load_files();
    }

    m_p_active_dir.reset();
    return false;
}

bool fetcher::change_primary_dir()
{
    if (m_primary_pool.empty()) {
        return false;
    }

    if (m_scanner.is_opened()) {
        if (! m_active_pool.empty() || ! m_primary_pool.empty()) {
            m_scanner.close();
            m_p_cleaner->push(m_p_active_dir);
        } else {
            return false;
        }
    }

    assert(m_active_pool.empty());

    if (m_scanner.is_opened()) {
        m_scanner.close();
    }

    m_p_cleaner->push(m_p_primary_dir);
    //m_p_primary_dir = std::make_shared<tmp_dir>(m_primary_pool.front().c_str(), m_root_dir.fd());
    m_p_primary_dir = std::make_shared<tmp_dir>(m_primary_pool.begin()->c_str(), m_root_dir.fd());
    //m_primary_pool.pop_front();
    m_primary_pool.erase(m_primary_pool.begin());
    if (! m_p_primary_dir->open()) {
        m_p_primary_dir.reset();
        return false;
    }

    dir_reader scanner;
    if (scanner.opendir(m_p_primary_dir->fd())) {
        //while (! m_active_pool.full()) {
        while (struct ::dirent* p_ent = scanner.readdir()) {
            //struct ::dirent* p_ent = scanner.readdir();
            //if (! p_ent) {
            //    break;
            //}

            if (p_ent->d_type == DT_DIR) {
                const std::string_view dir_name = p_ent->d_name;
                if (dir_name == "." || dir_name == "..") {
                    continue;
                }
                assert(dir_name.size() == gk_name_length);
                //m_active_pool.push_back(p_ent->d_name);
                m_active_pool.emplace(p_ent->d_name);
            }
        }
        if (! m_active_pool.empty()) {
            return change_active_dir();
        }
        return false;
    }
    m_p_primary_dir.reset();
    return false;
}

bool fetcher::fetch(std::string& filename, tmp_dir::ptr& p_dir)
{
    if (! m_root_dir.is_open()) {
        return false;
    }

    filename.clear();
    p_dir.reset();

    if (m_primary_pool.empty()) {
        if (! load_primary()) {
            return false;
        }
    }

    while (! m_is_stop_fn()) {
        if (! m_files_pool.empty()) {
            //filename = std::move(m_files_pool.front());
            filename = *m_files_pool.begin();
            //m_files_pool.pop_front();
            m_files_pool.erase(m_files_pool.begin());
            p_dir = m_p_active_dir;
            return true;
        }

        if (load_files()) {
            continue;
        }

        if (change_active_dir()) {
            continue;
        }
        if (change_primary_dir()) {
            continue;
        }

        return false;
    }
    return false;
}

bool fetcher::load_files()
{
    if (m_scanner.is_opened()) {
        m_scanner.rewinddir();
    } else {
        return (! m_files_pool.empty());
    }
    //while (! m_files_pool.full()) {
    while (struct ::dirent* p_file_ent = m_scanner.readdir()) {
        //struct ::dirent* p_file_ent = m_scanner.readdir();
        //if (! p_file_ent) {
        //    break;
        //}

        if (p_file_ent->d_type == DT_REG) {
            //m_files_pool.push_back(p_file_ent->d_name);
            m_files_pool.emplace(p_file_ent->d_name);
        }
    }
    return (! m_files_pool.empty());
}

bool fetcher::load_primary()
{
    dir_reader root_scanner;
    if (! root_scanner.opendir(m_root_dir.fd())) {
        return false;
    }

    //while (! m_primary_pool.full()) {
    while (struct ::dirent* p_ent = root_scanner.readdir()) {
        //struct ::dirent* p_ent = root_scanner.readdir();
        //if (! p_ent) {
        //    break;
        //}

        if (p_ent->d_type == DT_DIR) {
            const std::string_view dir_name = p_ent->d_name;
            if (dir_name == "." || dir_name == "..") {
                continue;
            }
            assert(dir_name.size() == gk_name_length);
            //m_primary_pool.push_back(p_ent->d_name);
            m_primary_pool.emplace(p_ent->d_name);
        }
    }
    root_scanner.close();
    return true;
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
