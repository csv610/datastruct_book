#ifndef SORTING_H
#define SORTING_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <random>
#include <span>
#include <utility>
#include <vector>

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

    merge_sort(left);
    merge_sort(right);

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

#endif
