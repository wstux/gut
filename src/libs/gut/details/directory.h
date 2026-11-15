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

#ifndef _LIBS_GUT_DETAILS_DIRECTORY_H_
#define _LIBS_GUT_DETAILS_DIRECTORY_H_

#include <string>
#include <string_view>

namespace wstux {
namespace gut {
namespace details {

class directory final
{
public:
    explicit directory(std::string_view dir_path);

    ~directory();

    int fd() const { return m_fd; }

    bool is_open() const;

    bool lock(bool is_exclusive) const;

    bool open();

    std::string_view path() const { return m_path; }

    void unlock() const;

    static int openat(int dir_fd, const std::string_view& dir_name, int flags);

    static bool open_dir(int& fd, const int parent_fd, const std::string_view& dir_name);

private:
    std::string m_path;
    int m_fd = -1;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_DIRECTORY_H_ */
