#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Hash = std::hash<Key>>
class misra_gries {
public:
    explicit misra_gries(std::size_t k) : k_(k) {
        if (k_ < 2) throw std::invalid_argument("k must be at least 2");
    }

    void add(const Key& key) {
        auto it = counters_.find(key);
        if (it != counters_.end()) { ++it->second; return; }
        if (counters_.size() < k_ - 1) { counters_.emplace(key, 1); return; }
        for (auto it = counters_.begin(); it != counters_.end();) {
            if (--it->second == 0) it = counters_.erase(it);
            else ++it;
        }
    }

    std::vector<std::pair<Key, std::size_t>> candidates() const {
        return {counters_.begin(), counters_.end()};
    }
    std::size_t tracked() const noexcept { return counters_.size(); }

private:
    std::size_t k_;
    std::unordered_map<Key, std::size_t, Hash> counters_;
};

} // namespace dsa
