#include "ab_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::ab_tree<int, int, 2, 5> tree;
    assert(tree.empty());

    for (int i = 1; i <= 20; ++i) {
        tree.insert(i, i * 10);
    }
    assert(tree.size() == 20);

    for (int i = 1; i <= 20; ++i) {
        auto* v = tree.find(i);
        assert(v && *v == i * 10);
    }
    assert(!tree.find(0));
    assert(!tree.find(21));

    auto sorted = tree.to_sorted_vector();
    assert(sorted.size() == 20);
    for (int i = 1; i < static_cast<int>(sorted.size()); ++i)
        assert(sorted[i - 1].first < sorted[i].first);

    std::print("All ab_tree tests passed\n");
    return 0;
}
