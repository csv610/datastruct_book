#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <random>
#include <cassert>

// ch02: Performance Analysis - Test complexity analysis, timing, recursion

// 1. Function growth rate comparison
void growth_rates() {
    std::cout << "=== Growth Rate Comparison ===\n";
    std::vector<int> sizes = {10, 100, 1000, 10000};
    for (int n : sizes) {
        double logn = std::log2(n);
        double nlogn = n * logn;
        double n2 = n * n;
        double n3 = n * n * n;
        double exp2 = std::pow(2, n);
        std::cout << "n=" << n << ": log n=" << logn 
                  << ", n log n=" << nlogn << ", n^2=" << n2 
                  << ", n^3=" << n3 << ", 2^n=" << exp2 << '\n';
    }
}

// 2. Timing utility
template <typename Func>
double time_it(Func f, int iterations = 1000) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count() / iterations;
}

// 3. Fibonacci - exponential recursive
int fib_recursive(int n) {
    if (n <= 1) return n;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

// 4. Fibonacci - linear DP
int fib_dp(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// 5. Fibonacci - O(log n) matrix
int fib_fast(int n) {
    if (n <= 1) return n;
    long long a = 1, b = 1, c = 1, d = 0;
    long long p = 1, q = 0, r = 0, s = 1;
    while (n > 0) {
        if (n & 1) {
            long long tp = p * a + q * c;
            long long tq = p * b + q * d;
            p = tp; q = tq;
        }
        long long ta = a * a + b * c;
        long long tb = a * b + b * d;
        long long tc = c * a + d * c;
        long long td = c * b + d * d;
        a = ta; b = tb; c = tc; d = td;
        n >>= 1;
    }
    return p;
}

// 6. Loop invariant example: Insertion Sort
void insertion_sort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
        // Invariant: arr[0..i] is sorted
    }
}

// 7. Binary search with loop invariant
int binary_search(const std::vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    // Invariant: target is in arr[left..right] if present
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// 8. Space complexity: recursion stack
int recursive_sum(int n) {
    if (n <= 0) return 0;
    return n + recursive_sum(n - 1);
}

int iterative_sum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; ++i) sum += i;
    return sum;
}

// 9. Amortized analysis: dynamic array
class DynamicArray {
    int* data_;
    size_t size_, capacity_;
public:
    DynamicArray() : data_(new int[1]), size_(0), capacity_(1) {}
    ~DynamicArray() { delete[] data_; }
    
    void push_back(int val) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            int* new_data = new int[capacity_];
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
            data_ = new_data;
        }
        data_[size_++] = val;
    }
    size_t size() const { return size_; }
    int operator[](size_t i) const { return data_[i]; }
};

int main() {
    std::cout << "=== Chapter 2: Performance Analysis ===\n\n";
    
    // 1. Growth rates
    growth_rates();
    std::cout << '\n';
    
    // 2. Timing comparison
    std::cout << "=== Timing Comparison (microseconds) ===\n";
    double t_rec = time_it([&]{ fib_recursive(20); }, 100);
    double t_dp = time_it([&]{ fib_dp(20); }, 10000);
    double t_fast = time_it([&]{ fib_fast(20); }, 10000);
    std::cout << "fib_recursive(20): " << t_rec << " µs\n";
    std::cout << "fib_dp(20):        " << t_dp << " µs\n";
    std::cout << "fib_fast(20):      " << t_fast << " µs\n";
    std::cout << "Speedup (recursive vs DP): " << t_rec / t_dp << "x\n\n";
    
    // 3. Correctness verification
    assert(fib_recursive(10) == 55);
    assert(fib_dp(10) == 55);
    assert(fib_fast(10) == 55);
    std::cout << "=== Fibonacci Correctness ===\n";
    for (int i = 0; i <= 15; ++i) {
        assert(fib_recursive(i) == fib_dp(i));
        assert(fib_dp(i) == fib_fast(i));
    }
    std::cout << "All Fibonacci implementations match up to 15\n\n";
    
    // 4. Insertion sort with invariant
    std::vector<int> arr = {5, 2, 8, 1, 9, 3};
    std::vector<int> sorted = {1, 2, 3, 5, 8, 9};
    insertion_sort(arr);
    assert(arr == sorted);
    std::cout << "=== Insertion Sort ===\n";
    std::cout << "Sorted: ";
    for (int x : arr) std::cout << x << ' ';
    std::cout << "\n\n";
    
    // 5. Binary search
    std::vector<int> search_arr = {1, 3, 5, 7, 9, 11, 13};
    assert(binary_search(search_arr, 7) == 3);
    assert(binary_search(search_arr, 1) == 0);
    assert(binary_search(search_arr, 13) == 6);
    assert(binary_search(search_arr, 4) == -1);
    std::cout << "=== Binary Search ===\n";
    std::cout << "Found 7 at index: " << binary_search(search_arr, 7) << '\n';
    std::cout << "Not found 4: " << binary_search(search_arr, 4) << "\n\n";
    
    // 6. Space complexity
    std::cout << "=== Space Complexity ===\n";
    std::cout << "recursive_sum(100) = " << recursive_sum(100) 
              << " (uses O(n) stack)\n";
    std::cout << "iterative_sum(100) = " << iterative_sum(100) 
              << " (uses O(1) space)\n\n";
    
    // 7. Amortized dynamic array
    std::cout << "=== Amortized Dynamic Array ===\n";
    DynamicArray da;
    for (int i = 0; i < 20; ++i) da.push_back(i);
    std::cout << "Pushed 20 elements, size: " << da.size() << '\n';
    std::cout << "Elements: ";
    for (size_t i = 0; i < da.size(); ++i) std::cout << da[i] << ' ';
    std::cout << "\n\n";
    
    // 8. Smoothed analysis concept
    std::cout << "=== Smoothed Analysis (Spielman-Teng) ===\n";
    std::cout << "Worst-case: O(n^2) for simplex, quicksort\n";
    std::cout << "Smoothed:   O(n^2) for simplex, O(n log n) for quicksort\n";
    std::cout << "Small random perturbations eliminate pathological cases\n\n";
    
    std::cout << "=== All tests passed ===\n";
    return 0;
}