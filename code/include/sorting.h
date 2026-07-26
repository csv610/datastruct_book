#ifndef SORTING_H
#define SORTING_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <random>
#include <span>
#include <utility>
#include <vector>

namespace dsa {

// ---- Insertion sort ----
template <std::regular T>
void insertion_sort(std::span<T> a) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        T key = std::move(a[i]);
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = std::move(a[j]);
            --j;
        }
        a[j + 1] = std::move(key);
    }
}

// ---- Merge sort ----
template <std::regular T>
void merge_sort(std::span<T> a) {
    if (a.size() <= 1) return;

    auto mid = a.size() / 2;
    std::vector<T> left(a.begin(), a.begin() + mid);
    std::vector<T> right(a.begin() + mid, a.end());

    merge_sort(std::span<T>(left));
    merge_sort(std::span<T>(right));

    std::size_t i = 0, j = 0, k = 0;
    while (i < left.size() && j < right.size())
        a[k++] = (left[i] <= right[j]) ? std::move(left[i++])
                                       : std::move(right[j++]);
    while (i < left.size()) a[k++] = std::move(left[i++]);
    while (j < right.size()) a[k++] = std::move(right[j++]);
}

// ---- Quick sort (randomized) ----
template <std::regular T>
std::size_t partition(std::span<T> a) {
    // Random pivot
    std::size_t r = std::rand() % a.size();
    std::swap(a[r], a.back());

    T pivot = std::move(a.back());
    std::size_t i = 0;

    for (std::size_t j = 0; j < a.size() - 1; ++j) {
        if (a[j] <= pivot) {
            std::swap(a[i], a[j]);
            ++i;
        }
    }
    std::swap(a[i], a.back());
    return i;
}

template <std::regular T>
void quick_sort(std::span<T> a) {
    if (a.size() <= 1) return;
    auto p = partition(a);
    quick_sort(a.subspan(0, p));
    quick_sort(a.subspan(p + 1));
}

// ---- Quickselect (find k-th smallest) ----
template <std::regular T>
T quick_select(std::span<T> a, std::size_t k) {
    if (a.size() == 1) return a[0];

    auto p = partition(a);
    if (k == p) return a[p];
    if (k < p)  return quick_select(a.subspan(0, p), k);
    return quick_select(a.subspan(p + 1), k - p - 1);
}

// ---- Counting sort (non-negative integers) ----
inline void counting_sort(std::span<int> a) {
    if (a.empty()) return;
    int max_val = *std::max_element(a.begin(), a.end());
    int min_val = *std::min_element(a.begin(), a.end());
    std::size_t range = static_cast<std::size_t>(max_val - min_val + 1);
    std::vector<std::size_t> count(range, 0);

    for (auto v : a)
        ++count[static_cast<std::size_t>(v - min_val)];

    for (std::size_t i = 1; i < range; ++i)
        count[i] += count[i - 1];

    std::vector<int> output(a.size());
    for (auto it = a.rbegin(); it != a.rend(); ++it) {
        auto idx = static_cast<std::size_t>(*it - min_val);
        output[--count[idx]] = *it;
    }

    std::copy(output.begin(), output.end(), a.begin());
}

// ---- Radix sort (non-negative integers, LSD base-10) ----
inline void radix_sort(std::span<int> a) {
    if (a.empty()) return;
    int max_val = *std::max_element(a.begin(), a.end());

    std::vector<int> output(a.size());
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        std::size_t count[10] = {};
        for (auto v : a)
            ++count[(v / exp) % 10];

        for (std::size_t i = 1; i < 10; ++i)
            count[i] += count[i - 1];

        for (auto it = a.rbegin(); it != a.rend(); ++it) {
            auto digit = static_cast<std::size_t>((*it / exp) % 10);
            output[--count[digit]] = *it;
        }

        std::copy(output.begin(), output.end(), a.begin());
    }
}

// ---- Bucket sort (uniform distribution in [0,1)) ----
inline void bucket_sort(std::span<float> a) {
    std::size_t n = a.size();
    if (n <= 1) return;

    std::vector<std::vector<float>> buckets(n);
    for (auto v : a) {
        auto idx = static_cast<std::size_t>(v * n);
        if (idx >= n) idx = n - 1;
        buckets[idx].push_back(v);
    }

    for (auto& bucket : buckets)
        insertion_sort(std::span<float>(bucket));

    std::size_t k = 0;
    for (auto& bucket : buckets)
        for (auto v : bucket)
            a[k++] = v;
}

}  // namespace dsa

#endif
