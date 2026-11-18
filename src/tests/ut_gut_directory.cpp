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
#include "gut/details/utils/tmp_dir.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;

class directory_fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        m_root_path = make_tmp_dir("ut_gut_directory");
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

    std::filesystem::path full_path(const std::string_view& dir_name)
    {
        return m_root_path / dir_name;
    }

    int parent_fd() const { return m_p_work_dir->fd(); }

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

TEST_F(directory_fixture, tmp_dir_invalid_size)
{
    details::tmp_dir::ptr p_dir = create_dir("dir_00001");
    ASSERT_FALSE(p_dir->is_open());
    ASSERT_TRUE(p_dir->size() == static_cast<size_t>(-1));
}

TEST_F(directory_fixture, dir_reader_double_open)
{
    details::tmp_dir::ptr p_dir = create_dir("dir_00002");
    ASSERT_TRUE(p_dir->open());

    details::dir_reader scanner;
    ASSERT_TRUE(scanner.opendir(p_dir->fd()));
    ASSERT_FALSE(scanner.opendir(p_dir->fd()));
}

TEST_F(directory_fixture, dir_reader_invalid_readdir)
{
    details::dir_reader scanner;
    ASSERT_TRUE(scanner.readdir() == nullptr);
}

TEST_F(directory_fixture, directory_opendir_opened_fd)
{
    int fd = 2;
    ASSERT_TRUE(details::directory::open_dir(fd, parent_fd(), "test_dir"));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
