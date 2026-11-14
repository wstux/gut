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

#ifndef _LIBS_GUT_DETAILS_FWD_H_
#define _LIBS_GUT_DETAILS_FWD_H_

#include <sys/types.h>

#include <memory>

namespace wstux {
namespace gut {
namespace details {
namespace fq {

class cleaner;

using cleaner_ptr_t = std::shared_ptr<cleaner>;

} // namespace fq

class dir_t;
class dir_reader;
class tmp_dir;

using tmp_dir_ptr_t = std::shared_ptr<tmp_dir>;

constexpr size_t gk_name_length = 9;
constexpr size_t gk_name_buffer_size = gk_name_length + 1;

constexpr size_t gk_filename_ext_length = 4;
constexpr size_t gk_filename_length = gk_name_length + gk_filename_ext_length;
constexpr size_t gk_filename_buffer_size = gk_filename_length + 1;

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FWD_H_ */
