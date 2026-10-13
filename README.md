# High-Throughput Concurrent In-Memory Cache (C++)

A high-performance, thread-safe, in-memory key-value cache built in C++17 with TCP socket communication, LRU eviction, and synchronization.

## System Architecture
* **Underlying Storage:** std::unordered_map for O(1) lookups paired with a doubly linked list (std::list) for O(1) LRU evictions.
* **Concurrency:** Read-write mutual exclusion (std::shared_mutex) to prevent race conditions across parallel TCP client threads.
* **Networking:** Multi-threaded POSIX socket interface handling concurrent client sessions on port 8080.

## Verified Benchmark Results
Tested under multi-threaded stress testing (10 concurrent worker threads, 20,000 total operations):
* **Throughput:** ~10,614 req/sec
* **Average Latency:** 0.094 ms/req
* **Success Rate:** 100% (20,000 / 20,000 operations completed)

## Build & Run

### Native (Linux / WSL)
```bash
g++ -std=c++17 -pthread main.cpp -o cache_server
./cache_server
`
## Protocol
* `SET <key> <value>`: Inserts or updates an entry.
* `GET <key>`: Fetches value; moves entry to front of LRU queue.