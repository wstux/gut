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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_FETCHER_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_FETCHER_H_

#include <set>

#include "gut/details/dir.h"
#include "gut/details/types.h"
#include "gut/details/filequeue/cleaner.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

class fetcher final
{
public:
    fetcher(dir_t& root_dir, const cleaner::ptr& p_cleaner, const is_stop_fn_t& is_stop_fn);

    ~fetcher();

    /// \brief  Retrieves the next item from the disk-based queue.
    /// \param  filename - the name of the discovered task file.
    /// \param  p_dir - a pointer to the physical directory containing the file.
    /// \return true if an item was successfully retrieved and is ready for processing overwise false.
    bool fetch(std::string& filename, tmp_dir::ptr& p_dir);

private:
    bool change_active_dir();

    bool change_primary_dir();

    bool load_files();

    bool load_primary();

private:
    const is_stop_fn_t m_is_stop_fn;

    //circular_buffer<std::string, 16> m_primary_pool;
    std::set<std::string> m_primary_pool;
    /**
     * inode (8 B) + rec_len (2 B) + name_len (1 B) + file_type (1 B) name (in cur. 10 B) = 22 B => 24 B (with align)
     * 64 KB / 24 B ~= 2730 records
     */
    //circular_buffer<std::string, 512> m_active_pool;
    std::set<std::string> m_active_pool;
    /**
     * inode (8 B) + rec_len (2 B) + name_len (1 B) + file_type (1 B) name (in cur. 14 B) = 26 B => 32 B (with align)
     * 64 KB / 32 B ~= 2048 records
     */
    //circular_buffer<std::string, 1024> m_files_pool;
    std::set<std::string> m_files_pool;

    dir_t& m_root_dir;
    tmp_dir::ptr m_p_primary_dir;
    tmp_dir::ptr m_p_active_dir;

    dir_reader m_scanner;

    cleaner::ptr m_p_cleaner;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_FETCHER_H_ */
