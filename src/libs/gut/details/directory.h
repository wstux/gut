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

#ifndef _LIBS_GUT_DETAILS_DIRECTORY_H_
#define _LIBS_GUT_DETAILS_DIRECTORY_H_

#include <string>
#include <string_view>

namespace wstux {
namespace gut {
namespace details {

/**
 *  \brief  RAII handle for a persistent nook stage directory.
 *
 *  \details    Manages the lifecycle and file descriptor of a top-level directory path
 *      on disk. Automatically creates missing directories on open() and provides
 *      low-level static helpers (::openat(), ::open_dir()) for descriptor-relative
 *      traversal.
 */
class directory final
{
public:
    /// \brief  Constructs a directory handle.
    /// \param  dir_path - absolute path to the directory.
    explicit directory(std::string_view dir_path);

    /// \brief  Closes the underlying directory file descriptor if open.
    ~directory();

    /// \brief  Returns the underlying file descriptor of the directory.
    /// \return Valid directory file descriptor, or -1 if not open.
    int fd() const { return m_fd; }

    /// \brief  Checks whether the directory currently holds an open file descriptor.
    /// \return true if open; false otherwise.
    bool is_open() const;

    /// \brief  Acquires a non-blocking advisory lock (::flock) on the directory.
    /// \param  is_exclusive - exclusive (LOCK_EX | LOCK_NB) or shared (LOCK_SH | LOCK_NB) lock.
    /// \return true if the lock was acquired; false on error.
    bool lock(bool is_exclusive) const;

    /// \brief  Ensures the directory exists on disk (creating it if necessary) and opens it.
    /// \return true if the directory was opened; false on error.
    bool open();

    /// \brief  Returns the stored filesystem path of this directory.
    /// \return A string view of path.
    std::string_view path() const { return m_path; }

    /// \brief  Releases any advisory lock held on the directory descriptor.
    void unlock() const;

    /// \brief  Low-level wrapper around ::openat().
    /// \param  dir_fd - open file descriptor of the parent directory (or AT_FDCWD).
    /// \param  dir_name - relative name of the target entry.
    /// \param  flags - open flags.
    /// \return Opened file descriptor on success, or -1 on error.
    static int openat(int dir_fd, const std::string_view& dir_name, int flags);

    /// \brief  Opens a subdirectory relative to parent descriptor with standard directory flags.
    /// \param  fd - output reference populated with the newly opened descriptor on success.
    /// \param  parent_fd - open file descriptor of the parent directory.
    /// \param  dir_name - name of the subdirectory to open.
    /// \return true if the subdirectory was opened; false on error.
    static bool open_dir(int& fd, const int parent_fd, const std::string_view& dir_name);

private:
    std::string m_path;
    int m_fd = -1;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_DIRECTORY_H_ */
