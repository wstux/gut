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

#ifndef _LIBS_GUT_LOGGING_HANDLER_H_
#define _LIBS_GUT_LOGGING_HANDLER_H_

#include <memory>

namespace wstux {
namespace gut {

struct logging_handler
{
    using ptr = std::unique_ptr<logging_handler>;

    /**
     *  \brief  Log message severity levels, analogous to syslog.
     */
    enum severity_level
    {
        emerg   = 0, //!< System is unusable.
        fatal   = 1, //!< Critical core error.
        crit    = 2, //!< Critical condition of components.
        error   = 3, //!< Runtime error.
        warning = 4, //!< Warning.
        notice  = 5, //!< Important notification.
        info    = 6, //!< Informational message.
        debug   = 7, //!< Debugging message.
        trace   = 8  //!< Execution trace.
    };

    /// \brief  Function signature to check if a specific logging level is enabled.
    using can_log_fn_t = bool (*)(void* p_this, severity_level lvl);
    /// \brief  Function signature to write a text message to the log.
    using log_fn_t     = void (*)(void* p_this, severity_level lvl, const char* p_msg);

    /// \brief  Virtual destructor.
    virtual ~logging_handler() {}

    void* p_this = nullptr;            //!< Pointer to the user logger context (the 'this' equivalent).
    can_log_fn_t can_log_fn = nullptr; //!< Pointer to the log level filtering function.
    log_fn_t log_fn = nullptr;         //!< Pointer to the log writing function.
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_LOGGING_HANDLER_H_ */
