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

#include "gut/details/directory.h"
#include "gut/details/retry_eintr.h"
#include "gut/details/filequeue/cleaner.h"
#include "gut/details/utils/tmp_dir.h"

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

        m_p_work_dir = std::make_shared<details::directory>(m_root_path.string());
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

    void create_file(const std::filesystem::path& path)
    {
        if (! exists_path(path.parent_path())) {
            ASSERT_TRUE(std::filesystem::create_directories(path.parent_path())) << errno;
        }

        int fd = ::open(path.c_str(), O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, 0644);
        ASSERT_TRUE(fd > 0) << "Failed to create file: " << path;
        const ssize_t rc = ::write(fd, "test_data", 9);
        ASSERT_TRUE(rc == 9);
        ::close(fd);
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

    bool exists(const std::string_view& dir_name)
    {
        int fd = ::openat(m_p_work_dir->fd(), dir_name.data(), O_PATH | O_NOFOLLOW | O_CLOEXEC);
        if (fd < 0) {
            return (errno != ENOENT);
        }
        ::close(fd);
        return true;
    }

    bool exists_path(const std::filesystem::path& path)
    {
        int fd = ::open(path.c_str(), O_PATH | O_NOFOLLOW | O_CLOEXEC);
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

    std::filesystem::path full_path(const std::string_view& dir_name)
    {
        return m_root_path / dir_name;
    }

    void remove_file(const details::tmp_dir::ptr& p_dir, const std::string& filename)
    {
        std::filesystem::remove(m_root_path / p_dir->name() / filename);
    }

    std::filesystem::path root_path() const { return m_root_path; }

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
    std::shared_ptr<details::directory> m_p_work_dir;
};

} // <anonymous> namespace

TEST_F(cleaner_fixture, remove_dir)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    const std::string dir_name = "000000001";
    //details::tmp_dir::ptr p_dir = create_dir("000000001");

    EXPECT_TRUE(cleaner.push(create_dir("000000001")));
    EXPECT_TRUE(cleaner.step(0, 0));
    EXPECT_FALSE(exists(dir_name));
}

TEST_F(cleaner_fixture, non_empty_dir)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    const std::string dir_name = "000000002";
    details::tmp_dir::ptr p_dir = create_dir(dir_name);

    int file_fd = create_file(p_dir, "randitem2.dat");
    ::close(file_fd);

    EXPECT_TRUE(cleaner.push(p_dir));
    p_dir.reset();
    EXPECT_TRUE(cleaner.step(0, 0));

    const std::filesystem::path file_path = full_path(dir_name) / "randitem2.dat";
    EXPECT_FALSE(std::filesystem::exists(file_path));
    EXPECT_FALSE(exists(dir_name));
}

