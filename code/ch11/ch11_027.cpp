#include <map>
#include <vector>
#include <algorithm>
#include <optional>

class lsm_tree {
    std::map<int, int> memtable_;               // in-memory sorted buffer
    std::vector<std::vector<std::pair<int,int>>> levels_;  // sorted runs on disk
    static constexpr size_t MEMTABLE_LIMIT = 1024;

    void flush() {
        if (memtable_.empty()) return;
        std::vector<std::pair<int,int>> run(memtable_.begin(), memtable_.end());
        levels_.push_back(std::move(run));
        memtable_.clear();
        compact();
    }

    void compact() {
        for (size_t i = 0; i + 1 < levels_.size(); ++i) {
            if (levels_[i].size() + levels_[i+1].size() > MEMTABLE_LIMIT) {
                std::vector<std::pair<int,int>> merged;
                std::merge(levels_[i].begin(), levels_[i].end(),
                           levels_[i+1].begin(), levels_[i+1].end(),
                           std::back_inserter(merged));
                levels_[i+1] = std::move(merged);
                levels_.erase(levels_.begin() + i);
            }
        }
    }

    std::optional<int> search_levels(int key) const {
        for (int i = static_cast<int>(levels_.size()) - 1; i >= 0; --i) {
            auto it = std::lower_bound(levels_[i].begin(), levels_[i].end(),
                                       std::make_pair(key, 0));
            if (it != levels_[i].end() && it->first == key)
                return it->second;
        }
        return std::nullopt;
    }

public:
    void put(int key, int value) {
        memtable_[key] = value;
        if (memtable_.size() >= MEMTABLE_LIMIT) flush();
    }

    std::optional<int> get(int key) const {
        auto it = memtable_.find(key);
        if (it != memtable_.end()) return it->second;
        return search_levels(key);
    }

    void erase(int key) {
        memtable_.erase(key);
    }
};
