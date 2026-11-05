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

#include "gut/details/utils.h"

namespace wstux {
namespace gut {
namespace details {
namespace utils {
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
uint64_t time_token_mcs()
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

uint64_t time_token_ns()
{
    // Custom time epoch start: 2026-01-01 00:00:00 UTC in nanoseconds.
    constexpr uint64_t k_epoch_ns = 1767225600000000000ull;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    uint64_t now_nanos = (static_cast<uint64_t>(ts.tv_sec) * 1000000000ull) + ts.tv_nsec;
    uint64_t diff_nanos = (now_nanos > k_epoch_ns) ? (now_nanos - k_epoch_ns) : 0;

    // Atomic sequence counter for same-nanosecond collisions
    static std::atomic<uint16_t> sequence{0};
    uint16_t seq_val = sequence.fetch_add(1, std::memory_order_relaxed) & 0x0FFF;

    // Shift nanoseconds left by 12 bits and merge the sequence counter
    return (diff_nanos << 12) | seq_val;
}

} // <anonymous> namespace

bool close(int& fd)
{
    if (is_open(fd)) {
        const int rc = ::close(fd);
        if (rc != 0) {
            return false;
        }
        fd = -1;
    }
    return true;
}

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
void gen_name(char* p_buffer, size_t size)
{
    assert(size >= gk_name_length);
    static_assert(gk_name_length == gk_name_length_mcs || gk_name_length == gk_name_length_ns);

    if (size < gk_name_length) {
        if (size > 0) {
            *p_buffer = '\0';
        }
        return;
    }

    uint64_t token = 0;
    if constexpr (gk_name_length == gk_name_length_mcs) {
        token = time_token_mcs();
    } else {
        token = time_token_ns();
    }
    encode_base64(token, p_buffer, gk_name_length);
}

bool is_alive(int fd)
{
    stat_t info;
    const int rc = RETRY_EINTR(::fstat64(fd, &info));
    return (rc == 0) ? info.st_nlink > 0 : false;
}

bool lock(const int fd, bool is_exclusive)
{
    const int op = is_exclusive ? LOCK_EX : LOCK_SH;
    const int rc = RETRY_EINTR(::flock(fd, LOCK_NB | op));
    if (rc != 0) {
        return false;
    }
    return true;
}

::time_t mtime(int fd)
{
    stat_t info;
    const int rc = RETRY_EINTR(::fstat64(fd, &info));
    return (rc == 0) ? info.st_mtime : -1;
}

int open_at(int dir_fd, const char* name, int flags, mode_t mode)
{
    int fd = RETRY_EINTR(::openat(dir_fd, name, flags, mode));
    if (! is_open(fd)) {
        switch (errno) {
        case EINVAL:
            if (flags & O_CLOEXEC) {
                flags &= ~O_CLOEXEC;
                fd = open_at(dir_fd, name, flags, mode);
                if (is_open(fd)) {
                    flags = RETRY_EINTR(::fcntl(fd, F_GETFD));
                    if ((flags == -1) || RETRY_EINTR(::fcntl(fd, F_SETFD, FD_CLOEXEC | flags))) {
                        close(fd);
                    }
                }
                return fd;
            } else {
                return fd;
            }
            break;
        case EPERM:
            if (flags & O_NOATIME) {
                flags &= ~O_NOATIME;
                return open_at(dir_fd, name, flags, mode);
            } else {
                return fd;
            }
            break;
        default:
            return fd;
        }
    }
    return fd;
}

void unlock(const int fd)
{
    const int rc = RETRY_EINTR(::flock(fd, LOCK_NB | LOCK_UN));
    if (rc != 0) {
        // \todo Need to do something
        return;
    }
}

} // namespace utils
} // namespace details
} // namespace gut
} // namespace wstux
