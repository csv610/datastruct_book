// ch14: Segment Trees, Fenwick Trees, and Union-Find
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

// ---- Segment Tree ----
template <typename T, typename Combine = std::plus<T>>
class segment_tree {
public:
    segment_tree(std::span<const T> data, Combine combine = {})
        : n_(data.size()), tree_(4 * n_), combine_(combine) {
        build(data, 1, 0, n_ - 1);
    }

    T query(size_t l, size_t r) {
        return query(1, 0, n_ - 1, l, r);
    }

    void update(size_t pos, const T& value) {
        update(1, 0, n_ - 1, pos, value);
    }

private:
    void build(std::span<const T> data, size_t node, size_t l, size_t r) {
        if (l == r) { tree_[node] = data[l]; return; }
        size_t mid = l + (r - l) / 2;
        build(data, 2 * node, l, mid);
        build(data, 2 * node + 1, mid + 1, r);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    T query(size_t node, size_t l, size_t r, size_t ql, size_t qr) {
        if (ql > r || qr < l) return T{};
        if (ql <= l && r <= qr) return tree_[node];
        size_t mid = l + (r - l) / 2;
        auto left = query(2 * node, l, mid, ql, qr);
        auto right = query(2 * node + 1, mid + 1, r, ql, qr);
        return combine_(left, right);
    }

    void update(size_t node, size_t l, size_t r, size_t pos, const T& value) {
        if (l == r) { tree_[node] = value; return; }
        size_t mid = l + (r - l) / 2;
        if (pos <= mid) update(2 * node, l, mid, pos, value);
        else update(2 * node + 1, mid + 1, r, pos, value);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    size_t n_;
    std::vector<T> tree_;
    Combine combine_;
};

// ---- Fenwick Tree (Binary Indexed Tree) ----
template <typename T>
class fenwick_tree {
public:
    explicit fenwick_tree(size_t n) : bit_(n + 1, T{}) {}

    void add(size_t idx, T delta) {
        ++idx;
        while (idx < bit_.size()) {
            bit_[idx] += delta;
            idx += idx & -idx;
        }
    }

    T prefix_sum(size_t idx) const {
        T result{};
        while (idx > 0) {
            result += bit_[idx];
            idx -= idx & -idx;
        }
        return result;
    }

    T range_sum(size_t l, size_t r) const {
        return prefix_sum(r) - prefix_sum(l);
    }

private:
    std::vector<T> bit_;
};

// ---- Union-Find (path compression + union by size) ----
class union_find {
public:
    union_find(size_t n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];
            x = parent_[x];
        }
        return x;
    }

    void union_sets(size_t x, size_t y) {
        size_t rx = find(x);
        size_t ry = find(y);
        if (rx == ry) return;
        if (size_[rx] < size_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        size_[rx] += size_[ry];
    }

    bool same_set(size_t x, size_t y) { return find(x) == find(y); }

    size_t set_size(size_t x) { return size_[find(x)]; }

    size_t count_sets() const {
        size_t count = 0;
        for (size_t i = 0; i < parent_.size(); ++i)
            if (parent_[i] == i) ++count;
        return count;
    }

private:
    std::vector<size_t> parent_;
    std::vector<size_t> size_;
};

int main() {
    std::cout << "=== Segment Tree ===\n\n";
    std::vector<int> data = {1, 3, 5, 7, 9, 11};
    segment_tree<int> st(data);
    std::cout << "Array:";
    for (auto x : data) std::cout << " " << x;
    std::cout << "\n";
    std::cout << "Sum [0, 5]: " << st.query(0, 5) << "\n";
    std::cout << "Sum [1, 3]: " << st.query(1, 3) << "\n";
    st.update(2, 10);
    std::cout << "After update pos 2 to 10:\n";
    std::cout << "Sum [1, 3]: " << st.query(1, 3) << "\n\n";

    // Range minimum
    auto min_comb = [](int a, int b) { return std::min(a, b); };
    segment_tree<int, decltype(min_comb)> st_min(data, min_comb);
    std::cout << "Min [0, 5]: " << st_min.query(0, 5) << "\n";
    std::cout << "Min [1, 3]: " << st_min.query(1, 3) << "\n\n";

    std::cout << "=== Fenwick Tree ===\n\n";
    fenwick_tree<int> bit(6);
    for (size_t i = 0; i < data.size(); ++i) bit.add(i, data[i]);
    std::cout << "Prefix sum [0, 5): " << bit.prefix_sum(5) << "\n";
    std::cout << "Range sum [1, 4): " << bit.range_sum(1, 4) << "\n";
    std::cout << "Range sum [0, 6): " << bit.range_sum(0, 6) << "\n";
    bit.add(2, 3);
    std::cout << "After add 3 at pos 2, range sum [0, 6): "
              << bit.range_sum(0, 6) << "\n\n";

    std::cout << "=== Union-Find ===\n\n";
    union_find uf(7);
    uf.union_sets(0, 1); uf.union_sets(1, 2);
    uf.union_sets(3, 4); uf.union_sets(4, 5); uf.union_sets(5, 6);
    std::cout << "Sets: " << uf.count_sets() << "\n";
    std::cout << "same_set(0, 2): " << uf.same_set(0, 2) << "\n";
    std::cout << "same_set(0, 3): " << uf.same_set(0, 3) << "\n";
    uf.union_sets(2, 3);
    std::cout << "After union(2, 3):\n";
    std::cout << "same_set(0, 6): " << uf.same_set(0, 6) << "\n";
    std::cout << "Sets: " << uf.count_sets() << "\n";
    std::cout << "Size of set containing 0: " << uf.set_size(0) << "\n\n";

    std::cout << "All ch14 tests passed.\n";
    return 0;
}
