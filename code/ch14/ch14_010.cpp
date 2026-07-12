template <typename T>
class fenwick_tree {
public:
    explicit fenwick_tree(size_t n) : bit_(n + 1, T{}) {}

    void add(size_t idx, T delta) {
        ++idx;  // 1-indexed internally
        while (idx < bit_.size()) {
            bit_[idx] += delta;
            idx += idx & -idx;
        }
    }

    T prefix_sum(size_t idx) const {
        // Sum over [0, idx)
        T result{};
        while (idx > 0) {
            result += bit_[idx];
            idx -= idx & -idx;
        }
        return result;
    }

    T range_sum(size_t l, size_t r) const {
        // Sum over [l, r)
        return prefix_sum(r) - prefix_sum(l);
    }

    // Find smallest index such that prefix_sum >= target
    size_t lower_bound(T target) const {
        size_t idx = 0;
        size_t bit_mask = 1;
        while (bit_mask < bit_.size()) bit_mask <<= 1;
        bit_mask >>= 1;

        while (bit_mask > 0) {
            size_t next = idx + bit_mask;
            if (next < bit_.size() && bit_[next] < target) {
                target -= bit_[next];
                idx = next;
            }
            bit_mask >>= 1;
        }
        return idx;  // 0-indexed
    }

private:
    std::vector<T> bit_;
};
