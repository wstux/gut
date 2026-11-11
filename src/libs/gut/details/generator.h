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

#ifndef _LIBS_GUT_DETAILS_GENERATOR_H_
#define _LIBS_GUT_DETAILS_GENERATOR_H_

#include <cstddef>

namespace wstux {
namespace gut {
namespace details {

constexpr size_t gk_name_length = 9;
constexpr size_t gk_name_buffer_size = gk_name_length + 1;

constexpr size_t gk_filename_ext_length = 4;
constexpr size_t gk_filename_length = gk_name_length + gk_filename_ext_length;
constexpr size_t gk_filename_buffer_size = gk_filename_length + 1;


/**
 *  \brief  Generating unique chronological name.
 *
 *  \details    This algorithm generates compact, fixed-length names that are strictly
 *      time-ordered (lexicographically sortable) and safe from collisions in highly
 *      concurrent multi-threaded environments.
 *
 *      The capacity of 9 Base64 characters provides (64^9 approx 1.8 times 10^16)
 *      combinations. Accounting for the 12-bit shift, this setup provides a lifespan
 *      of over 139 years (until the year 2165) without any risk of buffer size overflow.
 *
 *      Each character in Base64 represents exactly 6 bits of information (2⁶ = 64).
 *      Therefore, a string of 9 characters provides exactly 54 bits of storage capacity:
 *      (64^9 = (2^6)^9 = 2^{54} = 18,014,398,509,481,984) unique combinations.
 *
 *      Out of these 54 bits, the token payload is split as follows:
 *      - Lower 12 bits: Reserved for the sequence counter (handles 4096 collisions per tick).
 *      - Upper 42 bits: Reserved for the time delta ('diff_micros').
 *
 *      Since our custom epoch starts on January 1st, 2026, a 9-character buffer provides
 *      a flawless operational lifespan up until May 2165 before integer overflow occurs.
 */
void generate_name(char* p_bufer, size_t size);

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_GENERATOR_H_ */
