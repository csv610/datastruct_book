#include "bplus_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::bplus_tree<int, int, 4> tree;
    assert(tree.empty());
    assert(tree.size() == 0);

    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(5, 50);
    tree.insert(15, 150);
    assert(tree.size() == 4);

    auto* v = tree.find(10);
    assert(v && *v == 100);
    v = tree.find(20);
    assert(v && *v == 200);
    v = tree.find(99);
    assert(!v);

    // Force splits with unique keys
    for (int i = 21; i <= 40; ++i) {
        tree.insert(i * 10, i * 1000);
    }
    assert(tree.size() == 24);

    v = tree.find(210);
    assert(v && *v == 21000);
    v = tree.find(300);
    assert(v && *v == 30000);

    auto sorted = tree.to_sorted_vector();
    assert(sorted.size() == 24);
    for (int i = 1; i < static_cast<int>(sorted.size()); ++i)
        assert(sorted[i - 1].first < sorted[i].first);

    // Range query
    auto rng = tree.range(50, 250);
    assert(rng.size() >= 2);

    // Erase
    assert(tree.erase(10));
    assert(!tree.find(10));
    assert(tree.size() == 23);
    assert(!tree.erase(999));

    std::print("All bplus_tree tests passed\n");
    return 0;
}
