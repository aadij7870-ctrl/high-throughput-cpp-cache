#pragma once
#include "LRUCache.hpp"
#include <shared_mutex>
#include <mutex>

class ThreadSafeCache {
private:
    LRUCache cache;
    mutable std::shared_mutex rwLock;

public:
    explicit ThreadSafeCache(size_t cap) : cache(cap) {}

    void set(const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_mutex> lock(rwLock);
        cache.put(key, value);
    }

    std::optional<std::string> get(const std::string& key) {
        std::unique_lock<std::shared_mutex> lock(rwLock);
        return cache.get(key);
    }
};
