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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_HEAD_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_HEAD_H_

#include <functional>
#include <memory>
#include <mutex>

namespace wstux {
namespace gut {
namespace details {

class dir_t;
class tmp_dir;

namespace fq {

class head final
{
public:
    using tmp_dir_ptr_t = std::shared_ptr<tmp_dir>;

public:
    head(const dir_t& root_dir, const std::function<bool()>& is_stop_fn);

    tmp_dir_ptr_t get_active_dir(bool force_rotation = false);

private:
    tmp_dir_ptr_t rotate(bool force_rotation);

private:
    const dir_t& m_root_dir;
    const std::function<bool()> m_is_stop_fn;

    std::mutex m_dir_mutex;
    tmp_dir_ptr_t m_p_primary_dir;
    tmp_dir_ptr_t m_p_active_dir;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_HEAD_H_ */
