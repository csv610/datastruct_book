#pragma once
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

// ---- D-ary min-heap (generalized binary heap) ----
template <std::regular T, int D = 4, typename Compare = std::greater<T>>
class dary_heap {
    static_assert(D >= 2, "D must be >= 2");

public:
    dary_heap() = default;
    explicit dary_heap(Compare cmp) : compare_(std::move(cmp)) {}

    explicit dary_heap(std::vector<T> data, Compare cmp = {})
        : data_(std::move(data)), compare_(std::move(cmp)) {
        for (int i = static_cast<int>(data_.size()) / D - 1; i >= 0; --i)
            sift_down(static_cast<std::size_t>(i));
    }

    void push(const T& value) {
        data_.push_back(value);
        sift_up(data_.size() - 1);
    }

    void push(T&& value) {
        data_.push_back(std::move(value));
        sift_up(data_.size() - 1);
    }

    void pop() {
        if (empty()) throw std::underflow_error("heap is empty");
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) sift_down(0);
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return data_[0];
    }

    bool        empty() const noexcept { return data_.empty(); }
    std::size_t size()  const noexcept { return data_.size(); }

private:
    static constexpr std::size_t parent(std::size_t i) { return (i - 1) / D; }

    void sift_up(std::size_t i) {
        while (i > 0) {
            std::size_t p = parent(i);
            if (!compare_(data_[p], data_[i])) break;
            std::swap(data_[i], data_[p]);
            i = p;
        }
    }

    void sift_down(std::size_t i) {
        std::size_t n = data_.size();
        while (true) {
            std::size_t best = i;
            for (int c = 1; c <= D; ++c) {
                std::size_t child = D * i + c;
                if (child < n && compare_(data_[best], data_[child]))
                    best = child;
            }
            if (best == i) break;
            std::swap(data_[i], data_[best]);
            i = best;
        }
    }

    std::vector<T> data_;
    Compare        compare_;
};

}  // namespace dsa
