#include "veb_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::veb_tree<64> tree;
    assert(tree.empty());

    tree.insert(3);
    tree.insert(1);
    tree.insert(5);
    tree.insert(7);
    tree.insert(2);
    assert(tree.size() == 5);
    assert(tree.min_val() == 1);
    assert(tree.max_val() == 7);

    assert(tree.contains(3));
    assert(tree.contains(7));
    assert(!tree.contains(4));

    assert(tree.successor(3) == 5);
    assert(tree.successor(1) == 2);
    assert(tree.successor(7) == -1);
    assert(tree.predecessor(3) == 2);
    assert(tree.predecessor(5) == 3);
    assert(tree.predecessor(1) == -1);

    tree.erase(3);
    assert(!tree.contains(3));
    assert(tree.size() == 4);
    assert(tree.successor(2) == 5);

    tree.erase(1);
    assert(tree.min_val() == 2);
    tree.erase(7);
    assert(tree.max_val() == 5);

    // Fill more to test cluster handling
    for (int i = 10; i < 30; ++i) tree.insert(i);
    assert(tree.size() == 22);
    assert(tree.min_val() == 2);
    assert(tree.max_val() == 29);
    assert(tree.successor(20) == 21);
    assert(tree.predecessor(10) == 5);

    std::print("All veb_tree tests passed\n");
    return 0;
}
