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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_NOTIFIER_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_NOTIFIER_H_

#include <functional>
#include <map>
#include <string_view>

#include "gut/details/types.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

class notifier final
{
public:
    using callback_fn_t = std::function<void(uint32_t mask, const std::string_view& name)>;

public:
    notifier();

    ~notifier();

    notifier(const notifier&) = delete;
    notifier& operator=(const notifier&) = delete;

    int add_watch(const std::string_view& path, uint32_t mask, callback_fn_t cb);

    bool fetch();

    bool fetch(const size_t wait_ms);

    void remove_watch(int wd);

    bool wait(const size_t wait_ms);

private:
    void skip_all();

private:
    int m_inotify_fd = -1;
    std::map<int, callback_fn_t> m_wd_to_callback;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_NOTIFIER_H_ */
