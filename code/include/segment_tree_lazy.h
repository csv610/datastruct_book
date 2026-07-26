#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>

namespace dsa {

// Segment tree with lazy propagation for range updates and range queries
// Supports: range add, range sum query (easily adaptable to min/max)
template <typename T = int, typename Merge = std::plus<T>>
class segment_tree_lazy {
public:
    segment_tree_lazy(const std::vector<T>& data, Merge merge = Merge{})
        : n_(data.size()), merge_(std::move(merge)) {
        tree_.assign(4 * n_, T{});
        lazy_.assign(4 * n_, T{});
        if (n_ > 0) build(data, 1, 0, n_ - 1);
    }

    std::size_t size() const noexcept { return n_; }
    bool empty() const noexcept { return n_ == 0; }

    // range update: add val to all elements in [l, r]
    void update(std::size_t l, std::size_t r, T val) {
        update(1, 0, n_ - 1, l, r, val);
    }

    // range query: aggregate over [l, r]
    T query(std::size_t l, std::size_t r) {
        return query(1, 0, n_ - 1, l, r);
    }

    // point query
    T point_query(std::size_t idx) {
        return query(idx, idx);
    }

    // point update: set value
    void point_update(std::size_t idx, T val) {
        T delta = val - point_query(idx);
        update(idx, idx, delta);
    }

private:
    void build(const std::vector<T>& data, std::size_t node, std::size_t start, std::size_t end) {
        if (start == end) {
            tree_[node] = data[start];
            return;
        }
        std::size_t mid = start + (end - start) / 2;
        build(data, 2 * node, start, mid);
        build(data, 2 * node + 1, mid + 1, end);
        tree_[node] = merge_(tree_[2 * node], tree_[2 * node + 1]);
    }

    void push_down(std::size_t node, std::size_t start, std::size_t end) {
        if (lazy_[node] == T{}) return;
        std::size_t mid = start + (end - start) / 2;
        tree_[2 * node] += lazy_[node] * static_cast<T>(mid - start + 1);
        tree_[2 * node + 1] += lazy_[node] * static_cast<T>(end - mid);
        lazy_[2 * node] += lazy_[node];
        lazy_[2 * node + 1] += lazy_[node];
        lazy_[node] = T{};
    }

    void update(std::size_t node, std::size_t start, std::size_t end,
                std::size_t l, std::size_t r, T val) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            tree_[node] += val * static_cast<T>(end - start + 1);
            lazy_[node] += val;
            return;
        }
        push_down(node, start, end);
        std::size_t mid = start + (end - start) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        tree_[node] = merge_(tree_[2 * node], tree_[2 * node + 1]);
    }

    T query(std::size_t node, std::size_t start, std::size_t end,
            std::size_t l, std::size_t r) {
        if (r < start || end < l) return T{};
        if (l <= start && end <= r) return tree_[node];
        push_down(node, start, end);
        std::size_t mid = start + (end - start) / 2;
        return merge_(query(2 * node, start, mid, l, r),
                      query(2 * node + 1, mid + 1, end, l, r));
    }

    std::size_t n_;
    std::vector<T> tree_;
    std::vector<T> lazy_;
    Merge merge_;
};

}  // namespace dsa
