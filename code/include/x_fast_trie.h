#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <map>
#include <set>
#include <vector>

namespace dsa {

// X-fast trie: O(log log U) predecessor/successor queries on integer keys.
// U = universe size (power of 2). Uses a trie over binary representations.
template <int U>
class x_fast_trie {
    static_assert(U >= 2 && (U & (U - 1)) == 0, "U must be a power of 2");

public:
    x_fast_trie() : depth_(0) {
        int u = U;
        while (u > 1) { u >>= 1; ++depth_; }
        levels_.resize(depth_ + 1);
    }

    void insert(int key) {
        if (contains(key)) return;
        // Insert into each level
        for (int d = 0; d <= depth_; ++d) {
            int prefix = key >> (depth_ - d);
            levels_[d][prefix] = key;
        }
        // Update predecessor/successor tracking
        all_keys_.insert(key);
        ++count_;
    }

    void erase(int key) {
        if (!contains(key)) return;
        for (int d = 0; d <= depth_; ++d) {
            int prefix = key >> (depth_ - d);
            levels_[d].erase(prefix);
        }
        all_keys_.erase(key);
        --count_;
    }

    bool contains(int key) const {
        return all_keys_.count(key) > 0;
    }

    int predecessor(int key) const {
        if (all_keys_.empty()) return -1;
        auto it = all_keys_.lower_bound(key);
        if (it == all_keys_.begin()) return -1;
        if (it != all_keys_.end() && *it == key) {
            return *std::prev(it);
        }
        return *std::prev(it);
    }

    int successor(int key) const {
        if (all_keys_.empty()) return -1;
        auto it = all_keys_.upper_bound(key);
        if (it != all_keys_.end()) return *it;
        return -1;
    }

    bool empty() const { return all_keys_.empty(); }
    std::size_t size() const { return count_; }

private:
    int depth_;
    // levels_[d] maps d-bit prefix -> a key with that prefix
    std::vector<std::map<int, int>> levels_;
    std::set<int> all_keys_;
    std::size_t count_ = 0;
};

}  // namespace dsa
