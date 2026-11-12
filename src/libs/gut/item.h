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

#ifndef _LIBS_GUT_ITEM_H_
#define _LIBS_GUT_ITEM_H_

#include <memory>

#include "gut/details/entry.h"
#include "gut/details/types.h"
#include "gut/details/filequeue/queue.h"

namespace wstux {
namespace gut {

class queue;

class item final
{
public:
    using ptr = std::unique_ptr<item>;

public:
    bool commit();

    bool rollback();

private:
    friend class queue;
    //friend std::unique_ptr<item> std::make_unique<item>(::wstux::gut::details::fq::queue*&&,
    //    ::wstux::gut::details::fq::queue*&&, const ::wstux::gut::details::is_stop_fn_t&);
    friend std::unique_ptr<item> std::make_unique<item>(::wstux::gut::details::fq::sink_queue*&&,
        ::wstux::gut::details::fq::queue*&&, const ::wstux::gut::details::is_stop_fn_t&);

    item(details::fq::sink_queue* p_commit_queue, details::fq::queue* p_rollback_queue, const details::is_stop_fn_t& is_stop_fn);

private:
    const details::is_stop_fn_t m_is_stop_fn;

    details::entry m_entry;

    details::fq::sink_queue* m_p_commit_queue = nullptr;
    details::fq::queue* m_p_rollback_queue = nullptr;
    //details::fq::queue* m_p_current_queue = nullptr;
    bool m_is_moved = false;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_ITEM_H_ */
