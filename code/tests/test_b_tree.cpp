#include "b_tree.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    dsa::b_tree<int> bt(3);  // Order 3 B-tree (max 2 keys per node)
    bt.insert(10);
    bt.insert(20);
    bt.insert(5);
    bt.insert(6);
    bt.insert(12);
    bt.insert(30);
    bt.insert(7);
    bt.insert(17);

    assert(bt.contains(10));
    assert(bt.contains(30));
    assert(!bt.contains(99));

    auto sorted = bt.inorder();
    std::vector<int> expected = {5, 6, 7, 10, 12, 17, 20, 30};
    assert(sorted == expected);

    assert(bt.erase(6));
    assert(!bt.contains(6));
    sorted = bt.inorder();
    assert(sorted.size() == 7);

    assert(bt.erase(10));
    assert(!bt.contains(10));
    sorted = bt.inorder();
    assert(sorted.size() == 6);

    // Erase non-existent
    assert(!bt.erase(999));

    // Larger test
    dsa::b_tree<int> bt2(5);  // Order 5
    for (int i = 1; i <= 100; ++i)
        bt2.insert(i);
    for (int i = 1; i <= 100; ++i)
        assert(bt2.contains(i));
    sorted = bt2.inorder();
    assert(sorted.size() == 100);

    for (int i = 1; i <= 50; ++i)
        bt2.erase(i);
    assert(bt2.inorder().size() == 50);
    for (int i = 51; i <= 100; ++i)
        assert(bt2.contains(i));

    std::cout << "All B-tree tests passed\n";
    return 0;
}
