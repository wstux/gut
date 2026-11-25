# Gut queue library

This library implements a high-performance, thread-safe, inter-process transactional
file-based queue for Linux, written in C++17.

The library is designed for the reliable transfer of data between independent processes
and threads, ensuring no data loss in the event of a failure, while maintaining minimal
I/O overhead and strict control over RAM usage.

## Architecture and operational logic

The queue consists of three isolated subdirectories:
1. `input/` — incoming data, ready for processing;
2. `proc/` — a working directory for items currently being processed;
3. `output/` — outgoing data that has already been processed.

## Limitations and scope of application

The library implements an **"one event — one file"** architecture and is optimized for the
reliable transfer of medium to large payloads (ranging from a few kilobytes to tens of
megabytes).

**Not recommended for very small events (< 1–4 KB).** The minimum block size in Linux file
systems (ext4/xfs) and the page size of SSD memory are typically 4 KB. When writing a large
number of tiny messages (tens or hundreds of bytes), each file still occupies an entire block
on the disk as well as a separate `inode`. This leads to a high **write amplification** factor,
accelerated SSD wear, and the risk of exhausting the available `inode` count on the partition
if a large backlog accumulates.

### Data lifecycle

1. Creation

```
    [stagequeue::create()]
              │
              ▼
         ┌─────────┐  stageitem::rollback() ┌─────────┐
         │  proc/  │ ─────────────────────► │  input/ │
         └────┬────┘                        └─────────┘
              │
      stageitem::commit()
              │
              ▼
         ┌─────────┐
         │ output/ │
         └─────────┘
```

2. Getting the available data

```
   ┌─────────┐  stagequeue::pop()  ┌─────────┐  stageitem::commit()  ┌─────────┐
   │  input/ │ ──────────────────► │  proc/  │ ────────────────────► │ output/ │
   └────▲────┘                     └────┬────┘                       └─────────┘
        │                               │
        └──── stageitem::rollback() ────┘
```

### File storage and directory rotation

To prevent file system degradation caused by the accumulation of a large number of files
in a single directory, and to eliminate contention among writers, a two-level hierarchy
of temporary subdirectories is used. When a directory file reaches a specified size limit,
an automatic rotation to a new directory occurs. A shared file lock is maintained on the
currently active directory.

When reading data from the queue, a **K-way merge** algorithm is used to ensure that data
is delivered in chronological order, even when it is being written in parallel from
multiple sources. No more than 16 directories are kept open in memory at any given time.

```
  queue_stage/ (input / proc / output)
   ├── primary_dir_01/
   │    ├── active_dir_01/  [ LOCK_SH -> LOCK_EX -> Cleaner ]
   │    │    └── (empty, all files have been processed)
   │    └── active_dir_02/  [LOCK_SH producer]
   │         ├── item_0001.dat  ──┐
   │         └── item_0002.dat  ──┼──► Tail (K-way Merge)
   └── primary_dir_02/            │
        └── active_dir_01/  ──────┘
             └── item_0003.dat
```

## Example

```cpp
#include <iostream>

#include <gut/stageitem.h>
#include <gut/stagequeue.h>

int main()
{
    // 1. Create a receiver queue and a sender queue (connect the sender's output to the receiver's input)
    stagequeue::ptr p_rq = wstux::gut::stagequeue::create_queue("/tmp/gut_example/queue_1", []() { return false; });
    stagequeue::ptr p_wq = wstux::gut::stagequeue::create_queue("/tmp/gut_example/queue_2", p_rq->path(), []() { return false; });

    // 2. Writing an item
    if (stageitem::ptr p_item = p_wq->create()) {
        // Work with p_item
        p_item->commit();
    }

    // 3. Reading and processing an item (waiting up to 1000 ms)
    if (stageitem::ptr p_item = p_rq->pop(1000)) {
        bool success = true;
        if (success) {
            r_item->commit();
        } else {
            r_item->rollback();
        }
    }

    return 0;
}
```

## Benchmarks

Continuous performance tracking and historical benchmark results are available
on the [benchmarks page](https://wstux.github.io/gut/dev/bench/index.html),
[default benchmarks page](https://wstux.github.io/gut/dev/bench/default_index.html).
The metrics are automatically updated via CI to monitor performance across commits.

## License

&copy; 2026 Chistyakov Alexander.

Open sourced under MIT license, the terms of which can be read here — [MIT License](http://opensource.org/licenses/MIT).
