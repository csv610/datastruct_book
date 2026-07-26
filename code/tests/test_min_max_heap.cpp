#include "min_max_heap.h"
#include <cassert>
#include <print>
#include <vector>

int main() {
    // Test 1: build from vector
    {
        std::vector<int> v{5, 3, 8, 1, 7};
        dsa::min_max_heap<int> h(v);
        assert(h.min() == 1);
        assert(h.max() == 8);
    }

    // Test 2: push individual elements
    {
        dsa::min_max_heap<int> h;
        h.push(5);
        h.push(3);
        h.push(8);
        h.push(1);
        h.push(7);
        h.push(2);
        h.push(9);
        assert(h.min() == 1);
        assert(h.max() == 9);
    }

    // Test 3: pop_min / pop_max
    {
        dsa::min_max_heap<int> h({4, 1, 7, 3, 9, 2, 8});
        h.pop_min();
        assert(h.min() == 2);

        h.pop_max();
        assert(h.max() == 8);
        assert(h.size() == 5);
    }

    // Test 4: single element
    {
        dsa::min_max_heap<int> single({5});
        assert(single.min() == 5);
        assert(single.max() == 5);
    }

    std::print("All min-max heap tests passed\n");
    return 0;
}
