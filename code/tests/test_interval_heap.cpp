#include "interval_heap.h"
#include <cassert>
#include <print>

int main() {
    dsa::interval_heap<int> heap;
    assert(heap.empty());

    heap.push(3, 7);
    heap.push(1, 5);
    heap.push(10, 15);
    heap.push(8, 12);
    assert(heap.size() == 4);

    assert(heap.min() == 1);
    assert(heap.max() == 15);

    heap.pop_min();
    assert(heap.size() == 3);
    assert(heap.min() == 3);

    heap.pop_max();
    assert(heap.size() == 2);
    assert(heap.max() == 12);

    // More inserts
    for (int i = 0; i < 10; ++i) {
        heap.push(i * 10, i * 10 + 5);
    }
    assert(heap.size() == 12);
    assert(heap.min() == 0);

    // Pop all
    int prev_min = heap.min();
    while (!heap.empty()) {
        int cur_min = heap.min();
        assert(cur_min >= prev_min);
        heap.pop_min();
        prev_min = cur_min;
    }
    assert(heap.empty());

    std::print("All interval_heap tests passed\n");
    return 0;
}
