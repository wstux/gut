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

#ifndef _LIBS_GUT_QUEUE_H_
#define _LIBS_GUT_QUEUE_H_

#include <functional>
#include <string>
#include <string_view>

#include "gut/item.h"
#include "gut/details/bucket.h"
#include "gut/details/types.h"

namespace wstux {
namespace gut {

class queue final
{
public:
    using is_stop_fn_t = details::is_stop_fn_t;

public:
    queue(const std::string_view& path, const is_stop_fn_t& is_stop_fn);

    item::ptr create();

    item::ptr get(size_t wait_ms, bool is_read_only);

private:
    const std::string m_path;
    const is_stop_fn_t m_is_stop_fn;

    details::bucket m_input;
    details::bucket m_proc;
    details::bucket m_outpit;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_QUEUE_H_ */
