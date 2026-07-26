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

// ---- Bubble sort ----
template <std::regular T>
void bubble_sort(std::span<T> a) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        bool swapped = false;
        for (std::size_t j = 0; j + 1 < a.size() - i; ++j) {
            if (a[j + 1] < a[j]) {
                std::swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// ---- Shell sort (Ciura gaps) ----
template <std::regular T>
void shell_sort(std::span<T> a) {
    std::vector<std::size_t> gaps = {701, 301, 132, 57, 23, 10, 4, 1};
    for (auto gap : gaps) {
        for (std::size_t i = gap; i < a.size(); ++i) {
            T temp = std::move(a[i]);
            std::size_t j = i;
            while (j >= gap && a[j - gap] > temp) {
                a[j] = std::move(a[j - gap]);
                j -= gap;
            }
            a[j] = std::move(temp);
        }
    }
}

// ---- Bottom-up merge sort (iterative) ----
template <std::regular T>
void merge_sort_bottom_up(std::span<T> a) {
    std::size_t n = a.size();
    std::vector<T> aux(n);
    for (std::size_t sz = 1; sz < n; sz *= 2) {
        for (std::size_t lo = 0; lo < n; lo += 2 * sz) {
            std::size_t mid = std::min(lo + sz, n);
            std::size_t hi = std::min(lo + 2 * sz, n);
            std::size_t i = lo, j = mid, k = lo;
            while (i < mid && j < hi)
                aux[k++] = (a[i] <= a[j]) ? std::move(a[i++]) : std::move(a[j++]);
            while (i < mid) aux[k++] = std::move(a[i++]);
            while (j < hi) aux[k++] = std::move(a[j++]);
        }
        std::copy(aux.begin(), aux.end(), a.begin());
    }
}

// ---- Quick sort 3-way partition (Dutch National Flag) ----
template <std::regular T>
void quick_sort_3way(std::span<T> a) {
    if (a.size() <= 1) return;

    std::size_t lt = 0, gt = a.size() - 1, i = 0;
    T pivot = a[a.size() / 2];
    while (i <= gt) {
        if (a[i] < pivot) {
            std::swap(a[lt++], a[i++]);
        } else if (a[i] > pivot) {
            std::swap(a[i], a[gt--]);
        } else {
            ++i;
        }
    }
    if (lt > 0)
        quick_sort_3way(a.subspan(0, lt));
    if (gt + 1 < a.size())
        quick_sort_3way(a.subspan(gt + 1));
}

// ---- MSD Radix sort for strings ----
inline void radix_sort_msd(std::span<std::string> a, int radix = 256) {
    std::vector<std::string> aux(a.size());

    std::function<void(int, int, int)> sort = [&](int lo, int hi, int d) {
        if (hi <= lo) return;
        std::vector<int> count(radix + 2, 0);
        for (int i = lo; i <= hi; ++i) {
            int c = (d < static_cast<int>(a[i].size())) ? static_cast<unsigned char>(a[i][d]) : -1;
            ++count[c + 2];
        }
        for (int r = 0; r < radix + 1; ++r)
            count[r + 1] += count[r];
        for (int i = lo; i <= hi; ++i) {
            int c = (d < static_cast<int>(a[i].size())) ? static_cast<unsigned char>(a[i][d]) : -1;
            aux[count[c + 1]++] = std::move(a[i]);
        }
        for (int i = lo; i <= hi; ++i)
            a[i] = std::move(aux[i - lo]);
        for (int r = 0; r < radix; ++r)
            sort(lo + (r == 0 ? 0 : count[r]), lo + count[r + 1] - 1, d + 1);
    };

    sort(0, static_cast<int>(a.size()) - 1, 0);
}

}  // namespace dsa

#endif
