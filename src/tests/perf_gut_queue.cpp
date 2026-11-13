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
#include <random>

#include <benchmark/benchmark.h>

#include "gut/queue.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;
namespace fq = gut::details::fq;

std::filesystem::path make_tmp_dir(const std::string& prefix)
{
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path();

    std::random_device rd;
    std::mt19937 prng(rd());
    std::uniform_int_distribution<uint32_t> rand_dist;

    std::string rand_hex = std::to_string(rand_dist(prng));
    std::filesystem::path dir = temp_dir / (prefix + "_" + rand_hex);
    if (! std::filesystem::create_directories(dir)) {
        return "";
    }
    return dir;
}

void perf_queue_create(benchmark::State& state)
{
    static std::filesystem::path root_dir;
    static gut::queue::ptr wq;

    if (state.thread_index() == 0) {
        root_dir = make_tmp_dir("perf_queue_create");
        if (root_dir.empty()) {
            state.SkipWithError("Failed to create queue root dir");
        }
        wq = gut::queue::create_queue(root_dir.string(), []() { return false; });
        if (! wq) {
            state.SkipWithError("Failed to create write queue");
            return;
        }
    }

    for (auto _ : state) {
        gut::item::ptr item = wq->create();
        if (! item || ! item->commit()) {
            state.SkipWithError("Failed to create or commit item");
            break;
        }
    }

    state.SetItemsProcessed(state.iterations());

    if (state.thread_index() == 0) {
        wq.reset();
        std::error_code ec;
        std::filesystem::remove_all(root_dir, ec);
    }
}

void perf_queue_move(benchmark::State& state)
{
    static std::filesystem::path root_dir;
    static std::atomic_size_t moved_count{0};
    static gut::queue::ptr iq;
    static gut::queue::ptr pq;
    static gut::queue::ptr oq;

    if (state.thread_index() == 0) {
        root_dir = make_tmp_dir("perf_queue_move");
        if (root_dir.empty()) {
            state.SkipWithError("Failed to create queue root dir");
        }

        oq = gut::queue::create_queue((root_dir / "output").string(), []() { return false; });
        pq = gut::queue::create_queue((root_dir / "proc").string(), oq->path(), []() { return false; });
        iq = gut::queue::create_queue((root_dir / "input").string(), pq->path(), []() { return false; });
        if (! iq || ! pq || ! oq) {
            state.SkipWithError("Failed to create queues");
            return;
        }

        for (size_t i = 0; i < 20000; ++i) {
            gut::item::ptr item = iq->create();
            if (item) {
                item->commit();
            }
        }
        moved_count.store(0, std::memory_order_relaxed);
    }

    for (auto _ : state) {
        while (true) {
            gut::item::ptr item = pq->pop(0);
            if (!item) {
                break;
            }
            if (item->commit()) {
                moved_count.fetch_add(1, std::memory_order_relaxed);
            }
        }
    }

    if (state.thread_index() == 0) {
        state.SetItemsProcessed(moved_count.load(std::memory_order_relaxed));
        iq.reset();
        pq.reset();
        oq.reset();
        std::error_code ec;
        std::filesystem::remove_all(root_dir, ec);
    }
}

void perf_queue_process_gut(benchmark::State& state)
{
    static std::filesystem::path root_dir;
    static gut::queue::ptr iq;
    static gut::queue::ptr oq;
    static std::atomic<size_t> items_processed{0};

    if (state.thread_index() == 0) {
        root_dir = make_tmp_dir("perf_queue_process_gut");
        if (root_dir.empty()) {
            state.SkipWithError("Failed to create queue root dir");
        }
        iq = gut::queue::create_queue((root_dir / "input").string(), []() { return false; });
        oq = gut::queue::create_queue((root_dir / "output").string(), iq->path(), []() { return false; });
        items_processed.store(0, std::memory_order_relaxed);
    }

    if (state.thread_index() % 2 == 0) {
        for (auto _ : state) {
            gut::item::ptr item = iq->create();
            if (item && item->commit()) {
                items_processed.fetch_add(1, std::memory_order_relaxed);
            }
        }
    } else {
        for (auto _ : state) {
            while (gut::item::ptr item = oq->pop(1)) {
                item->commit();
            }
        }
    }

    if (state.thread_index() == 0) {
        state.SetItemsProcessed(items_processed.load(std::memory_order_relaxed));
        iq.reset();
        oq.reset();
        std::error_code ec;
        std::filesystem::remove_all(root_dir, ec);
    }
}

void perf_queue_empty_pop(benchmark::State& state)
{
    std::filesystem::path root_dir = make_tmp_dir("perf_queue_empty_pop");
    gut::queue::ptr q = gut::queue::create_queue(root_dir.string(), []() { return false; });

    for (auto _ : state) {
        gut::item::ptr item = q->pop(0);
        benchmark::DoNotOptimize(item);
    }

    state.SetItemsProcessed(state.iterations());

    q.reset();
    std::error_code ec;
    std::filesystem::remove_all(root_dir, ec);
}

} // <anonymous> namespace

BENCHMARK(perf_queue_create)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();

BENCHMARK(perf_queue_move)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Iterations(1)->UseRealTime();

BENCHMARK(perf_queue_process_gut)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();

BENCHMARK(perf_queue_empty_pop)->UseRealTime();

BENCHMARK_MAIN();
