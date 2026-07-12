#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <concepts>
#include <functional>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

template <std::regular T, typename Compare = std::less<T>>
class max_heap {
public:
    max_heap() = default;

    explicit max_heap(Compare cmp) : compare_(std::move(cmp)) {}

    // Build heap from existing vector in O(n)
    explicit max_heap(std::vector<T> data, Compare cmp = {})
        : data_(std::move(data)), compare_(std::move(cmp)) {
        for (int i = static_cast<int>(data_.size()) / 2 - 1; i >= 0; --i)
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
        if (empty())
            throw std::underflow_error("heap is empty");
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) sift_down(0);
    }

    const T& top() const {
        if (empty())
            throw std::underflow_error("heap is empty");
        return data_[0];
    }

    bool        empty() const noexcept { return data_.empty(); }
    std::size_t size()  const noexcept { return data_.size(); }

    // Reference to internal data (for bulk operations)
    const std::vector<T>& data() const noexcept { return data_; }

private:
    void sift_up(std::size_t i) {
        while (i > 0) {
            std::size_t p = (i - 1) / 2;
            if (!compare_(data_[p], data_[i])) break;
            std::swap(data_[i], data_[p]);
            i = p;
        }
    }

    void sift_down(std::size_t i) {
        std::size_t n = data_.size();
        while (true) {
            std::size_t largest = i;
            std::size_t l = 2 * i + 1;
            std::size_t r = 2 * i + 2;
            if (l < n && compare_(data_[largest], data_[l]))
                largest = l;
            if (r < n && compare_(data_[largest], data_[r]))
                largest = r;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }

    std::vector<T> data_;
    Compare        compare_;
};

// ---- In-place heap sort ----
template <std::regular T>
void heap_sort(std::span<T> data) {
    if (data.empty()) return;

    // Build max-heap
    for (int i = static_cast<int>(data.size()) / 2 - 1; i >= 0; --i) {
        std::size_t current = static_cast<std::size_t>(i);
        while (true) {
            std::size_t largest = current;
            std::size_t l = 2 * current + 1;
            std::size_t r = 2 * current + 2;
            if (l < data.size() && data[l] > data[largest]) largest = l;
            if (r < data.size() && data[r] > data[largest]) largest = r;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }

    // Extract elements
    for (std::size_t i = data.size() - 1; i > 0; --i) {
        std::swap(data[0], data[i]);
        std::size_t current = 0;
        std::size_t heap_sz = i;
        while (true) {
            std::size_t largest = current;
            std::size_t l = 2 * current + 1;
            std::size_t r = 2 * current + 2;
            if (l < heap_sz && data[l] > data[largest]) largest = l;
            if (r < heap_sz && data[r] > data[largest]) largest = r;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }
}

#endif
