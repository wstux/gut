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

//#include <gmock/gmock.h>
//#include <memory>
#include <string>
//#include <vector>
//#include <cassert>

#include <gtest/gtest.h>

#include "gut/details/circular_buffer.h"

namespace {

namespace gut = ::wstux::gut::details;

class instance_counter
{
public:
    static inline int live_instances = 0;
    static inline int move_constructs = 0;
    static inline int copy_constructs = 0;

    static void reset()
    {
        live_instances = 0;
        move_constructs = 0;
        copy_constructs = 0;
    }

    std::string data;

    explicit instance_counter(std::string s)
        : data(std::move(s))
    {
        ++live_instances;
    }
    instance_counter(const instance_counter& other)
        : data(other.data)
    {
        ++live_instances;
        ++copy_constructs;
    }
    instance_counter(instance_counter&& other) noexcept
        : data(std::move(other.data))
    {
        ++live_instances;
        ++move_constructs;
    }

    ~instance_counter() { --live_instances; }

    instance_counter& operator=(const instance_counter&) = delete;
    instance_counter& operator=(instance_counter&&) = delete;
};

} // <anonymous> namespace

TEST(circular_buffer, initial_state)
{
    gut::circular_buffer<int, 5> buffer;

    EXPECT_TRUE(buffer.capacity() == 5) << buffer.capacity();
    EXPECT_TRUE(buffer.size() == 0) << buffer.size();
    EXPECT_TRUE(buffer.empty());
    EXPECT_FALSE(buffer.full());
}

TEST(circular_buffer, push_pop_operations)
{
    gut::circular_buffer<int, 3> buffer;

    buffer.push_back(10);
    EXPECT_EQ(buffer.size(), 1u);
    EXPECT_EQ(buffer.front(), 10);
    EXPECT_FALSE(buffer.empty());

    buffer.push_back(20);
    buffer.push_back(30);
    EXPECT_EQ(buffer.size(), 3u);
    EXPECT_TRUE(buffer.full());

    EXPECT_EQ(buffer.front(), 10);
    buffer.pop_front();

    EXPECT_EQ(buffer.front(), 20);
    buffer.pop_front();

    EXPECT_EQ(buffer.front(), 30);
    buffer.pop_front();

    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0u);
}

TEST(circular_buffer, buffer_wrap)
{
    gut::circular_buffer<int, 3> buffer;

    buffer.push_back(1);
    buffer.push_back(2);
    buffer.pop_front(); // head = 1, tail = 2

    buffer.push_back(3);
    buffer.push_back(4); // tail = 0

    EXPECT_TRUE(buffer.full());
    EXPECT_EQ(buffer.size(), 3u);
    EXPECT_EQ(buffer.front(), 2);

    buffer.pop_front(); // head = 0
    EXPECT_EQ(buffer.front(), 3);
}

TEST(circular_buffer, move)
{
    instance_counter::reset();
    gut::circular_buffer<instance_counter, 2> buffer;

    instance_counter obj("heavy_data");

    buffer.push_back(obj);
    EXPECT_EQ(instance_counter::copy_constructs, 1);
    EXPECT_EQ(instance_counter::move_constructs, 0);

    buffer.push_back(instance_counter("temporary_data"));
    EXPECT_EQ(instance_counter::move_constructs, 1);
}

TEST(circular_buffer, lifecycle)
{
    instance_counter::reset();
    {
        gut::circular_buffer<instance_counter, 4> buffer;
        buffer.push_back(instance_counter("A"));
        buffer.push_back(instance_counter("B"));
        buffer.push_back(instance_counter("C"));

        EXPECT_EQ(instance_counter::live_instances, 3);

        buffer.pop_front();
        EXPECT_EQ(instance_counter::live_instances, 2);
    }
    EXPECT_EQ(instance_counter::live_instances, 0);
}

TEST(circular_buffer, front)
{
    gut::circular_buffer<int, 3> buffer;
    buffer.push_back(100);
    EXPECT_EQ(buffer.front(), 100);

    buffer.front() = 500;
    EXPECT_EQ(buffer.front(), 500);

    const gut::circular_buffer<int, 3>& const_buffer = buffer;
    EXPECT_EQ(const_buffer.front(), 500);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
