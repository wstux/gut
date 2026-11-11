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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_TAIL_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_TAIL_H_

#include <memory>
#include <set>
#include <string>

#include "gut/details/types.h"
#include "gut/details/filequeue/notifier.h"

namespace wstux {
namespace gut {
namespace details {

class dir_t;
class tmp_dir;

namespace fq {

class cleaner;

class tail final
{
public:
    using cleaner_ptr_t = std::shared_ptr<cleaner>;
    using tmp_dir_ptr_t = std::shared_ptr<tmp_dir>;

public:
    tail(const dir_t& root_dir, const cleaner_ptr_t& p_cleaner, const is_stop_fn_t& is_stop_fn);

    ~tail();

    /// \brief  Retrieves the next item from the disk-based queue.
    /// \param  filename - the name of the discovered task file.
    /// \param  p_dir - a pointer to the physical directory containing the file.
    /// \return true if an item was successfully retrieved and is ready for processing overwise false.
    bool get(std::string& filename, tmp_dir_ptr_t& p_dir, size_t wait_ms);

    bool start();

private:
    bool reload();

private:
    class primary_tier;
    using primary_tier_ptr_t = std::shared_ptr<primary_tier>;

private:
    const dir_t& m_root_dir;
    const is_stop_fn_t m_is_stop_fn;

    cleaner_ptr_t m_p_cleaner;

    tmp_dir_ptr_t m_p_active_dir;

    std::mutex m_tail_mutex;
    notifier m_notifier;
    int m_root_wd = -1;
    std::set<std::string> m_primary_pool;
    std::map<std::string, primary_tier_ptr_t> m_primary_tiers;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_TAIL_H_ */
