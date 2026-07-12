class subset_sum_backtrack {
public:
    subset_sum_backtrack(std::vector<int> vals, int target)
        : vals_(std::move(vals)), target_(target) {
        std::sort(vals_.begin(), vals_.end());
    }

    bool has_solution() {
        std::vector<int> current;
        return backtrack(0, 0, current);
    }

    std::vector<std::vector<int>> find_all() {
        results_.clear();
        std::vector<int> current;
        backtrack_all(0, 0, current);
        return results_;
    }

private:
    bool backtrack(size_t i, int sum, std::vector<int>& cur) {
        if (sum == target_) return true;
        if (i >= vals_.size() || sum > target_) return false;

        int remaining = 0;
        for (size_t j = i; j < vals_.size(); ++j)
            remaining += vals_[j];
        if (sum + remaining < target_) return false;

        cur.push_back(vals_[i]);
        if (backtrack(i + 1, sum + vals_[i], cur)) return true;
        cur.pop_back();

        return backtrack(i + 1, sum, cur);
    }

    void backtrack_all(size_t i, int sum, std::vector<int>& cur) {
        if (sum == target_) {
            results_.push_back(cur);
            return;
        }
        if (i >= vals_.size() || sum > target_) return;

        int remaining = 0;
        for (size_t j = i; j < vals_.size(); ++j)
            remaining += vals_[j];
        if (sum + remaining < target_) return;

        cur.push_back(vals_[i]);
        backtrack_all(i + 1, sum + vals_[i], cur);
        cur.pop_back();
        backtrack_all(i + 1, sum, cur);
    }

    std::vector<int> vals_;
    int target_;
    std::vector<std::vector<int>> results_;
};
