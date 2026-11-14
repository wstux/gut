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
#include <sys/file.h>

#include <filesystem>
#include <random>

#include <gtest/gtest.h>

#include "gut/details/dir.h"
#include "gut/details/retry_eintr.h"
#include "gut/details/filequeue/cleaner.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;
namespace fq = gut::details::fq;

class cleaner_fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_root_path = make_tmp_dir("ut_gut_cleaner");
        ASSERT_TRUE(std::filesystem::create_directory(m_root_path)) << "Failed to create root directory '" << m_root_path << "'";

        m_p_work_dir = std::make_shared<details::dir_t>(m_root_path.string());
        ASSERT_TRUE(m_p_work_dir->open());
    }

    void TearDown() override
    {
        std::filesystem::remove_all(m_root_path);
    }

    details::tmp_dir::ptr create_dir(const std::string& name)
    {
        details::tmp_dir::ptr p_dir;
        create_dir(p_dir, name);
        return p_dir;
    }

    int create_file(const details::tmp_dir::ptr& p_dir, const std::string& filename)
    {
        int fd = -1;
        create_file(fd, p_dir, filename);
        return fd;
    }

    bool exists(const details::tmp_dir::ptr& p_dir)
    {
        int fd = ::openat(m_p_work_dir->fd(), p_dir->name().data(), O_PATH | O_NOFOLLOW | O_CLOEXEC);

        if (fd < 0) {
            return (errno != ENOENT);
        }
        ::close(fd);
        return true;
    }

    std::filesystem::path full_path(const details::tmp_dir::ptr& p_dir)
    {
        return m_root_path / p_dir->name();
    }

    void remove_file(const details::tmp_dir::ptr& p_dir, const std::string& filename)
    {
        std::filesystem::remove(m_root_path / p_dir->name() / filename);
    }

private:
    void create_dir(details::tmp_dir::ptr& p_dir, const std::string& name)
    {
        std::filesystem::path path = m_root_path / name;
        ASSERT_TRUE(std::filesystem::create_directories(path)) << "Failed to create directory '" << path << "'";

        p_dir = std::make_shared<details::tmp_dir>(name, m_p_work_dir->fd());
    }

    void create_file(int& file_fd, const details::tmp_dir::ptr& p_dir, const std::string& filename)
    {
        ASSERT_TRUE(p_dir->open());
        file_fd = ::openat(p_dir->fd(), filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        ASSERT_TRUE(file_fd >= 0);
        p_dir->close();

        const ssize_t rc = ::write(file_fd, "test_data", 9);
        ASSERT_TRUE(rc == 9);
    }

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
    std::shared_ptr<details::dir_t> m_p_work_dir;
};

} // <anonymous> namespace

TEST_F(cleaner_fixture, remove_dir)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    details::tmp_dir::ptr p_dir = create_dir("000000001");

    EXPECT_TRUE(cleaner.push(p_dir));
    EXPECT_TRUE(cleaner.step(0, 0));
    EXPECT_FALSE(exists(p_dir));
}

TEST_F(cleaner_fixture, non_empty_dir)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    details::tmp_dir::ptr p_dir = create_dir("000000002");

    int file_fd = create_file(p_dir, "randitem2.dat");
    ::close(file_fd);

    EXPECT_TRUE(cleaner.push(p_dir));
    EXPECT_TRUE(cleaner.step(0, 0));

    const std::filesystem::path file_path = full_path(p_dir) / "randitem2.dat";
    EXPECT_FALSE(std::filesystem::exists(file_path));
    EXPECT_FALSE(exists(p_dir));
}

TEST_F(cleaner_fixture, DISABLED_locked_file)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    details::tmp_dir::ptr p_dir = create_dir("000000003");

    int file_fd = create_file(p_dir, "randitem3.dat");
    ASSERT_TRUE(__GUT_LOCK_FD(file_fd, true));

    EXPECT_TRUE(cleaner.push(p_dir));
    EXPECT_FALSE(cleaner.step(0, 0));
    __GUT_UNLOCK_FD(file_fd);
    ::close(file_fd);

    const std::filesystem::path file_path = full_path(p_dir) / "randitem2.dat";
    EXPECT_TRUE(std::filesystem::exists(file_path));
}

TEST_F(cleaner_fixture, file_timeout_to_remove)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    details::tmp_dir::ptr p_dir = create_dir("000000004");

    int file_fd = create_file(p_dir, "randitem4.dat");
    ::close(file_fd);

    cleaner.push(p_dir);

    EXPECT_FALSE(cleaner.step(0, 10));

    const std::filesystem::path file_path = full_path(p_dir) / "randitem4.dat";
    EXPECT_TRUE(std::filesystem::exists(file_path));
    EXPECT_TRUE(exists(p_dir));
}

TEST_F(cleaner_fixture, dir_timeout_to_remove)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    details::tmp_dir::ptr p_dir = create_dir("000000005");

    cleaner.push(p_dir);

    EXPECT_FALSE(cleaner.step(5, 0));
    EXPECT_TRUE(std::filesystem::exists(full_path(p_dir)));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
