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

#ifndef _LIBS_GUT_DETAILS_ENTRY_H_
#define _LIBS_GUT_DETAILS_ENTRY_H_

#include <string_view>

#include "gut/details/dir.h"
#include "gut/details/utils.h"

namespace wstux {
namespace gut {
namespace details {

class entry final
{
public:
    entry();

    ~entry();

    void close();

    bool create(const tmp_dir::ptr& p_dir, const std::string_view& name);

    bool is_alive() const;

    bool move(const tmp_dir::ptr& p_new_parent_dir);

    std::string_view name() const { return m_name; }

    bool open(const tmp_dir::ptr& p_parent_dir, const std::string_view& name);

    //void reset() {}

private:
    char m_name[utils::gk_filename_buffer_size];
    int m_fd = -1;
    tmp_dir::ptr m_p_parent_dir;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_ENTRY_H_ */
