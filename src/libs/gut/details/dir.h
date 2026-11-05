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

#ifndef _LIBS_GUT_DETAILS_DIR_H_
#define _LIBS_GUT_DETAILS_DIR_H_

#include <dirent.h>

#include <memory>
#include <string>
#include <string_view>

#include "gut/details/utils.h"

namespace wstux {
namespace gut {
namespace details {

class tmp_dir final
{
public:
    using ptr = std::shared_ptr<tmp_dir>;

public:
    explicit tmp_dir(int parent_fd);

    tmp_dir(const char* p_name, int parent_fd);

    tmp_dir(const std::string_view& name, int parent_fd);

    ~tmp_dir();

    void close();

    bool create();

    int fd() const { return m_fd; }

    bool lock(bool is_exclusive) const { return utils::lock(m_fd, is_exclusive); }

    ::time_t mtime() const
    {
        struct ::stat64 info;
        return (RETRY_EINTR(::fstat64(m_fd, &info)) == 0) ? info.st_mtime : -1;
    }

    std::string_view name() const { return m_name; }

    bool open();

    bool remove();

    size_t size() const;

    void unlock() const { utils::unlock(m_fd); }

private:
    const int m_parent_fd = -1;

    int m_fd = -1;
    char m_name[utils::gk_name_buffer_size];
};

class dir_t final
{
public:
    explicit dir_t(std::string dir_path);

    explicit dir_t(std::string_view dir_path);

    ~dir_t();

    int fd() const { return m_fd; }

    bool is_open() const { return utils::is_open(m_fd); }

    bool lock(bool is_exclusive) const { return utils::lock(m_fd, is_exclusive); }

    bool open();

    std::string_view path() const { return m_path; }

    void unlock() const { utils::unlock(m_fd); }

private:
    std::string m_path;
    int m_fd = -1;
};

class dir_reader final
{
public:
    dir_reader() = default;

    dir_reader(const dir_reader&) = delete;
    dir_reader& operator=(const dir_reader&) = delete;

    dir_reader(dir_reader&& other) noexcept;
    dir_reader& operator=(dir_reader&& other) noexcept;

    ~dir_reader() { close(); }

    void close() noexcept;

    int dirfd() const { return m_p_scanner ? ::dirfd(m_p_scanner) : -1; }

    bool is_opened() const { return m_p_scanner != nullptr; }

    bool opendir(const char* p_path) noexcept;

    bool opendir(int fd) noexcept;

    struct ::dirent* readdir() const noexcept;

    void rewinddir() const noexcept;

private:
    DIR* m_p_scanner = nullptr;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_DIR_H_ */
