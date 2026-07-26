#include "scapegoat_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::scapegoat_tree<int, int> tree;
    assert(tree.empty());

    for (int i = 1; i <= 50; ++i) {
        tree.insert(i, i * 10);
    }
    assert(tree.size() == 50);

    for (int i = 1; i <= 50; ++i) {
        auto* v = tree.find(i);
        assert(v && *v == i * 10);
    }
    assert(!tree.find(0));
    assert(!tree.find(51));

    auto sorted = tree.to_sorted_vector();
    assert(sorted.size() == 50);
    for (int i = 1; i < static_cast<int>(sorted.size()); ++i)
        assert(sorted[i - 1].first < sorted[i].first);

    // Erase
    assert(tree.erase(25));
    assert(!tree.find(25));
    assert(tree.size() == 49);

    // Erase more to trigger rebuilds
    for (int i = 1; i <= 25; ++i) {
        tree.erase(i);
    }
    assert(tree.size() == 25);
    auto sorted2 = tree.to_sorted_vector();
    assert(sorted2.size() == 25);
    for (int i = 1; i < static_cast<int>(sorted2.size()); ++i)
        assert(sorted2[i - 1].first < sorted2[i].first);

    std::print("All scapegoat_tree tests passed\n");
    return 0;
}