TEST_F(cleaner_fixture, locked_file)
{
    fq::cleaner cleaner([]() -> bool { return false; });
    details::tmp_dir::ptr p_dir = create_dir("000000003");

    int file_fd = create_file(p_dir, "randitem3.dat");
    ASSERT_TRUE(__GUT_LOCK_FD(file_fd, true));

    EXPECT_TRUE(cleaner.push(p_dir));
    EXPECT_FALSE(cleaner.step(0, 0));
    __GUT_UNLOCK_FD(file_fd);
    ::close(file_fd);

    const std::filesystem::path file_path = full_path(p_dir) / "randitem3.dat";
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

TEST_F(cleaner_fixture, cleanup_with_saving_files)
{
    const std::filesystem::path valid_file = root_path() / "000000001/000000002/item_0001.dat";
    create_file(valid_file);

    const std::filesystem::path empty_branch = root_path() / "000000003/000000004";
    ASSERT_TRUE(std::filesystem::create_directories(empty_branch));

    EXPECT_TRUE(fq::cleaner::cleanup(root_path().string(), 0, 0, false, []() -> bool { return false; }));
    EXPECT_TRUE(exists("000000001/000000002"));
    EXPECT_TRUE(exists("000000001/000000002/item_0001.dat"));
    EXPECT_FALSE(exists("000000003"));
}

TEST_F(cleaner_fixture, cleanup_partial_primary_dir)
{
    const std::filesystem::path active_with_data = root_path() / "000000010/000000011/item_0002.dat";
    const std::filesystem::path empty_active = root_path() / "000000010/000000012";

    create_file(active_with_data);
    std::filesystem::create_directories(empty_active);

    EXPECT_TRUE(fq::cleaner::cleanup(root_path().string(), 0, 0, false, []() -> bool { return false; }));
    EXPECT_TRUE(exists("000000010"));
    EXPECT_TRUE(exists("000000010/000000011"));
    EXPECT_TRUE(exists("000000010/000000011/item_0002.dat"));
    EXPECT_FALSE(exists("000000010/000000012"));
}

TEST_F(cleaner_fixture, cleanup_removes_files)
{
    const std::filesystem::path abandoned_file = root_path() / "000000020/000000021/item_0003.dat";
    create_file(abandoned_file);

    EXPECT_TRUE(fq::cleaner::cleanup(root_path().string(), 0, 0, true, []() -> bool { return false; }));

    EXPECT_FALSE(std::filesystem::exists(abandoned_file));
    EXPECT_FALSE(exists("000000020"));
}

TEST_F(cleaner_fixture, cleanup_skips_locked_and_fresh_files)
{
    const std::filesystem::path locked_dir = root_path() / "000000030/000000031/item_0004.dat";
    const std::filesystem::path fresh_dir = root_path() / "000000030/000000032/item_0005.dat";

    create_file(locked_dir);
    create_file(fresh_dir);

    const int lock_fd = ::open(locked_dir.c_str(), O_RDONLY | O_CLOEXEC);
    ASSERT_TRUE(lock_fd > 0);
    ASSERT_TRUE(__GUT_LOCK_FD(lock_fd, true));

    EXPECT_FALSE(fq::cleaner::cleanup(root_path().string(), 0, 60, true, []() -> bool { return false; }));
    EXPECT_TRUE(exists("000000030/000000031/item_0004.dat"));
    EXPECT_TRUE(exists("000000030/000000032/item_0005.dat"));
    EXPECT_TRUE(exists("000000030"));
    EXPECT_TRUE(exists("000000030/000000031"));
    EXPECT_TRUE(exists("000000030/000000032"));

    __GUT_UNLOCK_FD(lock_fd);
    ::close(lock_fd);
}

TEST_F(cleaner_fixture, cleanup_garbage)
{
    create_file(root_path() / "bad_dir/nested/invalid.dat");
    create_file(root_path() / "000000040/too_long_directory_name/item_0006.dat");
    create_file(root_path() / "000000040/000000041/invalid.txt");
    create_file(root_path() / "stray_root_file.tmp");

    EXPECT_TRUE(fq::cleaner::cleanup(root_path().string(), 0, 0, false, []() -> bool { return false; }));
    EXPECT_FALSE(exists("bad_dir"));
    EXPECT_FALSE(exists("000000040/too_long_directory_name"));
    EXPECT_FALSE(exists("000000040/000000041/invalid.txt"));
    EXPECT_FALSE(exists("stray_root_file.tmp"));
    EXPECT_FALSE(exists("000000040"));
}

TEST_F(cleaner_fixture, cleanup_locked_directory)
{
    const std::filesystem::path active_dir = root_path() / "000000050/000000051";
    ASSERT_TRUE(std::filesystem::create_directories(active_dir));

    int dir_fd = ::open(active_dir.c_str(), O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    ASSERT_TRUE(dir_fd > 0);
    ASSERT_TRUE(__GUT_LOCK_FD(dir_fd, false));

    EXPECT_TRUE(fq::cleaner::cleanup(root_path().string(), 0, 0, false, []() -> bool { return false; }));
    EXPECT_TRUE(exists("000000050/000000051"));

    __GUT_UNLOCK_FD(dir_fd);
    ::close(dir_fd);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
