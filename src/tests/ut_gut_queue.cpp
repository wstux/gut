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
#include <filesystem>
#include <functional>
#include <random>
#include <thread>

#include <gtest/gtest.h>

#include "gut/queue.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;
namespace fq = gut::details::fq;

static constexpr size_t k_write_threads = 4;
static constexpr size_t k_read_threads = 2;
static constexpr size_t k_items_per_writer = 1000;

class queue_fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_root_path = make_tmp_dir("ut_gut_queue");
        ASSERT_TRUE(std::filesystem::create_directory(m_root_path)) << "Failed to create root directory '" << m_root_path << "'";
    }

    void TearDown() override
    {
        std::filesystem::remove_all(m_root_path);
    }

    const std::filesystem::path& root_path() const { return m_root_path; }

    void join_read_workers()
    {
        for (size_t i = k_write_threads; i < m_workers.size(); ++i) {
            m_workers[i].join();
        }
    }

    void join_write_workers()
    {
        for (size_t i = 0; i < k_write_threads; ++i) {
            m_workers[i].join();
        }
    }

    void start_read_workers(const std::function<void()>& fn)
    {
        for (size_t t = 0; t < k_read_threads; ++t) {
            m_workers.emplace_back(fn);
        }
    }

    void start_write_workers(const std::function<void()>& fn)
    {
        for (size_t t = 0; t < k_write_threads; ++t) {
            m_workers.emplace_back(fn);
        }
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
    std::vector<std::thread> m_workers;
};

} // <anonymous> namespace

TEST_F(queue_fixture, create_item)
{
    std::filesystem::path rqueue_dir = root_path() / "create_ritem";
    std::filesystem::path wqueue_dir = root_path() / "create_witem";
    gut::queue::ptr rq = gut::queue::create_queue(rqueue_dir.string(), []() -> bool { return false; });
    gut::queue::ptr wq = gut::queue::create_queue(wqueue_dir.string(), rq->path(), []() -> bool { return false; });

    gut::item::ptr item1 = wq->create();
    ASSERT_TRUE(item1);
    EXPECT_FALSE(wq->pop(0));

    EXPECT_TRUE(item1->commit());

    ASSERT_TRUE(rq->pop(0));
    EXPECT_FALSE(rq->pop(0));
}

TEST_F(queue_fixture, DISABLED_rollback)
{
    std::filesystem::path rqueue_dir = root_path() / "rollback_ritem";
    std::filesystem::path wqueue_dir = root_path() / "rollback_witem";
    gut::queue::ptr rq = gut::queue::create_queue(rqueue_dir.string(), []() -> bool { return false; });
    gut::queue::ptr wq = gut::queue::create_queue(wqueue_dir.string(), rq->path(), []() -> bool { return false; });

    gut::item::ptr item = wq->create();
    ASSERT_TRUE(item);
    EXPECT_TRUE(item->commit());

    gut::item::ptr ritem = rq->pop(0);
    ASSERT_TRUE(ritem);
    EXPECT_FALSE(rq->pop(0));
    ASSERT_TRUE(ritem->rollback());
    EXPECT_TRUE(rq->pop(0));
}

TEST_F(queue_fixture, DISABLED_create_item_multithreaded)
{
    std::filesystem::path rqueue_dir = root_path() / "create_ritem_multithreaded";
    std::filesystem::path wqueue_dir = root_path() / "create_witem_multithreaded";
    gut::queue::ptr rq = gut::queue::create_queue(rqueue_dir.string(), []() -> bool { return false; });
    gut::queue::ptr wq = gut::queue::create_queue(wqueue_dir.string(), rq->path(), []() -> bool { return false; });

    std::atomic<size_t> total_written{0};
    std::atomic<size_t> total_read{0};
    std::atomic<bool> writers_done{false};

    const std::function<void()> read_fn = [&rq, &total_read, &writers_done]() {
            while (! writers_done.load(std::memory_order_acquire) || rq->pop(0) != nullptr) {
                if (gut::item::ptr item = rq->pop(10)) {
                    if (item->commit()) {
                        total_read.fetch_add(1, std::memory_order_relaxed);
                    }
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
            while (gut::item::ptr item = rq->pop(0)) {
                if (item->commit()) {
                    total_read.fetch_add(1, std::memory_order_relaxed);
                }
            }
        };

    const std::function<void()> write_fn = [&wq, &total_written]() {
            for (size_t i = 0; i < k_items_per_writer; ++i) {
                if (gut::item::ptr item = wq->create()) {
                    if (item->commit()) {
                        total_written.fetch_add(1, std::memory_order_relaxed);
                    }
                }
            }
        };

    start_write_workers(write_fn);
    start_read_workers(read_fn);

    join_write_workers();
    writers_done.store(true, std::memory_order_release);
    join_read_workers();

    const size_t expected = k_write_threads * k_items_per_writer;
    EXPECT_EQ(total_written.load(), expected);
    EXPECT_EQ(total_read.load(), expected);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
