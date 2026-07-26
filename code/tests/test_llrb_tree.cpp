#include "llrb_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::llrb_tree<int, int> tree;
    assert(tree.empty());

    for (int i = 1; i <= 30; ++i) {
        tree.insert(i, i * 10);
    }
    assert(tree.size() == 30);

    for (int i = 1; i <= 30; ++i) {
        auto* v = tree.find(i);
        assert(v && *v == i * 10);
    }
    assert(!tree.find(0));
    assert(!tree.find(31));

    auto sorted = tree.to_sorted_vector();
    assert(sorted.size() == 30);
    for (int i = 1; i < static_cast<int>(sorted.size()); ++i)
        assert(sorted[i - 1].first < sorted[i].first);

    // Erase
    assert(tree.erase(15));
    assert(!tree.find(15));
    assert(tree.size() == 29);

    assert(tree.erase(1));
    assert(!tree.find(1));

    assert(tree.erase(30));
    assert(!tree.find(30));

    // Erase all remaining
    for (int i = 2; i <= 29; ++i) {
        if (i != 15) tree.erase(i);
    }
    assert(tree.size() == 0);
    assert(tree.empty());

    std::print("All llrb_tree tests passed\n");
    return 0;
}
