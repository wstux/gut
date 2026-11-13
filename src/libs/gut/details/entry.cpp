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
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

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
    unmap();
    utils::close(m_fd);
    std::atomic_store(&m_p_parent_dir, tmp_dir::ptr());
}

bool entry::create(const tmp_dir::ptr& p_dir, const std::string_view& name)
{
    if (IS_OPEN_FD(m_fd)) {
        return false;
    }
    if (! p_dir || (! name.empty() && name.size() != gk_filename_length)) {
        return false;
    }
    for (size_t i = 0; i < 8 && ! IS_OPEN_FD(m_fd); ++i) {
        if (! name.empty()) {
            ::strncpy(m_name, name.data(), gk_filename_length);
        } else {
            generate_name(m_name, gk_filename_buffer_size);
            ::memcpy(m_name + gk_name_length, ".dat", 4);
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
            std::atomic_store(&m_p_parent_dir, p_dir);
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
    if (! IS_OPEN_FD(m_fd)) {
        return false;
    }
    struct ::stat64 info;
    const int rc = RETRY_EINTR(::fstat64(m_fd, &info));
    return (rc == 0) ? info.st_nlink > 0 : false;
}

const void* entry::map(const off64_t offset, const size_t size)
{
    assert(IS_OPEN_FD(m_fd));
    if (m_map_addr && (m_map_offset == offset) && (m_map_size == size)) {
        return m_map_addr;
    }

    unmap();

    m_map_addr = ::mmap64(nullptr, size, PROT_READ, MAP_SHARED | MAP_FILE, m_fd, offset);
    if (m_map_addr && (m_map_addr != MAP_FAILED)) {
        m_map_offset = offset;
        m_map_size = size;
    } else {
        m_map_addr = nullptr;
    }

    return m_map_addr;
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

    if (IS_OPEN_FD(m_fd) || m_name[0] != '\0') {
        return false;
    }
    if (! p_parent_dir || name.size() != gk_filename_length) {
        return false;
    }

    constexpr int flags = O_RDWR | O_NOATIME | O_NOCTTY | O_CLOEXEC | O_LARGEFILE | O_NOFOLLOW;
    constexpr int mode = S_IRUSR | S_IWUSR; // 0600
    m_fd = RETRY_EINTR(::openat(p_parent_dir->fd(), name.data(), flags, mode));
    if (! IS_OPEN_FD(m_fd)) {
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

ssize_t entry::read(const off64_t offset, char* p_buffer, size_t size)
{
    assert(p_buffer && size);

    if (offset > entry::size()) {
        return -1;
    }

    if (::lseek(m_fd, offset, SEEK_SET) < 0) {
        return -1;
    }

    ssize_t chunk_size = 0;
    ssize_t read_size = 0;
    do {
        chunk_size = ::read(m_fd, &p_buffer[read_size], size);
        if ((chunk_size < 0) && (errno != EINTR)) {
            return chunk_size;
        }

        if (chunk_size > 0) {
            size -= chunk_size;
            read_size += chunk_size;
        }
    } while ((chunk_size > 0) && (size > 0));
    return read_size;
}

bool entry::remove()
{
    if (m_name[0] == '\0') {
        return false;
    }

    tmp_dir::ptr p_dir = std::atomic_load(&m_p_parent_dir);
    if (! p_dir) {
        return false;
    }

    const int rc = RETRY_EINTR(::unlinkat(p_dir->fd(), m_name, 0));
    if (rc != 0) {
        return false;
    }
    close();

    return true;
}

ssize_t entry::size() const
{
    assert(IS_OPEN_FD(m_fd));

    struct ::stat64 info;
    const int rc = RETRY_EINTR(::fstat64(m_fd, &info));
    return (rc == 0) ? info.st_size : -1;
}

bool entry::truncate(const size_t size)
{
    assert(IS_OPEN_FD(m_fd));
    if (! is_alive()) {
        return false;
    }
    const int rc = RETRY_EINTR(::ftruncate(m_fd, size));
    if (rc == 0) {
        if (is_mapped()) {
            unmap();
        }
        return true;
    }
    return false;
}

void entry::unmap()
{
    if (m_map_addr) {
        const int rc = RETRY_EINTR(::munmap((void*)m_map_addr, m_map_size));
        if (rc != 0) {
            // \todo Need to do something
        }

        m_map_addr = nullptr;
        m_map_size = 0;
        m_map_offset = 0;
    }
}

ssize_t entry::write(const char* p_data, const ssize_t size, const off64_t offset)
{
    ssize_t total = 0;

    if (! IS_OPEN_FD(m_fd)) {
        assert(IS_OPEN_FD(m_fd));
        return -1;
    }

    if (offset >= 0) {
        if (::lseek(m_fd, offset, SEEK_SET) < 0) {
            return  -1;
        }
    }

    do {
        const ssize_t written = ::write(m_fd, p_data + total, size - total);
        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return total;
        }
        total += written;
    } while (total < size);
    assert(total == size);
    return total;
}

} // namespace details
} // namespace gut
} // namespace wstux
