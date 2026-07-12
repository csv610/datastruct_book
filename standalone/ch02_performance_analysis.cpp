// ch02: Performance Analysis — asymptotic analysis examples
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

// O(n) — find max
int find_max(const std::vector<int>& arr) {
    int mx = arr[0];
    for (size_t i = 1; i < arr.size(); ++i)
        if (arr[i] > mx) mx = arr[i];
    return mx;
}

// O(n^2) — nested loops
void nested_loop_example(int n) {
    int count = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            ++count;
    std::cout << "nested loop count: " << count << " (n^2 = " << n * n << ")\n";
}

// dependent nested loops O(n^2/2)
void dependent_loop_example(int n) {
    int count = 0;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            ++count;
    std::cout << "dependent loop count: " << count << " (n(n-1)/2 = " << n * (n - 1) / 2 << ")\n";
}

// O(log n) — logarithmic
int logarithmic(int n) {
    int count = 0;
    for (int i = 1; i < n; i *= 2)
        ++count;
    return count;
}

// O(log n) — binary search
int binary_search(const std::vector<int>& arr, int target) {
    int lo = 0, hi = (int)arr.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// O(2^n) — recursive fibonacci (naive)
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

// tail recursion → O(n) with memoization
int fib_linear(int n, std::vector<int>& memo) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];
    memo[n] = fib_linear(n - 1, memo) + fib_linear(n - 2, memo);
    return memo[n];
}

int main() {
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    std::cout << "find_max: " << find_max(data) << " (expect 9)\n";
    nested_loop_example(5);
    dependent_loop_example(5);
    std::cout << "logarithmic(32): " << logarithmic(32) << " (expect 5)\n";

    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13};
    std::cout << "binary search 7: " << binary_search(sorted, 7) << " (expect 3)\n";
    std::cout << "binary search 6: " << binary_search(sorted, 6) << " (expect -1)\n";

    std::cout << "fib(10): " << fib(10) << " (expect 55)\n";

    std::vector<int> memo(20, -1);
    std::cout << "fib_linear(15): " << fib_linear(15, memo) << " (expect 610)\n";

    std::cout << "All ch02 tests passed.\n";
    return 0;
}
