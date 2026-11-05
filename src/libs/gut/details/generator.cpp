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

#include <time.h>
#include <unistd.h>
#include <sys/file.h>

#include <cassert>
#include <cstdint>
#include <atomic>
#include <string_view>

#include "gut/details/generator.h"

namespace wstux {
namespace gut {
namespace details {
namespace {

/**
 *  \brief  Encodes an integer value into a fixed-length Base64 string.
 *  \param  value - unique 64-bit token to encode.
 *  \param  p_buffer - pointer to the destination character array.
 *  \param  length - required length of the resulting string.
 *
 *  \details    The buffer is populated from right to left, starting with the least
 *      significant bits. Unused higher bits are padded with the first character of
 *      the alphabet (zero equivalent).
 *
 *      A filesystem-safe and URL-safe alphabet is used (`-`, `0-9`, `A-Z`, `_`, `a-z`).
 *
 *      Characters in the alphabet are arranged strictly in ascending order of their
 *      ASCII codes. This guarantees that alphabetical sorting performed by the OS
 *      perfectly matches the chronological creation order.
 */
void encode_base64(uint64_t value, char* p_buffer, size_t length)
{
    static constexpr std::string_view k_alphabet = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

    char* s = p_buffer + length;
    *s = '\0';

    while (s > p_buffer) {
        --s;
        *s = k_alphabet[value % k_alphabet.size()];
        value /= k_alphabet.size();
    }
}

/**
 *  \brief  Generates a unique 64-bit token based on time and an atomic counter.
 *  \return Unique 64-bit identifier ready for encoding.
 *
 *  \details    Operates in a lock-free manner and is fully thread-safe. Mitigates
 *      system timer granularity limitations and concurrent multi-threaded execution.
 *
 *      A custom epoch (2026/01/01) is subtracted from the current system time in
 *      nanoseconds. This reduces the bit width of the number and conserves space
 *      in the final name.
 *
 *      The resulting delta ('diff_nanos') is shifted left by 12 bits ('diff_nanos << 12').
 *
 *      A cyclic atomic sequence counter is embedded into the lower 12 bits. This
 *      guarantees token uniqueness even if multiple threads invoke the function
 *      simultaneously within the exact same nanosecond (supporting up to 4096
 *      generations per nanosecond per process).
 */
uint64_t time_token()
{
    // Custom time epoch start: 2026-01-01 00:00:00 UTC in microseconds.
    constexpr uint64_t k_epoch_mcs = 1767225600000000ull;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    // Convert seconds and nanoseconds to total microseconds
    uint64_t now_micros = (static_cast<uint64_t>(ts.tv_sec) * 1000000ull) + (ts.tv_nsec / 1000L);

    // Guard against system time back-shifts (e.g., via NTP synchronization)
    uint64_t diff_micros = (now_micros > k_epoch_mcs) ? (now_micros - k_epoch_mcs) : 0;


    // Atomic sequence counter to prevent collisions within the same microsecond.
    // The 0x0FFF mask allocates exactly 12 bits (range 0 - 4095) for the sequence.
    static std::atomic<uint16_t> sequence{0};
    uint16_t seq_val = sequence.fetch_add(1, std::memory_order_relaxed) & 0x0FFF;

    // Shift microseconds left by 12 bits and merge the sequence counter into the lower bits.
    return (diff_micros << 12) | seq_val;
}

} // <anonymous> namespace

void generate_name(char* p_buffer, size_t size)
{
    assert(size >= gk_name_buffer_size);

    if (size < gk_name_length) {
        if (size > 0) {
            *p_buffer = '\0';
        }
        return;
    }

    const uint64_t token = time_token();
    encode_base64(token, p_buffer, gk_name_length);
}

} // namespace details
} // namespace gut
} // namespace wstux
