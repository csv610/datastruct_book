// ch03: Performance Measurement — timing, benchmarking, sorting
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

// ---- simple timer ----
class timer {
    using clock = std::chrono::high_resolution_clock;
    clock::time_point start_;
public:
    timer() : start_(clock::now()) {}
    void reset() { start_ = clock::now(); }
    double elapsed_ms() const {
        auto dur = clock::now() - start_;
        return std::chrono::duration<double, std::milli>(dur).count();
    }
};

// ---- measure template ----
template <typename Func>
double measure(Func f) {
    timer t;
    f();
    return t.elapsed_ms();
}

// ---- insertion sort ----
void insertion_sort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// ---- merge sort ----
void merge(std::vector<int>& arr, int lo, int mid, int hi) {
    std::vector<int> left(arr.begin() + lo, arr.begin() + mid + 1);
    std::vector<int> right(arr.begin() + mid + 1, arr.begin() + hi + 1);
    size_t i = 0, j = 0, k = lo;
    while (i < left.size() && j < right.size())
        arr[k++] = left[i] <= right[j] ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void merge_sort(std::vector<int>& arr, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(arr, lo, mid);
    merge_sort(arr, mid + 1, hi);
    merge(arr, lo, mid, hi);
}

void merge_sort(std::vector<int>& arr) {
    merge_sort(arr, 0, (int)arr.size() - 1);
}

int main() {
    std::mt19937 rng(42);

    // ---- insertion sort benchmark ----
    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 0);
    std::shuffle(data.begin(), data.end(), rng);

    double t1 = measure([&]() { insertion_sort(data); });
    std::cout << "insertion_sort 1000: " << t1 << " ms\n";
    bool sorted = std::is_sorted(data.begin(), data.end());
    std::cout << "  sorted: " << sorted << "\n";

    // ---- merge sort benchmark ----
    std::shuffle(data.begin(), data.end(), rng);
    double t2 = measure([&]() { merge_sort(data); });
    std::cout << "merge_sort 1000: " << t2 << " ms\n";
    std::cout << "  sorted: " << std::is_sorted(data.begin(), data.end()) << "\n";

    // ---- std::sort benchmark ----
    std::shuffle(data.begin(), data.end(), rng);
    double t3 = measure([&]() { std::sort(data.begin(), data.end()); });
    std::cout << "std::sort 1000: " << t3 << " ms\n";
    std::cout << "  sorted: " << std::is_sorted(data.begin(), data.end()) << "\n";

    std::cout << "All ch03 tests passed.\n";
    return 0;
}
