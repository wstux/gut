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

#include <fcntl.h>

#include <cassert>
#include <cstring>

#include "gut/details/entry.h"
#include "gut/details/retry_eintr.h"

namespace wstux {
namespace gut {
namespace details {

entry::entry()
{
    m_name[0] = '\0';
}

entry::~entry()
{
    close();
}

void entry::close()
{
    // unmap if needs
    utils::close(m_fd);
    m_p_parent_dir.reset();
}

bool entry::create(const tmp_dir::ptr& p_dir, const std::string_view& name)
{
    if (utils::is_open(m_fd)) {
        return false;
    }
    if (! p_dir || (! name.empty() && name.size() != gk_filename_length)) {
        return false;
    }
    for (size_t i = 0; i < 8 && ! utils::is_open(m_fd); ++i) {
        if (! name.empty()) {
            ::strncpy(m_name, name.data(), gk_filename_length);
        } else {
            generate_name(m_name, gk_filename_buffer_size);
            m_name[gk_name_length + 0] = '.';
            m_name[gk_name_length + 1] = 'd';
            m_name[gk_name_length + 2] = 'a';
            m_name[gk_name_length + 3] = 't';
        }
        m_name[gk_filename_buffer_size - 1] = '\0';

        constexpr int flags = O_CREAT | O_EXCL | O_RDWR | O_NOCTTY | O_CLOEXEC | O_LARGEFILE | O_NOFOLLOW | O_NOATIME;
        constexpr int mode = S_IRUSR | S_IWUSR; // 0600
        m_fd = RETRY_EINTR(::openat(p_dir->fd(), m_name, flags, mode));
        if (m_fd == -1) {
            if (errno != EEXIST) {
                return false;
            }
        } else {
            m_p_parent_dir = p_dir;
            if (! utils::lock(m_fd, true)) {
                close();
                return false;
            }
            return true;
        }
    }

    return false;
}

bool entry::is_alive() const
{
    if (! utils::is_open(m_fd)) {
        return false;
    }
    return utils::is_alive(m_fd);
}

bool entry::move(const tmp_dir::ptr& p_new_parent_dir)
{
    assert(m_name[0] != '\0');

    tmp_dir::ptr p_cur_parent_dir = std::atomic_load(&m_p_parent_dir);
    assert(p_cur_parent_dir && p_new_parent_dir);

    if (m_name[0] == '\0') {
        return false;
    }

    if (! p_cur_parent_dir || ! p_new_parent_dir) {
        return false;
    }

    if (p_cur_parent_dir->fd() == p_new_parent_dir->fd()) {
        return false;
    }

    const int rc = RETRY_EINTR(::renameat(p_cur_parent_dir->fd(), m_name, p_new_parent_dir->fd(), m_name));
    if (rc == 0) {
        std::atomic_store(&m_p_parent_dir, p_new_parent_dir);
        return true;
    }
    return false;
}

bool entry::open(const tmp_dir::ptr& p_parent_dir, const std::string_view& name)
{
    assert(m_name[0] == '\0');
    assert(p_parent_dir);

    if (utils::is_open(m_fd) || m_name[0] != '\0') {
        return false;
    }
    if (! p_parent_dir || name.size() != gk_filename_length) {
        return false;
    }

    constexpr int flags = O_RDWR | O_NOATIME | O_NOCTTY | O_CLOEXEC | O_LARGEFILE | O_NOFOLLOW;
    constexpr int mode = S_IRUSR | S_IWUSR; // 0600
    m_fd = RETRY_EINTR(::openat(p_parent_dir->fd(), name.data(), flags, mode));
    if (! utils::is_open(m_fd)) {
        return false;
    }
    if (! utils::lock(m_fd, true)) {
        utils::close(m_fd);
        return false;
    }
    std::atomic_store(&m_p_parent_dir, p_parent_dir);
    ::strncpy(m_name, name.data(), gk_filename_length);
    m_name[gk_filename_buffer_size - 1] = '\0';
    return true;
}

} // namespace details
} // namespace gut
} // namespace wstux
