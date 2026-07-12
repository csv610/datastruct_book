class union_find {
public:
    union_find(size_t n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t x) {
        // Path compression
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];  // halving
            x = parent_[x];
        }
        return x;
    }

    void union_sets(size_t x, size_t y) {
        size_t rx = find(x);
        size_t ry = find(y);
        if (rx == ry) return;

        // Union by size
        if (size_[rx] < size_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        size_[rx] += size_[ry];
    }

    bool same_set(size_t x, size_t y) {
        return find(x) == find(y);
    }

    size_t set_size(size_t x) {
        return size_[find(x)];
    }

    size_t count_sets() const {
        size_t count = 0;
        for (size_t i = 0; i < parent_.size(); ++i) {
            if (parent_[i] == i) ++count;
        }
        return count;
    }

private:
    std::vector<size_t> parent_;
    std::vector<size_t> size_;
};
