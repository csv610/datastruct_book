#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <std::regular T, typename Compare = std::less<T>>
class max_heap {
public:
    max_heap() = default;

    explicit max_heap(Compare cmp) : compare_(std::move(cmp)) {}

    max_heap(std::initializer_list<T> init, Compare cmp = {})
        : data_(init), compare_(std::move(cmp)) {
        for (int i = static_cast<int>(data_.size()) / 2 - 1; i >= 0; --i)
            sift_down(static_cast<std::size_t>(i));
    }

    // Build heap from existing vector in O(n)
    explicit max_heap(std::vector<T> data, Compare cmp = {})
        : data_(std::move(data)), compare_(std::move(cmp)) {
        for (int i = static_cast<int>(data_.size()) / 2 - 1; i >= 0; --i)
            sift_down(static_cast<std::size_t>(i));
    }

    template <std::input_iterator It>
    max_heap(It begin, It end, Compare cmp = {})
        : data_(begin, end), compare_(std::move(cmp)) {
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

    template <typename... Args>
    void emplace(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
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

    void merge(max_heap& other) {
        data_.insert(data_.end(),
                     std::make_move_iterator(other.data_.begin()),
                     std::make_move_iterator(other.data_.end()));
        other.data_.clear();
        for (int i = static_cast<int>(data_.size()) / 2 - 1; i >= 0; --i)
            sift_down(static_cast<std::size_t>(i));
    }

    void clear() noexcept { data_.clear(); }

    void swap(max_heap& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(compare_, other.compare_);
    }

    bool        empty() const noexcept { return data_.empty(); }
    std::size_t size()  const noexcept { return data_.size(); }

    // Reference to internal data (for bulk operations)
    const std::vector<T>& data() const noexcept { return data_; }

    // Iterator support
    using iterator = typename std::vector<T>::const_iterator;
    iterator begin() const noexcept { return data_.begin(); }
    iterator end() const noexcept { return data_.end(); }

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

template <std::regular T, typename Compare>
void swap(max_heap<T, Compare>& a, max_heap<T, Compare>& b) noexcept {
    a.swap(b);
}

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

}  // namespace dsa

#endif
