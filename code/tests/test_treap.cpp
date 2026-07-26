#include "treap.h"
#include <cassert>
#include <vector>
#include <print>

int main() {
    dsa::treap<int> t;
    t.insert(5);
    t.insert(3);
    t.insert(8);
    t.insert(1);
    t.insert(4);
    t.insert(7);
    t.insert(9);

    assert(t.size() == 7);
    assert(t.contains(5));
    assert(t.contains(1));
    assert(!t.contains(10));

    auto sorted = t.inorder();
    std::vector<int> expected = {1, 3, 4, 5, 7, 8, 9};
    assert(sorted == expected);

    assert(t.erase(3));
    assert(!t.contains(3));
    sorted = t.inorder();
    assert(sorted.size() == 6);

    assert(t.erase(5));
    sorted = t.inorder();
    expected = {1, 4, 7, 8, 9};
    assert(sorted == expected);

    assert(!t.erase(100));

    // Stress test
    dsa::treap<int> t2;
    for (int i = 0; i < 500; ++i)
        t2.insert(i);
    assert(t2.size() == 500);
    for (int i = 0; i < 500; ++i)
        assert(t2.contains(i));
    for (int i = 0; i < 250; ++i)
        t2.erase(i);
    sorted = t2.inorder();
    assert(sorted.size() == 250);
    assert(sorted[0] == 250);

    std::print("All treap tests passed\n");
    return 0;
}
