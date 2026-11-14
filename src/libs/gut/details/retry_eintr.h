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

#ifndef _LIBS_GUT_DETAILS_RETRY_EINTR_H_
#define _LIBS_GUT_DETAILS_RETRY_EINTR_H_

#include <cerrno>

#define RETRY_EINTR(expr)                                                       \
    __extension__ ({                                                            \
        long int __rc;                                                          \
        do {                                                                    \
            __rc = (long int)(expr);                                            \
        } while (__rc == -1L && errno == EINTR);                                \
        __rc;                                                                   \
    })

#define __GUT_IS_OPEN_FD(fd)        (fd >= 0)

#define __GUT_CLOSE_FD(fd)                                                      \
    __extension__ ({                                                            \
        bool __rc = false;                                                      \
        if (__GUT_IS_OPEN_FD(fd)) {                                             \
            if (::close(fd) == 0) {                                             \
                fd = -1;                                                        \
                __rc = true;                                                    \
            }                                                                   \
        }                                                                       \
        __rc;                                                                   \
    })

#define __GUT_LOCK_FD(fd, is_exclusive)                                         \
    __extension__ ({                                                            \
        bool __rc = RETRY_EINTR(::flock(fd, LOCK_NB | (is_exclusive ? LOCK_EX : LOCK_SH))) == 0; \
        __rc;                                                                   \
    })

#define __GUT_UNLOCK_FD(fd)                                                     \
    __extension__ ({                                                            \
        bool __rc = RETRY_EINTR(::flock(fd, LOCK_NB | LOCK_UN)) == 0;           \
        __rc;                                                                   \
    })

#endif /* _LIBS_GUT_DETAILS_RETRY_EINTR_H_ */
