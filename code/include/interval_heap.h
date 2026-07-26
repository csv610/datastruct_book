#pragma once
#include <algorithm>
#include <functional>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

// Interval heap: double-ended priority queue.
// Supports O(log n) insert, O(1) min/max, O(log n) delete min/max.
// Uses two std::sets: one sorted by lo (min), one by hi (max).
template <typename T, typename Compare = std::less<T>>
class interval_heap {
public:
    interval_heap() = default;

    void push(T lo, T hi) {
        if (cmp_(hi, lo)) std::swap(lo, hi);
        by_lo_.insert({lo, hi});
        by_hi_.insert({lo, hi});
        ++count_;
    }

    const T& min() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return by_lo_.begin()->first;
    }

    const T& max() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return by_hi_.rbegin()->second;
    }

    void pop_min() {
        if (empty()) throw std::underflow_error("heap is empty");
        auto it = by_lo_.begin();
        by_hi_.erase(*it);
        by_lo_.erase(it);
        --count_;
    }

    void pop_max() {
        if (empty()) throw std::underflow_error("heap is empty");
        auto it = std::prev(by_hi_.end());
        by_lo_.erase(*it);
        by_hi_.erase(it);
        --count_;
    }

    bool empty() const noexcept { return count_ == 0; }
    std::size_t size() const noexcept { return count_; }

private:
    struct interval {
        T first;   // lo
        T second;  // hi
        bool operator<(const interval& o) const {
            return Compare{}(first, o.first);
        }
    };

    struct interval_cmp_hi {
        Compare cmp;
        bool operator()(const interval& a, const interval& b) const {
            return cmp(a.second, b.second);
        }
    };

    std::set<interval> by_lo_;
    std::set<interval, interval_cmp_hi> by_hi_;
    std::size_t count_ = 0;
    Compare cmp_;
};

}  // namespace dsa
