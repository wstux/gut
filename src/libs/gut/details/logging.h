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

#ifndef _LIBS_GUT_DETAILS_LOGGING_H_
#define _LIBS_GUT_DETAILS_LOGGING_H_

#include <cstdarg>
#include <cstdio>

#include "gut/logging_handler.h"

namespace wstux {
namespace gut {
namespace details {

inline bool can_log(const logging_handler::ptr& p_hdlr, logging_handler::severity_level lvl)
{
    return p_hdlr && p_hdlr->can_log_fn(p_hdlr->p_this, lvl);
}

inline void log(const logging_handler::ptr& p_hdlr, logging_handler::severity_level lvl, const char* p_fmt, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, p_fmt);
    std::vsnprintf(buffer, sizeof(buffer), p_fmt, args);
    va_end(args);

    p_hdlr->log_fn(p_hdlr->p_this, lvl, buffer);
}

} // namespace details
} // namespace gut
} // namespace wstux

#define _GUT_LOG(logger, level, fmt, ...)                                       \
    do {                                                                        \
        if (::wstux::gut::details::can_log(logger, level)) {                    \
            ::wstux::gut::details::log(logger, level, fmt, ##__VA_ARGS__);      \
        }                                                                       \
    } while(0)

#define GUT_LOG_EMERG(logger,  fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::emerg,  fmt, ##__VA_ARGS__)
#define GUT_LOG_FATAL(logger,  fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::fatal,  fmt, ##__VA_ARGS__)
#define GUT_LOG_CRIT(logger,   fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::crit,   fmt, ##__VA_ARGS__)
#define GUT_LOG_ERROR(logger,  fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::error,  fmt, ##__VA_ARGS__)
#define GUT_LOG_WARN(logger,   fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::warning,fmt, ##__VA_ARGS__)
#define GUT_LOG_NOTICE(logger, fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::notice, fmt, ##__VA_ARGS__)
#define GUT_LOG_INFO(logger,   fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::info,   fmt, ##__VA_ARGS__)
#define GUT_LOG_DEBUG(logger,  fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::debug,  fmt, ##__VA_ARGS__)
#define GUT_LOG_TRACE(logger,  fmt, ...)    _GUT_LOG(logger, ::wstux::gut::logging_handler::severity_level::trace,  fmt, ##__VA_ARGS__)

#endif /* _LIBS_GUT_DETAILS_LOGGING_H_ */
