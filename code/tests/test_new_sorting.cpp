#include "sorting.h"
#include <algorithm>
#include <cassert>
#include <print>
#include <random>
#include <string>
#include <vector>

int main() {
    // Bubble sort
    {
        std::vector<int> v = {5, 3, 8, 1, 2};
        dsa::bubble_sort(std::span<int>(v));
        assert((v == std::vector<int>{1, 2, 3, 5, 8}));
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        dsa::bubble_sort(std::span<int>(v));
        assert((v == std::vector<int>{1, 2, 3, 4, 5})); // already sorted
    }
    {
        std::vector<int> v = {};
        dsa::bubble_sort(std::span<int>(v));
        assert(v.empty());
    }

    // Shell sort
    {
        std::vector<int> v = {64, 34, 25, 12, 22, 11, 90};
        dsa::shell_sort(std::span<int>(v));
        assert((v == std::vector<int>{11, 12, 22, 25, 34, 64, 90}));
    }
    {
        std::vector<int> v(100);
        for (int i = 0; i < 100; ++i) v[i] = 100 - i;
        dsa::shell_sort(std::span<int>(v));
        for (int i = 0; i < 100; ++i) assert(v[i] == i + 1);
    }

    // Bottom-up merge sort
    {
        std::vector<int> v = {5, 3, 8, 1, 2};
        dsa::merge_sort_bottom_up(std::span<int>(v));
        assert((v == std::vector<int>{1, 2, 3, 5, 8}));
    }
    {
        std::vector<int> v = {3, 1};
        dsa::merge_sort_bottom_up(std::span<int>(v));
        assert((v == std::vector<int>{1, 3}));
    }
    {
        std::vector<int> v = {7, 6, 5, 4, 3, 2, 1};
        dsa::merge_sort_bottom_up(std::span<int>(v));
        assert((v == std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
    }

    // Quick sort 3-way
    {
        std::vector<int> v = {4, 2, 5, 2, 3, 2, 1};
        dsa::quick_sort_3way(std::span<int>(v));
        assert((v == std::vector<int>{1, 2, 2, 2, 3, 4, 5}));
    }
    {
        std::vector<int> v = {3, 3, 3, 3};
        dsa::quick_sort_3way(std::span<int>(v));
        assert((v == std::vector<int>{3, 3, 3, 3}));
    }

    // MSD Radix sort for strings
    {
        std::vector<std::string> v = {"banana", "apple", "cherry", "date", "apricot"};
        dsa::radix_sort_msd(std::span<std::string>(v));
        assert(v[0] == "apple");
        assert(v[1] == "apricot");
        assert(v[2] == "banana");
        assert(v[3] == "cherry");
        assert(v[4] == "date");
    }
    {
        std::vector<std::string> v = {"she", "sells", "seashells", "by", "the", "sea"};
        dsa::radix_sort_msd(std::span<std::string>(v));
        assert(v[0] == "by");
        assert(v[1] == "sea");
        assert(v[2] == "seashells");
        assert(v[3] == "sells");
        assert(v[4] == "she");
        assert(v[5] == "the");
    }

    std::println("All new sorting tests passed");
    return 0;
}
