template <typename Key>
class misra_gries {
public:
    explicit misra_gries(int k) : k_(k) {}

    void add(const Key& key) {
        // If key already tracked, increment
        auto it = counters_.find(key);
        if (it != counters_.end()) {
            it->second += 1;
            return;
        }

        // If room, add new entry
        if (static_cast<int>(counters_.size()) < k_ - 1) {
            counters_[key] = 1;
            return;
        }

        // Decrement all, remove zeros
        for (auto it = counters_.begin(); it != counters_.end(); ) {
            it->second -= 1;
            if (it->second == 0) it = counters_.erase(it);
            else ++it;
        }
    }

    std::vector<std::pair<Key, size_t>> heavy_hitters(size_t min_count) const {
        std::vector<std::pair<Key, size_t>> result;
        for (const auto& [key, count] : counters_) {
            if (count >= min_count) result.push_back({key, count});
        }
        return result;
    }

    const std::unordered_map<Key, size_t>& counters() const { return counters_; }

private:
    int k_;
    std::unordered_map<Key, size_t> counters_;
};
