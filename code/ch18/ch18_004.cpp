class subset_sum {
public:
    subset_sum(std::span<const int> values, int target)
        : values_(values.begin(), values.end()), target_(target) {
        std::sort(values_.begin(), values_.end());  // enable pruning
    }

    std::vector<std::vector<int>> solve() {
        std::vector<int> current;
        backtrack(0, 0, current);
        return solutions_;
    }

private:
    void backtrack(size_t i, int sum, std::vector<int>& current) {
        if (sum == target_) {
            solutions_.push_back(current);
            return;
        }
        if (i >= values_.size() || sum > target_) return;

        // Prune: if sum + remaining values can't reach target
        int remaining = std::accumulate(values_.begin() + i, values_.end(), 0);
        if (sum + remaining < target_) return;

        // Include values_[i]
        current.push_back(values_[i]);
        backtrack(i + 1, sum + values_[i], current);
        current.pop_back();

        // Exclude values_[i]
        backtrack(i + 1, sum, current);
    }

    std::vector<int> values_;
    int target_;
    std::vector<std::vector<int>> solutions_;
};
