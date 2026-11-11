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

#include <poll.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>

#include <cassert>
#include <array>

#include "gut/details/retry_eintr.h"
#include "gut/details/filequeue/notifier.h"

namespace wstux {
namespace gut {
namespace details {
namespace fq {

notifier::notifier()
{
    m_inotify_fd = RETRY_EINTR(::inotify_init1(IN_CLOEXEC | IN_NONBLOCK));
    assert(m_inotify_fd >= 0);
}

notifier::~notifier()
{
    if (m_inotify_fd >= 0) {
        ::close(m_inotify_fd);
        m_inotify_fd = -1;
    }
}

int notifier::add_watch(const std::string_view& path, uint32_t mask, callback_fn_t cb)
{
    if (m_inotify_fd < 0 || path.empty()) {
        return -1;
    }

    const int wd = RETRY_EINTR(::inotify_add_watch(m_inotify_fd, path.data(), mask));
    if (wd >= 0) {
        m_wd_to_callback.emplace(wd, std::move(cb));
    }
    return wd;
}

bool notifier::fetch()
{
    if (m_inotify_fd < 0) {
        return false;
    }

    constexpr size_t k_events_buffer_size = 64 * 1024;
    alignas(struct ::inotify_event) std::array<char, k_events_buffer_size> events_buffer;
    while (true) {
        const ssize_t length = ::read(m_inotify_fd, events_buffer.data(), events_buffer.size());
        if (length < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            if (errno == EINTR) {
                continue;
            }
            skip_all();
            return true;
        }

        if (length == 0) {
            return false;
        }

        for (char* i = events_buffer.data(); i < events_buffer.data() + length; ) {
            struct ::inotify_event* p_event = reinterpret_cast<struct inotify_event*>(i);
            if (p_event->mask & IN_Q_OVERFLOW) {
                skip_all();
                return true;
            }

            if (p_event->wd != -1) {
                std::map<int, callback_fn_t>::iterator it = m_wd_to_callback.find(p_event->wd);
                if (it != m_wd_to_callback.end() && it->second) {
                    it->second(p_event->mask, std::string_view(p_event->name));
                }
            } else {
                skip_all();
            }
            i += __builtin_offsetof(struct inotify_event, name) + p_event->len;
        }
    }
    return true;
}

bool notifier::fetch(const size_t wait_ms)
{
    if (m_inotify_fd < 0) {
        return false;
    }

    do {
        int events_count = -1;
        if (RETRY_EINTR(::ioctl(m_inotify_fd, FIONREAD, &events_count)) != 0) {
            return false;
        }

        if (events_count >= (int)sizeof(struct ::inotify_event)) {
            break;
        }

        assert(events_count == 0);
        if (wait_ms == 0) {
            return false;
        }

        struct ::pollfd wt;
        wt.fd = m_inotify_fd;
        wt.events = POLLIN | POLLPRI;
        if (::poll(&wt, 1, wait_ms) < 1) {
            return false;
        }
    } while (false);

    constexpr size_t k_events_buffer_size = 64 * 1024;
    alignas(struct ::inotify_event) std::array<char, k_events_buffer_size> events_buffer;
    while (true) {
        const ssize_t length = ::read(m_inotify_fd, events_buffer.data(), events_buffer.size());
        if (length < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            if (errno == EINTR) {
                continue;
            }
            skip_all();
            return true;
        }

        for (char* i = events_buffer.data(); i < events_buffer.data() + length; ) {
            struct ::inotify_event* p_event = reinterpret_cast<struct inotify_event*>(i);
            if (p_event->mask & IN_Q_OVERFLOW) {
                skip_all();
                return true;
            }

            if (p_event->wd != -1) {
                std::map<int, callback_fn_t>::iterator it = m_wd_to_callback.find(p_event->wd);
                if (it != m_wd_to_callback.end() && it->second) {
                    it->second(p_event->mask, std::string_view(p_event->name));
                }
            } else {
                skip_all();
            }
            i += __builtin_offsetof(struct inotify_event, name) + p_event->len;
        }
    }
    return true;
}

void notifier::remove_watch(int wd)
{
    if (m_inotify_fd < 0 || wd < 0) {
        return;
    }

    const int rc = RETRY_EINTR(::inotify_rm_watch(m_inotify_fd, wd));
    if (rc == 0) {
        m_wd_to_callback.erase(wd);
    }
}

void notifier::skip_all()
{
    for (std::map<int, callback_fn_t>::value_type& p : m_wd_to_callback) {
        p.second(0, std::string_view());
    }
}

bool notifier::wait(const size_t wait_ms)
{
    if (m_inotify_fd < 0) {
        return false;
    }

    do {
        int events_count = -1;
        if (RETRY_EINTR(::ioctl(m_inotify_fd, FIONREAD, &events_count)) != 0) {
            return false;
        }

        if (events_count >= (int)sizeof(struct ::inotify_event)) {
            break;
        }

        assert(events_count == 0);
        if (wait_ms == 0) {
            return false;
        }

        struct ::pollfd wt;
        wt.fd = m_inotify_fd;
        wt.events = POLLIN | POLLPRI;
        if (::poll(&wt, 1, wait_ms) < 1) {
            return false;
        }
    } while (false);
    return true;
}

} // namespace fq
} // namespace details
} // namespace gut
} // namespace wstux
