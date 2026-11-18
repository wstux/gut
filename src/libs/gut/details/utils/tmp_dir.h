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

#ifndef _LIBS_GUT_DETAILS_UTILS_TMP_DIR_H_
#define _LIBS_GUT_DETAILS_UTILS_TMP_DIR_H_

#include <dirent.h>

#include <memory>
#include <string_view>

#include "gut/details/fwd.h"

namespace wstux {
namespace gut {
namespace details {

/**
 *  \brief  RAII handle representing a rotated temporary subdirectory inside a queue stage.
 *
 *  \details    Encapsulates a directory entry with a fixed-length Base64 name opened
 *      relative to a parent directory file descriptor (::openat). Holds an optional
 *      pointer to its parent to guarantee that parent file descriptors remain valid
 *      throughout the lifetime of nested child directories.
 *
 *      Supports non-blocking POSIX advisory locking (::flock) and atomic removal via ::unlinkat.
 */
class tmp_dir final
{
public:
    using ptr = std::shared_ptr<tmp_dir>;

public:
    /// \brief  Creates a new temporary directory descriptor relative to a parent directory descriptor.
    /// \param  parent_fd - open file descriptor of the parent directory (must remain valid until opened).
    explicit tmp_dir(int parent_fd);

    /// \brief  Creates a new temporary directory descriptor relative to a managed parent directory.
    /// \param  p_parent - the parent directory (keeps parent descriptor alive).
    explicit tmp_dir(const ptr& p_parent);

    /// \brief  Creates a new temporary directory descriptor relative to a parent directory descriptor.
    /// \param  name - directory name.
    /// \param  parent_fd - open file descriptor of the parent directory (must remain valid until opened).
    tmp_dir(const std::string_view& name, int parent_fd);

    /// \brief  Creates a new temporary directory descriptor relative to a managed parent directory.
    /// \param  name - directory name.
    /// \param  p_parent - the parent directory (keeps parent descriptor alive).
    tmp_dir(const std::string_view& name, const ptr& p_parent);

    /// \brief  Closes the underlying directory file descriptor if open and releases the parent reference.
    ~tmp_dir();

    /// \brief  Closes the directory file descriptor and resets it to -1.
    void close();

    /// \brief  Creates the directory on disk relative to its parent (::mkdirat) and opens it.
    /// \return true if the directory was created and opened; false on error.
    bool create();

    /// \brief  Returns the underlying POSIX file descriptor of the opened directory.
    /// \return Valid directory file descriptor, or -1 if not open.
    int fd() const { return m_fd; }

    /// \brief  Checks whether the directory currently holds an open file descriptor.
    /// \return true if directory file descriptor more then -1; false otherwise.
    bool is_open() const;

    /// \brief  Acquires a non-blocking advisory lock (::flock) on the directory.
    /// \param  is_exclusive - exclusive (LOCK_EX | LOCK_NB) or shared (LOCK_SH | LOCK_NB) lock.
    /// \return true if the lock was acquired; false on error.
    bool lock(bool is_exclusive) const;

    /// \brief  Queries the last modification timestamp of the directory via ::fstat64().
    /// \return Last modification time in seconds since the Epoch, or 0 on error.
    ::time_t mtime() const;

    /// \brief  Returns the fixed-length Base64 name of this directory.
    /// \return A string view of name.
    std::string_view name() const { return m_name; }

    /// \brief  Opens the existing directory on disk relative to its parent via ::openat().
    /// \return true if opened successfully; false on error.
    bool open();

    /// \brief  Unlinks the empty directory from its parent via ::unlinkat.
    /// \return true if removed or already absent (ENOENT); false on error.
    bool remove();

    /// \brief  Returns the current metadata size of the directory in bytes via ::fstat64().
    /// \return Directory size in bytes, or 0 on error.
    size_t size() const;

    /// \brief  Releases any advisory lock held on the directory descriptor.
    void unlock() const;

private:
    const ptr m_p_parent;
    const int m_parent_fd = -1;

    int m_fd = -1;
    char m_name[gk_name_buffer_size];
};

/**
 *  \brief  Movable RAII wrapper around POSIX directory streams.
 *
 *  \details    Provides safe, leak-free iteration over directory entries using ::opendir(),
 *      ::fdopendir(), and ::readdir().
 */
class dir_reader final
{
public:
    /// \brief  Constructs an unopened directory reader.
    dir_reader() = default;

    dir_reader(const dir_reader&) = delete;
    dir_reader& operator=(const dir_reader&) = delete;

    /// \brief  Move-constructs a reader, transferring ownership of the underlying directory stream.
    /// \param  other - source reader (left in an unopened state).
    dir_reader(dir_reader&& other) noexcept;

    /// \brief  Move-assigns a reader, closing any currently open stream before taking ownership.
    /// \param  other - source reader (left in an unopened state).
    /// \return Reference to this.
    dir_reader& operator=(dir_reader&& other) noexcept;

    /// \brief  Closes the underlying directory stream if open.
    ~dir_reader() { close(); }

    /// \brief  Closes the active directory stream.
    void close() noexcept;

    /// \brief  Returns the underlying file descriptor associated with the open directory stream.
    /// \return Directory file descriptor via ::dirfd(), or -1 if not opened.
    int dirfd() const { return m_p_scanner ? ::dirfd(m_p_scanner) : -1; }

    /// \brief  Checks whether a directory stream is currently open.
    /// \return true if open; false otherwise.
    bool is_opened() const { return m_p_scanner != nullptr; }

    /// \brief  Opens a directory stream by filesystem path.
    /// \param  p_path - null-terminated path to the directory (must not be nullptr).
    /// \return true if opened successfully; false on error.
    bool opendir(const char* p_path) noexcept;

    /// \brief  Opens a directory stream from an existing open directory file descriptor.
    /// \param  fd - valid open directory file descriptor.
    /// \return true if opened successfully; false on error.
    bool opendir(int fd) noexcept;

    /// \brief  Opens a directory stream from an existing open directory.
    /// \param  p_parent - an open directory instance.
    /// \return true if opened successfully; false on error.
    bool opendir(const tmp_dir::ptr& p_parent) noexcept { return opendir(p_parent->fd()); }

    /// \brief  Reads the next entry from the open directory stream.
    /// \return Pointer to the next ::dirent, or nullptr on end-of-stream or error.
    struct ::dirent* readdir() const noexcept;

    /// \brief  Resets the position of the directory stream back to the beginning.
    void rewinddir() const noexcept;

private:
    DIR* m_p_scanner = nullptr;
};

} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_UTILS_TMP_DIR_H_ */
