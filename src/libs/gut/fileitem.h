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

#ifndef _LIBS_GUT_FILEITEM_H_
#define _LIBS_GUT_FILEITEM_H_

#include <memory>
#include <string_view>

#include "gut/logging_handler.h"
#include "gut/details/entry.h"

namespace wstux {
namespace gut {

class filequeue;
class sink_filequeue;

class fileitem final
{
public:
    using ptr = std::unique_ptr<fileitem>;

public:
    bool is_alive() const;

    bool is_mapped() const;

    const void* map();

    const void* map(const off64_t offset, const size_t size);

    ssize_t read(const off64_t offset, char* p_buffer, const size_t size);

    bool remove();

    ssize_t size() const;

    bool truncate(const size_t size);

    void unmap();

    ssize_t write(const char* p_data, const ssize_t size, const off64_t offset);

private:
    friend filequeue;
    friend sink_filequeue;
    friend ptr std::make_unique<fileitem>(::wstux::gut::logging_handler::ptr&);

    explicit fileitem(logging_handler::ptr& p_logger);

private:
    const logging_handler::ptr& m_p_logger;
    details::entry m_entry;
};

} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_FILEITEM_H_ */
