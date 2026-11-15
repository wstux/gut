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

### Data lifecycle

1. Creation

```
   [stage_queue::create()]
              │
              ▼
         ┌─────────┐  stage_item::rollback() ┌─────────┐
         │  proc/  │ ──────────────────────► │  input/ │
         └────┬────┘                         └─────────┘
              │
     stage_item::commit()
              │
              ▼
         ┌─────────┐
         │ output/ │
         └─────────┘
```

2. Getting the available data

```
   ┌─────────┐  stage_queue::pop()  ┌─────────┐  stage_item::commit()  ┌─────────┐
   │  input/ │ ───────────────────► │  proc/  │ ─────────────────────► │ output/ │
   └────▲────┘                      └────┬────┘                        └─────────┘
        │                                │
        └──── stage_item::rollback() ────┘
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

#include <gut/stage_item.h>
#include <gut/stage_queue.h>

int main()
{
    // 1. Create a receiver queue and a sender queue (connect the sender's output to the receiver's input)
    stage_queue::ptr p_rq = wstux::gut::stage_queue::create_queue("/tmp/gut_example/queue_1", []() { return false; });
    stage_queue::ptr p_wq = wstux::gut::stage_queue::create_queue("/tmp/gut_example/queue_2", p_rq->path(), []() { return false; });

    // 2. Writing an item
    if (stage_item::ptr p_item = p_wq->create()) {
        // Work with p_item
        p_item->commit();
    }

    // 3. Reading and processing an item (waiting up to 1000 ms)
    if (stage_item::ptr p_item = p_rq->pop(1000)) {
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
