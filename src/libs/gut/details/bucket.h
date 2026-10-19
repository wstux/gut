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

#ifndef _LIBS_GUT_DETAILS_BUCKET_H_
#define _LIBS_GUT_DETAILS_BUCKET_H_

#include <string>
#include <string_view>

#include "gut/details/types.h"

namespace wstux {
namespace gut {
namespace details {

class bucket final
{
public:
    enum class entry_type
    {
        input,
        output,
        process
    };

public:
    bucket(const std::string_view& root_dir, const entry_type type, const is_stop_fn_t& is_stop_fn);

private:
    static std::string work_dir(const std::string_view& root_dir, const entry_type type);

private:
    const is_stop_fn_t m_is_stop_fn;
    std::string m_work_dir;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_BUCKET_H_ */
