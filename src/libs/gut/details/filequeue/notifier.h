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

#ifndef _LIBS_GUT_DETAILS_FILEQUEUE_NOTIFIER_H_
#define _LIBS_GUT_DETAILS_FILEQUEUE_NOTIFIER_H_

#include <functional>
#include <map>
#include <string_view>

namespace wstux {
namespace gut {
namespace details {
namespace fq {

/**
 *  \brief  RAII wrapper around Linux ::inotify for monitoring filesystem directory events.
 *
 *  \details    Tracks directory modifications (e.g., IN_MOVED_TO, IN_CREATE) across
 *      multiple watched directories using a single non-blocking ::inotify file
 *      descriptor and dispatches events to registered per-watch callbacks.
 *
 *  \warning    ::notifier instances are **not thread-safe** and must be synchronized
 *      externally if accessed from multiple threads.
 */
class notifier final
{
public:
    /// \brief  Callback signature invoked for each received ::inotify event.
    /// \param  mask - bitmask of ::inotify event flags (e.g., IN_MOVED_TO, IN_CREATE).
    /// \param  name - relative filename associated with the event (may be empty for directory-level events).
    using callback_fn_t = std::function<void(uint32_t mask, const std::string_view& name)>;

public:
    /// \brief  Initializes a new non-blocking, close-on-exec ::inotify instance.
    notifier();

    /// \brief  Closes the underlying ::inotify file descriptor and removes all watches.
    ~notifier();

    notifier(const notifier&) = delete;
    notifier& operator=(const notifier&) = delete;

    /// \brief  Registers a filesystem watch on path for events specified in mask.
    /// \param  path - directory path to monitor.
    /// \param  mask - bitmask of ::inotify flags (e.g., IN_MOVED_TO, IN_CREATE).
    /// \param  cb - callback invoked when matching events occur on this watch descriptor.
    /// \return A non-negative watch descriptor on success, or -1 on error.
    int add_watch(const std::string_view& path, uint32_t mask, callback_fn_t cb);

    /// \brief  Reads and dispatches all currently pending ::inotify events without blocking.
    /// \return true if at least one event was processed; false if no events were available or on error.
    bool fetch();

    /// \brief  Waits up to wait_ms milliseconds for events, then reads and dispatches them.
    /// \param  wait_ms - maximum wait time in milliseconds (0 for non-blocking poll).
    /// \return true if events were received and dispatched; false on timeout or error.
    bool fetch(const size_t wait_ms);

    /// \brief  Unregisters an active watch descriptor and removes its associated callback.
    /// \param  wd - watch descriptor previously returned by add_watch().
    void remove_watch(int wd);

    /// \brief  Blocks in ::poll() until the ::inotify descriptor becomes readable or wait_ms elapses.
    /// \param  wait_ms - maximum wait time in milliseconds (0 for non-blocking poll).
    /// \return true if events are ready to be read via fetch(); false on timeout or signal interruption.
    bool wait(const size_t wait_ms);

private:
    /// \brief  Drains and discards all unread events from the ::inotify buffer.
    void skip_all();

private:
    int m_inotify_fd = -1;
    std::map<int, callback_fn_t> m_wd_to_callback;
};

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux

#endif /* _LIBS_GUT_DETAILS_FILEQUEUE_NOTIFIER_H_ */
