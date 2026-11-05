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

#include <atomic>
#include <chrono>
#include <filesystem>
#include <random>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "gut/details/filequeue/cleaner.h"
#include "gut/details/filequeue/queue.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;
namespace fq = gut::details::fq;

class filequeue_fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_root_path = make_tmp_dir("ut_gut_filequeue");
        ASSERT_TRUE(std::filesystem::create_directory(m_root_path)) << "Failed to create root directory '" << m_root_path << "'";

        m_work_path = m_root_path / "queue";
        ASSERT_TRUE(std::filesystem::create_directory(m_work_path)) << "Failed to create work directory '" << m_work_path << "'";

        m_p_cleaner = std::make_shared<details::fq::cleaner>([]() -> bool { return false; });
    }

    void TearDown() override
    {
        std::filesystem::remove_all(m_root_path);
    }

    std::string dst_queue_dir() const { return (m_work_path / "dst").string(); }

    std::string src_queue_dir() const { return (m_work_path / "src").string(); }

    fq::queue make_fq(const std::string& path)
    {
        return fq::queue(path, m_p_cleaner, []() -> bool { return false; });
    }

private:
    static std::filesystem::path make_tmp_dir(const std::string& prefix)
    {
        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();

        std::random_device rd;
        std::mt19937 prng(rd());
        std::uniform_int_distribution<uint32_t> rand_dist;

        std::string rand_hex = std::to_string(rand_dist(prng));
        return temp_dir / (prefix + "_" + rand_hex);
    }

private:
    std::filesystem::path m_root_path;
    std::filesystem::path m_work_path;
    details::fq::cleaner::ptr m_p_cleaner;
};

} // <anonymous> namespace

TEST_F(filequeue_fixture, create_new_item)
{
    fq::queue queue = make_fq(src_queue_dir());

    ASSERT_TRUE(queue.open());

    details::entry e;
    EXPECT_TRUE(queue.create_item(e, "item_0001.dat"));
    EXPECT_TRUE(e.is_alive());
}

TEST_F(filequeue_fixture, push_item)
{
    fq::queue dst_queue = make_fq(dst_queue_dir());
    fq::queue src_queue = make_fq(src_queue_dir());
    ASSERT_TRUE(dst_queue.open());
    ASSERT_TRUE(src_queue.open());

    details::entry e;
    ASSERT_TRUE(src_queue.create_item(e, "item_0002.dat"));
    EXPECT_TRUE(dst_queue.push_item(e));
}

TEST_F(filequeue_fixture, move_item)
{
    fq::queue dst_queue = make_fq(dst_queue_dir());
    fq::queue src_queue = make_fq(src_queue_dir());
    ASSERT_TRUE(dst_queue.open());
    ASSERT_TRUE(src_queue.open());

    {
        details::entry e;
        ASSERT_TRUE(src_queue.create_item(e, "item_0003.dat"));
    }
    {
        details::entry e;
        EXPECT_TRUE(src_queue.pop_item(e, dst_queue));
        EXPECT_TRUE(e.is_alive());
        EXPECT_TRUE(e.name() == "item_0003.dat") << e.name();
    }
    details::entry e;
    EXPECT_FALSE(src_queue.pop_item(e, dst_queue));
    EXPECT_TRUE(dst_queue.pop_item(e, src_queue));
    EXPECT_TRUE(e.name() == "item_0003.dat") << e.name();
}

TEST_F(filequeue_fixture, names_collision)
{
    fq::queue queue = make_fq(src_queue_dir());
    ASSERT_TRUE(queue.open());

    details::entry e1;
    details::entry e2;

    ASSERT_TRUE(queue.create_item(e1, "item_0003.dat"));
    ASSERT_TRUE(queue.create_item(e2, "item_0003.dat"));
}

TEST_F(filequeue_fixture, threads_race)
{
    fq::queue dst_queue = make_fq(dst_queue_dir());
    fq::queue src_queue = make_fq(src_queue_dir());
    ASSERT_TRUE(dst_queue.open());
    ASSERT_TRUE(src_queue.open());

    constexpr size_t k_num_threads = 8;
    std::vector<std::thread> workers;
    std::atomic_size_t success_count{0};

    for (size_t i = 0; i < k_num_threads; ++i) {
        workers.emplace_back([&src_queue, &dst_queue, &success_count, i]() {
            details::entry e;
            std::string name = "item_000" + std::to_string(i) + ".dat";
            if (src_queue.create_item(e, name)) {
                if (dst_queue.push_item(e)) {
                    success_count++;
                }
            }
        });
    }

    for (std::thread& t : workers) {
        if (t.joinable()) {
            t.join();
        }
    }

    EXPECT_TRUE(success_count == k_num_threads) << success_count;
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
