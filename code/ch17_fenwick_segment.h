#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

#include <concepts>
#include <functional>
#include <numeric>
#include <span>
#include <stdexcept>
#include <vector>

// ---- Fenwick Tree (Binary Indexed Tree) ----
template <std::regular T>
class fenwick_tree {
public:
    explicit fenwick_tree(std::size_t n) : bit_(n + 1, T{}) {}

    // Add delta at position idx (0-indexed)
    void add(std::size_t idx, T delta) {
        ++idx;  // 1-indexed internally
        while (idx < bit_.size()) {
            bit_[idx] += delta;
            idx += idx & -idx;
        }
    }

    // Sum over [0, idx)
    T prefix_sum(std::size_t idx) const {
        T result{};
        while (idx > 0) {
            result += bit_[idx];
            idx -= idx & -idx;
        }
        return result;
    }

    // Sum over [l, r)
    T range_sum(std::size_t l, std::size_t r) const {
        if (l > r) throw std::invalid_argument("l must be <= r");
        return prefix_sum(r) - prefix_sum(l);
    }

    // Point value at idx
    T point_value(std::size_t idx) const {
        return range_sum(idx, idx + 1);
    }

    // Set position idx to value (must be non-decreasing for correctness)
    void set(std::size_t idx, T value) {
        T current = point_value(idx);
        add(idx, value - current);
    }

    // Find smallest idx such that prefix_sum(idx) >= target
    std::size_t lower_bound(T target) const {
        if (target <= T{}) return 0;
        std::size_t idx = 0;
        std::size_t mask = 1;
        while (mask < bit_.size()) mask <<= 1;
        mask >>= 1;

        while (mask > 0) {
            std::size_t next = idx + mask;
            if (next < bit_.size() && bit_[next] < target) {
                target -= bit_[next];
                idx = next;
            }
            mask >>= 1;
        }
        return idx;  // 0-indexed
    }

    std::size_t size() const noexcept { return bit_.size() - 1; }

private:
    std::vector<T> bit_;
};

// ---- Segment Tree (range sum / point update) ----
template <std::regular T, typename Combine = std::plus<T>>
class segment_tree {
public:
    segment_tree(std::span<const T> data, Combine combine = {})
        : n_(data.size()), tree_(4 * n_), combine_(std::move(combine)) {
        build(data, 1, 0, n_ - 1);
    }

    // Query over [l, r] inclusive
    T query(std::size_t l, std::size_t r) {
        return query(1, 0, n_ - 1, l, r);
    }

    // Point update: set position pos to value
    void update(std::size_t pos, const T& value) {
        update(1, 0, n_ - 1, pos, value);
    }

    std::size_t size() const noexcept { return n_; }

private:
    void build(std::span<const T> data, std::size_t node,
               std::size_t l, std::size_t r) {
        if (l == r) {
            tree_[node] = data[l];
            return;
        }
        auto mid = l + (r - l) / 2;
        build(data, 2 * node, l, mid);
        build(data, 2 * node + 1, mid + 1, r);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    T query(std::size_t node, std::size_t l, std::size_t r,
            std::size_t ql, std::size_t qr) {
        if (ql > r || qr < l) return T{};
        if (ql <= l && r <= qr) return tree_[node];
        auto mid = l + (r - l) / 2;
        return combine_(query(2 * node, l, mid, ql, qr),
                        query(2 * node + 1, mid + 1, r, ql, qr));
    }

    void update(std::size_t node, std::size_t l, std::size_t r,
                std::size_t pos, const T& value) {
        if (l == r) {
            tree_[node] = value;
            return;
        }
        auto mid = l + (r - l) / 2;
        if (pos <= mid)
            update(2 * node, l, mid, pos, value);
        else
            update(2 * node + 1, mid + 1, r, pos, value);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    std::size_t         n_;
    std::vector<T>      tree_;
    Combine             combine_;
};

#endif
