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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_QUEUE_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <string>
#include <string_view>

#include "gut/details/dir.h"
#include "gut/details/entry.h"
#include "gut/details/types.h"
#include "gut/details/filequeue/cleaner.h"
//#include "gut/details/filequeue/fetcher.h"
#include "gut/details/filequeue/head.h"
#include "gut/details/filequeue/tail.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

class queue final
{
public:
    queue(const std::string_view& root_dir, const cleaner::ptr& p_cleaner, const is_stop_fn_t& is_stop_fn);

    bool create_item(entry& e, const std::string_view& name = std::string_view());


    bool open();

    bool pop_item(entry& e, queue& target);

    bool push_item(entry& e);

    bool start();

private:
    //tmp_dir::ptr rotate(bool force_rotation);

private:
    const is_stop_fn_t m_is_stop_fn;

    //std::mutex m_dir_mutex;
    //std::condition_variable m_dir_cv;
    //bool m_is_rotating_process = false;
    dir_t m_root_dir;
    //tmp_dir::ptr m_p_primary_dir;
    //tmp_dir::ptr m_p_active_dir;

    cleaner::ptr m_p_cleaner;
    size_t m_cleaner_ts = 0;

    head m_queue_head;
    tail m_queue_tail;
    //fetcher m_fetcher;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_QUEUE_H_ */
