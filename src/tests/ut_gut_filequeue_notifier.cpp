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

#include <linux/limits.h>
#include <sys/inotify.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "gut/details/filequeue/notifier.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details::fq;

class notifier_fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
        std::random_device rd;
        std::mt19937 prng(rd());
        std::uniform_int_distribution<uint32_t> rand_dist;
        m_test_dir = temp_dir / ("ut_gut_filequeue_notifier_" + std::to_string(rand_dist(prng)));

        ASSERT_TRUE(std::filesystem::create_directories(m_test_dir));
    }

    void TearDown() override { std::filesystem::remove_all(m_test_dir); }

    const char* test_dir() const { return m_test_dir.c_str(); }

private:
    std::filesystem::path m_test_dir;
};

} // <anonymous> namespace

TEST_F(notifier_fixture, add_remove_watch)
{
    details::notifier n;
    const int wd = n.add_watch(test_dir(), IN_CREATE | IN_MOVED_TO, [](uint32_t, const std::string_view&) {});
    ASSERT_TRUE(wd > 0) << "Failed to add watch, errno: " << errno << " (path: " << test_dir() << ")";
    EXPECT_NO_THROW(n.remove_watch(wd));
}

TEST_F(notifier_fixture, non_existent_path)
{
    details::notifier n;
    std::filesystem::path path = test_dir();
    path /= "non_existent_dir";

    int wd = n.add_watch(path.c_str(), IN_CREATE, [](uint32_t, const std::string_view&) {});
    EXPECT_TRUE(wd == -1);
    EXPECT_TRUE(errno == ENOENT);
}

TEST_F(notifier_fixture, file_creation)
{
    details::notifier n;

    std::string captured_name;
    uint32_t captured_mask = 0;
    bool callback_triggered = false;

    int wd = n.add_watch(test_dir(), IN_CREATE,
        [&](uint32_t mask, const std::string_view& name) {
            if (! name.empty()) {
                callback_triggered = true;
                captured_mask = mask;
                captured_name = name;
            }
        });
    ASSERT_TRUE(wd > 0);

    std::string target_filename = "test_message.dat";
    {
        std::filesystem::path path = test_dir();
        std::ofstream file(path / target_filename);
        file << "test_data";
    }

    EXPECT_TRUE(n.fetch(200));

    EXPECT_TRUE(callback_triggered) << "Notifier failed to capture file creation event within timeout";
    EXPECT_TRUE(captured_name == target_filename);
    EXPECT_TRUE(captured_mask & IN_CREATE);

    n.remove_watch(wd);
}

TEST_F(notifier_fixture, file_creation_fetch)
{
    details::notifier n;

    std::string captured_name;
    uint32_t captured_mask = 0;
    bool callback_triggered = false;

    int wd = n.add_watch(test_dir(), IN_CREATE,
        [&](uint32_t mask, const std::string_view& name) {
            if (! name.empty()) {
                callback_triggered = true;
                captured_mask = mask;
                captured_name = name;
            }
        });
    ASSERT_TRUE(wd > 0);

    std::string target_filename = "test_message.dat";
    {
        std::filesystem::path path = test_dir();
        std::ofstream file(path / target_filename);
        file << "test_data";
    }

    EXPECT_TRUE(n.fetch());

    EXPECT_TRUE(callback_triggered) << "Notifier failed to capture file creation event within timeout";
    EXPECT_TRUE(captured_name == target_filename);
    EXPECT_TRUE(captured_mask & IN_CREATE);

    n.remove_watch(wd);
}

TEST_F(notifier_fixture, events_timeout)
{
    details::notifier n;
    const int wd = n.add_watch(test_dir(), IN_CREATE, [](uint32_t, const std::string_view&) {});
    ASSERT_TRUE(wd > 0);

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    ASSERT_FALSE(n.fetch(50));
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration::rep duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    EXPECT_TRUE(duration > 45);
    n.remove_watch(wd);
}

TEST_F(notifier_fixture, wait)
{
    details::notifier n;
    const int wd = n.add_watch(test_dir(), IN_CREATE, [](uint32_t, const std::string_view&) {});
    ASSERT_TRUE(wd > 0);

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    ASSERT_FALSE(n.wait(50));

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration::rep duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    EXPECT_TRUE(duration > 45);
    n.remove_watch(wd);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
