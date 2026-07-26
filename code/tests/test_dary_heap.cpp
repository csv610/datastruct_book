#include "dary_heap.h"
#include <cassert>
#include <print>
#include <string>
#include <vector>

int main() {
    // Basic operations
    {
        dsa::dary_heap<int, 4> h;
        h.push(5);
        h.push(3);
        h.push(8);
        h.push(1);
        h.push(7);
        assert(h.top() == 1);
        h.pop();
        assert(h.top() == 3);
        h.pop();
        assert(h.top() == 5);
    }

    // Build heap from vector
    {
        dsa::dary_heap<int, 3> h({5, 3, 8, 1, 7, 2});
        assert(h.top() == 1);
        assert(h.size() == 6);
    }

    // 2-ary heap (binary heap)
    {
        dsa::dary_heap<int, 2> h;
        for (int i = 10; i > 0; --i) h.push(i);
        assert(h.top() == 1);
        h.pop();
        assert(h.top() == 2);
    }

    // Max-heap variant
    {
        dsa::dary_heap<int, 4, std::less<int>> h;
        h.push(5);
        h.push(3);
        h.push(8);
        h.push(1);
        assert(h.top() == 8);
    }

    // Extract all in order
    {
        dsa::dary_heap<int, 5> h;
        std::vector<int> input = {20, 5, 15, 1, 10, 25};
        for (auto v : input) h.push(v);
        std::vector<int> sorted;
        while (!h.empty()) {
            sorted.push_back(h.top());
            h.pop();
        }
        assert((sorted == std::vector<int>{1, 5, 10, 15, 20, 25}));
    }

    std::println("All d-ary heap tests passed");
    return 0;
}
