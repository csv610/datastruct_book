#include "two_three_four_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::two_three_four_tree<int, int> tree;
    assert(tree.empty());

    // Insert enough to force 4-node splits
    for (int i = 1; i <= 15; ++i) {
        tree.insert(i * 10, i * 100);
    }
    assert(tree.size() == 15);

    for (int i = 1; i <= 15; ++i) {
        auto* v = tree.find(i * 10);
        assert(v && *v == i * 100);
    }
    assert(!tree.find(0));
    assert(!tree.find(160));

    auto sorted = tree.to_sorted_vector();
    assert(sorted.size() == 15);
    for (int i = 1; i < static_cast<int>(sorted.size()); ++i)
        assert(sorted[i - 1].first < sorted[i].first);

    // Erase
    assert(tree.erase(10));
    assert(!tree.find(10));
    assert(tree.size() == 14);
    assert(!tree.erase(999));

    std::print("All two_three_four_tree tests passed\n");
    return 0;
}
