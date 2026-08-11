#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

namespace dsa {

template <typename T = int>
class persistent_segment_tree {
public:
    explicit persistent_segment_tree(std::size_t n, const std::vector<T>& initial = {})
        : n_(n) {
        if (n_ == 0) throw std::invalid_argument("persistent segment tree needs nonzero size");
        roots_.push_back(build(initial, 0, n_));
    }

    std::size_t versions() const noexcept { return roots_.size(); }
    std::size_t size() const noexcept { return n_; }

    T query(std::size_t version, std::size_t left, std::size_t right) const {
        check_version(version); check_range(left, right);
        return query(roots_[version], 0, n_, left, right);
    }

    std::size_t update(std::size_t version, std::size_t index, const T& value) {
        check_version(version);
        if (index >= n_) throw std::out_of_range("persistent segment tree index");
        roots_.push_back(update(roots_[version], 0, n_, index, value));
        return roots_.size() - 1;
    }

private:
    struct node {
        T sum{};
        std::shared_ptr<const node> left, right;
    };
    using pointer = std::shared_ptr<const node>;

    pointer build(const std::vector<T>& values, std::size_t lo, std::size_t hi) const {
        if (hi - lo == 1) return std::make_shared<node>(node{lo < values.size() ? values[lo] : T{}, nullptr, nullptr});
        const auto mid = lo + (hi - lo) / 2;
        auto left = build(values, lo, mid), right = build(values, mid, hi);
        return std::make_shared<node>(node{left->sum + right->sum, left, right});
    }

    pointer update(const pointer& old, std::size_t lo, std::size_t hi,
                   std::size_t index, const T& value) const {
        if (hi - lo == 1) return std::make_shared<node>(node{value, nullptr, nullptr});
        const auto mid = lo + (hi - lo) / 2;
        pointer left = old->left, right = old->right;
        if (index < mid) left = update(left, lo, mid, index, value);
        else right = update(right, mid, hi, index, value);
        return std::make_shared<node>(node{left->sum + right->sum, left, right});
    }

    T query(const pointer& current, std::size_t lo, std::size_t hi,
            std::size_t left, std::size_t right) const {
        if (right <= lo || hi <= left) return T{};
        if (left <= lo && hi <= right) return current->sum;
        const auto mid = lo + (hi - lo) / 2;
        return query(current->left, lo, mid, left, right) +
               query(current->right, mid, hi, left, right);
    }

    void check_version(std::size_t v) const { if (v >= roots_.size()) throw std::out_of_range("version"); }
    void check_range(std::size_t l, std::size_t r) const {
        if (l > r || r > n_) throw std::out_of_range("range");
    }

    std::size_t n_;
    std::vector<pointer> roots_;
};

} // namespace dsa
