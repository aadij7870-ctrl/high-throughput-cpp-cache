#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <optional>

class LRUCache {
private:
    size_t capacity;
    std::list<std::pair<std::string, std::string>> itemsList;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> itemMap;

public:
    explicit LRUCache(size_t cap) : capacity(cap) {}

    void put(const std::string& key, const std::string& value) {
        auto it = itemMap.find(key);
        if (it != itemMap.end()) {
            itemsList.erase(it->second);
            itemsList.push_front({key, value});
            itemMap[key] = itemsList.begin();
            return;
        }

        if (itemsList.size() >= capacity) {
            std::string oldestKey = itemsList.back().first;
            itemsList.pop_back();
            itemMap.erase(oldestKey);
        }

        itemsList.push_front({key, value});
        itemMap[key] = itemsList.begin();
    }

    std::optional<std::string> get(const std::string& key) {
        auto it = itemMap.find(key);
        if (it == itemMap.end()) {
            return std::nullopt;
        }

        auto value = it->second->second;
        itemsList.erase(it->second);
        itemsList.push_front({key, value});
        itemMap[key] = itemsList.begin();

        return value;
    }
};
