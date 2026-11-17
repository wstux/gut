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
#include <thread>

#include <benchmark/benchmark.h>

#include "gut/stagequeue.h"

namespace {

namespace gut = ::wstux::gut;
namespace details = gut::details;
namespace fq = gut::details::fq;

class queue_fixture : public benchmark::Fixture
{
public:
    void SetUp(const benchmark::State& state) override
    {
        if (state.thread_index() == 0) {
            m_root_dir = make_tmp_dir("perf_queue_fixture");
            m_oq = gut::stagequeue::create_queue((m_root_dir / "output").string(), []() { return false; });
            m_pq = gut::stagequeue::create_queue((m_root_dir / "proc").string(), m_oq->path(), []() { return false; });
            m_iq = gut::stagequeue::create_queue((m_root_dir / "input").string(), m_pq->path(), []() { return false; });
        }
    }

    void TearDown(const benchmark::State& state) override
    {
        if (state.thread_index() == 0) {
            m_oq.reset();
            m_pq.reset();
            m_iq.reset();
            std::error_code ec;
            std::filesystem::remove_all(m_root_dir, ec);
        }
    }

    gut::stagequeue::ptr& input_queue() { return m_iq; }
    gut::stagequeue::ptr& proc_queue() { return m_pq; }
    gut::stagequeue::ptr& output_queue() { return m_oq; }

