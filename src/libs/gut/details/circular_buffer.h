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

#ifndef _LIBS_GUT_DETAILS_CIRCULAR_BUFFER_H_
#define _LIBS_GUT_DETAILS_CIRCULAR_BUFFER_H_

#include <cassert>
#include <cstddef>
#include <array>
#include <new>
#include <utility>

namespace wstux {
namespace gut {
namespace details {

template <typename T, std::size_t N>
class circular_buffer final
{
public:
    using value_type = T;
    using size_type = std::size_t;

public:
    circular_buffer() = default;

    ~circular_buffer()
    {
        while (! empty()) {
            pop_front();
        }
    }

    circular_buffer(const circular_buffer&) = delete;
    circular_buffer& operator=(const circular_buffer&) = delete;

    constexpr size_type capacity() const noexcept { return N; }

    bool empty() const noexcept { return m_size == 0; }

    const T& front() const
    {
        assert(! empty());
        return *ptr_at(m_head);
    }

    T& front()
    {
        assert(! empty());
        return *ptr_at(m_head);
    }

    bool full() const noexcept { return m_size == N; }

    void pop_front() noexcept
    {
        if (!empty()) {
            ptr_at(m_head)->~T();
            m_head = (m_head + 1) % N;
            m_size--;
        }
    }

    void push_back(const T& value)
    {
        assert(! full());
        ::new (static_cast<void*>(ptr_at(m_tail))) T(value);
        m_tail = (m_tail + 1) % N;
        ++m_size;
    }

    void push_back(T&& value)
    {
        assert(! full());
        ::new (static_cast<void*>(ptr_at(m_tail))) T(std::move(value));
        m_tail = (m_tail + 1) % N;
        ++m_size;
    }

    size_type size() const noexcept { return m_size; }

private:
    struct alignas(T) value_storage final
    {
        std::array<std::byte, sizeof(T)> data;
    };

private:
    T* ptr_at(size_type index) noexcept
    {
        return reinterpret_cast<T*>(m_storage[index].data.data());
    }

    const T* ptr_at(size_type index) const noexcept
    {
        return reinterpret_cast<const T*>(m_storage[index].data.data());
    }

private:
    std::array<value_storage, N> m_storage;

    size_type m_head = 0;
    size_type m_tail = 0;
    size_type m_size = 0;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_CIRCULAR_BUFFER_H_ */
