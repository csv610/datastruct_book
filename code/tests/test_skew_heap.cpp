#include "skew_heap.h"
#include <cassert>
#include <print>
#include <vector>

int main() {
    dsa::skew_heap<int> h;
    h.push(5);
    h.push(3);
    h.push(8);
    h.push(1);
    h.push(4);
    assert(h.top() == 1);
    h.pop();
    assert(h.top() == 3);
    h.pop();
    assert(h.top() == 4);

    dsa::skew_heap<int> h2;
    h2.push(2);
    h2.push(6);
    h.meld(std::move(h2));

    std::vector<int> sorted;
    while (!h.empty()) {
        sorted.push_back(h.top());
        h.pop();
    }
    assert((sorted == std::vector<int>{2, 4, 5, 6, 8}));

    dsa::skew_heap<int> empty;
    assert(empty.empty());

    std::print("All skew heap tests passed\n");
    return 0;
}