    static std::filesystem::path make_tmp_dir(const std::string& prefix)
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

private:
    std::filesystem::path m_root_dir;
    gut::stagequeue::ptr m_oq;
    gut::stagequeue::ptr m_pq;
    gut::stagequeue::ptr m_iq;
};

/*void perf_queue_create(benchmark::State& state)
{
    static std::filesystem::path root_dir;
    static gut::stagequeue::ptr wq;

    if (state.thread_index() == 0) {
        root_dir = make_tmp_dir("perf_queue_create");
        if (root_dir.empty()) {
            state.SkipWithError("Failed to create queue root dir");
        }
        wq = gut::stagequeue::create_queue(root_dir.string(), []() { return false; });
        if (! wq) {
            state.SkipWithError("Failed to create write queue");
            return;
        }
    }

    for (auto _ : state) {
        gut::stageitem::ptr item = wq->create();
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
}*/
BENCHMARK_DEFINE_F(queue_fixture, perf_queue_create)(benchmark::State& state)
{
    gut::stagequeue::ptr& wq = input_queue();
    for (auto _ : state) {
        gut::stageitem::ptr item = wq->create();
        if (! item || ! item->commit()) {
            state.SkipWithError("Failed to create or commit item");
            break;
        }
    }
    state.SetItemsProcessed(state.iterations());
}

/*void perf_queue_move(benchmark::State& state)
{
    static std::filesystem::path root_dir;
    static std::atomic_size_t moved_count{0};
    static gut::stagequeue::ptr iq;
    static gut::stagequeue::ptr pq;
    static gut::stagequeue::ptr oq;

    if (state.thread_index() == 0) {
        root_dir = make_tmp_dir("perf_queue_move");
        if (root_dir.empty()) {
            state.SkipWithError("Failed to create queue root dir");
        }

        oq = gut::stagequeue::create_queue((root_dir / "output").string(), []() { return false; });
        pq = gut::stagequeue::create_queue((root_dir / "proc").string(), oq->path(), []() { return false; });
        iq = gut::stagequeue::create_queue((root_dir / "input").string(), pq->path(), []() { return false; });
        if (! iq || ! pq || ! oq) {
            state.SkipWithError("Failed to create queues");
            return;
        }

        for (size_t i = 0; i < 20000; ++i) {
            gut::stageitem::ptr item = iq->create();
            if (item) {
                item->commit();
            }
        }
        moved_count.store(0, std::memory_order_relaxed);
    }

    for (auto _ : state) {
        while (true) {
            gut::stageitem::ptr item = pq->pop(0);
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
}*/
BENCHMARK_DEFINE_F(queue_fixture, perf_queue_move)(benchmark::State& state)
{
    gut::stagequeue::ptr& iq = input_queue();
    gut::stagequeue::ptr& pq = proc_queue();

    static std::mutex refill_mutex;
    constexpr size_t k_batch_size = 5000;

    size_t processed_count = 0;
    for (auto _ : state) {
        gut::stageitem::ptr item = pq->pop(0);
        if (! item) {
            state.PauseTiming();
            {
                std::lock_guard<std::mutex> lock(refill_mutex);
                item = pq->pop(0);
                if (! item) {
                    for (size_t i = 0; i < k_batch_size; ++i) {
                        if (gut::stageitem::ptr new_item = iq->create()) {
                            new_item->commit();
                        }
                    }
                    item = pq->pop(0);
                }
            }
            state.ResumeTiming();
        }

        if (item && item->commit()) {
            ++processed_count;
        }
    }

    state.SetItemsProcessed(processed_count);
}

/*void perf_queue_process_gut(benchmark::State& state)
{
    static std::filesystem::path root_dir;
    static gut::stagequeue::ptr iq;
    static gut::stagequeue::ptr oq;
    static std::atomic_size_t active_writers{0};
    static std::atomic_size_t total_completed{0};

    const size_t total_threads = static_cast<size_t>(state.threads());
    const size_t writers_count = total_threads / 2;

    if (state.thread_index() == 0) {
        root_dir = make_tmp_dir("perf_queue_process_gut");
        if (root_dir.empty()) {
            state.SkipWithError("Failed to create queue root dir");
        }
        oq = gut::stagequeue::create_queue((root_dir / "output").string(), []() { return false; });
        iq = gut::stagequeue::create_queue((root_dir / "input").string(), oq->path(), []() { return false; });
        active_writers.store(writers_count, std::memory_order_relaxed);
        total_completed.store(0, std::memory_order_relaxed);
    }

    if (state.thread_index() % 2 == 0) {
        for (auto _ : state) {
            if (gut::stageitem::ptr p_item = iq->create()) {
                p_item->commit();
            }
        }
        active_writers.fetch_sub(1, std::memory_order_acq_rel);
    } else {
        for (auto _ : state) {
            while (true) {
                if (gut::stageitem::ptr p_item = oq->pop(1)) {
                    if (p_item->commit()) {
                        total_completed.fetch_add(1, std::memory_order_relaxed);
                    }
                    break;
                }
                if (active_writers.load(std::memory_order_acquire) == 0) {
                    if (gut::stageitem::ptr p_item = oq->pop(0)) {
                        if (p_item->commit()) {
                            total_completed.fetch_add(1, std::memory_order_relaxed);
                        }
                    }
                    break;
                }
                std::this_thread::yield();
            }
        }
    }

    if (state.thread_index() == 0) {
        state.SetItemsProcessed(total_completed.load(std::memory_order_relaxed));
        iq.reset();
        oq.reset();
        std::error_code ec;
        std::filesystem::remove_all(root_dir, ec);
    }
}*/
BENCHMARK_DEFINE_F(queue_fixture, perf_queue_process_gut)(benchmark::State& state)
{
    gut::stagequeue::ptr& iq = input_queue();
    gut::stagequeue::ptr& oq = proc_queue();

    size_t processed_count = 0;
    for (auto _ : state) {
        if (gut::stageitem::ptr w_item = iq->create()) {
            if (! w_item->commit()) {
                state.SkipWithError("Failed to create or commit item");
                break;
            }
        }

        if (gut::stageitem::ptr r_item = oq->pop(100)) {
            if (r_item->commit()) {
                ++processed_count;
            }
        }
    }
    state.SetItemsProcessed(processed_count);
}

void perf_queue_empty_pop(benchmark::State& state)
{
    std::filesystem::path root_dir = queue_fixture::make_tmp_dir("perf_queue_empty_pop");
    gut::stagequeue::ptr q = gut::stagequeue::create_queue(root_dir.string(), []() { return false; });

    for (auto _ : state) {
        gut::stageitem::ptr item = q->pop(0);
        benchmark::DoNotOptimize(item);
    }

    state.SetItemsProcessed(state.iterations());

    q.reset();
    std::error_code ec;
    std::filesystem::remove_all(root_dir, ec);
}

} // <anonymous> namespace

//BENCHMARK(perf_queue_create)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK_REGISTER_F(queue_fixture, perf_queue_create)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();

//BENCHMARK(perf_queue_move)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Iterations(1)->UseRealTime();
BENCHMARK_REGISTER_F(queue_fixture, perf_queue_move)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();

//BENCHMARK(perf_queue_process_gut)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK_REGISTER_F(queue_fixture, perf_queue_process_gut)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();

BENCHMARK(perf_queue_empty_pop)->UseRealTime();

BENCHMARK_MAIN();
