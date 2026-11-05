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
#include <memory>
#include <string>
#include <string_view>

#include "gut/item.h"
#include "gut/details/types.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {

class queue final
{
public:
    using ptr = std::unique_ptr<queue>;
    using is_stop_fn_t = details::is_stop_fn_t;

public:
    item::ptr create();

    const std::string path() const { return m_path; }

    item::ptr pop(size_t wait_ms);

    static ptr create_queue(const std::string_view& path, const is_stop_fn_t& is_stop_fn);

    static ptr create_queue(const std::string_view& path, const std::string_view& out_path, const is_stop_fn_t& is_stop_fn);

private:
    friend std::unique_ptr<queue> std::make_unique<queue>(const std::string_view&, const ::wstux::gut::details::is_stop_fn_t&);
    friend std::unique_ptr<queue> std::make_unique<queue>(const std::string_view&, const std::string_view&,
        const ::wstux::gut::details::is_stop_fn_t&);

    queue(const std::string_view& path, const is_stop_fn_t& is_stop_fn);

    queue(const std::string_view& path, const std::string_view& out_path, const is_stop_fn_t& is_stop_fn);

    details::fq::queue& input_queue() { return m_input; }

private:
    const std::string m_path;
    const is_stop_fn_t m_is_stop_fn;

    details::fq::cleaner::ptr m_p_cleaner;

    details::fq::queue m_input;
    details::fq::queue m_proc;
    details::fq::queue m_output;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_QUEUE_H_ */
