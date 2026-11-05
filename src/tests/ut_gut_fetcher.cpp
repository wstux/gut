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

#include <fcntl.h>
#include <unistd.h>

#include <chrono>
#include <filesystem>
#include <random>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "gut/details/dir.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/filequeue/fetcher.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;
namespace fq = gut::details::fq;

class fetcher_fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_root_path = make_tmp_dir("ut_gut_fetcher");
        ASSERT_TRUE(std::filesystem::create_directory(m_root_path)) << "Failed to create root directory '" << m_root_path << "'";

        m_tmp_path = m_root_path / "tmp";
        ASSERT_TRUE(std::filesystem::create_directory(m_tmp_path)) << "Failed to create tmp directory '" << m_tmp_path << "'";

        m_work_path = m_root_path / "queue";
        ASSERT_TRUE(std::filesystem::create_directory(m_work_path)) << "Failed to create work directory '" << m_work_path << "'";

        m_p_work_dir = std::make_shared<details::dir_t>(m_work_path.string());
        ASSERT_TRUE(m_p_work_dir->open());
        m_p_cleaner = std::make_shared<details::fq::cleaner>([]() -> bool { return false; });
    }

    void TearDown() override
    {
        std::filesystem::remove_all(m_root_path);
    }

    bool cleaner_step()
    {
        return m_p_cleaner->step(0, 0);
    }

    void create_dirs(const std::string& primary_dir, const std::string& active_dir)
    {
        std::filesystem::path path = m_work_path / primary_dir / active_dir;
        ASSERT_TRUE(std::filesystem::create_directories(path)) << "Failed to create directory '" << path << "'";
    }

    fq::fetcher make_fetcher()
    {
        return fq::fetcher(*m_p_work_dir, m_p_cleaner, []() { return false; });
    }

    details::tmp_dir::ptr make_tmp_dir(const std::string& dir_name, const int parent_fd) const
    {
        details::tmp_dir::ptr p_dir = std::make_shared<details::tmp_dir>(dir_name.c_str(), parent_fd);
        p_dir->open();
        return p_dir;
    }

    void move_file(const std::string& primary_dir, const std::string& active_dir, const std::string& filename)
    {
        std::filesystem::path tmp_path = m_tmp_path / filename;
        int fd = ::open(tmp_path.string().c_str(), O_CREAT | O_WRONLY, 0644);
        ASSERT_TRUE(fd >= 0);
        const ssize_t rc = ::write(fd, "test_data", 9);
        ASSERT_TRUE(rc == 9);
        ::close(fd);

        std::filesystem::path target_path = m_work_path / primary_dir / active_dir / filename;
        ::rename(tmp_path.string().c_str(), target_path.string().c_str());
    }

    void remove_file(const std::string& primary_dir, const std::string& active_dir, const std::string& filename)
    {
        std::filesystem::remove(m_work_path / primary_dir / active_dir / filename);
    }

    const details::dir_t& work_dir() const { return *m_p_work_dir; }

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
    std::filesystem::path m_tmp_path;
    std::filesystem::path m_work_path;
    std::shared_ptr<details::dir_t> m_p_work_dir;
    details::fq::cleaner::ptr m_p_cleaner;
};

} // <anonymous> namespace

TEST_F(fetcher_fixture, scan_existing_files)
{
    create_dirs("000000001", "000000002");
    move_file("000000001", "000000002", "randitem1.dat");

    fq::fetcher fetcher = make_fetcher();

    std::string filename;
    details::tmp_dir::ptr p_out_dir;

    EXPECT_TRUE(fetcher.fetch(filename, p_out_dir));
    EXPECT_TRUE(filename == "randitem1.dat") << filename;
    ASSERT_TRUE(p_out_dir.get() != nullptr);
    EXPECT_TRUE(p_out_dir->name() == "000000002") << p_out_dir->name();
}

TEST_F(fetcher_fixture, empty_queue)
{
    fq::fetcher fetcher = make_fetcher();

    std::string filename;
    details::tmp_dir::ptr p_out_dir;

    EXPECT_FALSE(fetcher.fetch(filename, p_out_dir));
    EXPECT_TRUE(filename.empty());
    EXPECT_TRUE(p_out_dir.get() == nullptr);
}

TEST_F(fetcher_fixture, detects_subdirs)
{
    fq::fetcher fetcher = make_fetcher();

    std::thread writer_thread([this]() {
        create_dirs("000000003", "000000004");
        move_file("000000003", "000000004", "randitem2.dat");
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::string filename;
    details::tmp_dir::ptr p_out_dir;

    EXPECT_TRUE(fetcher.fetch(filename, p_out_dir));
    EXPECT_TRUE(filename == "randitem2.dat") << filename;
    writer_thread.join();
}

TEST_F(fetcher_fixture, add_item_to_queue_head)
{
    create_dirs("000000005", "000000006");
    fq::fetcher fetcher = make_fetcher();

    std::string filename;
    details::tmp_dir::ptr p_out_dir;

    EXPECT_FALSE(fetcher.fetch(filename, p_out_dir));

    std::thread writer_thread([this]() {
        move_file("000000005", "000000006", "randitem3.dat");
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(fetcher.fetch(filename, p_out_dir));
    EXPECT_TRUE(filename == "randitem3.dat") << filename;
    writer_thread.join();
}

TEST_F(fetcher_fixture, evicts_to_cleaner)
{
    create_dirs("000000007", "000000008");
    create_dirs("000000007", "000000009");
    move_file("000000007", "000000008", "randitem4.dat");

    fq::fetcher fetcher = make_fetcher();

    std::string filename;
    details::tmp_dir::ptr p_out_dir;

    EXPECT_TRUE(fetcher.fetch(filename, p_out_dir));
    EXPECT_TRUE(filename == "randitem4.dat") << filename;

    remove_file("000000007", "000000008", "randitem4.dat");
    EXPECT_FALSE(fetcher.fetch(filename, p_out_dir));
    EXPECT_TRUE(cleaner_step());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
