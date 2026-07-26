#include "r_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::r_tree<2> tree;
    assert(tree.empty());

    tree.insert({{0.0, 0.0}, 0});
    tree.insert({{1.0, 1.0}, 1});
    tree.insert({{2.0, 2.0}, 2});
    tree.insert({{5.0, 5.0}, 3});
    tree.insert({{3.0, 3.0}, 4});
    assert(tree.size() == 5);

    // Range query: find points in [1, 3] x [1, 3]
    dsa::r_tree<2>::rect q = {{{1.0, 1.0}}, {{3.0, 3.0}}};
    auto result = tree.range_query(q);
    assert(result.size() >= 2);

    // Nearest neighbor
    auto nn = tree.nearest({{1.1, 1.1}});
    assert(nn.id == 1);

    auto nn2 = tree.nearest({{4.9, 4.9}});
    assert(nn2.id == 3);

    std::print("All r_tree tests passed\n");
    return 0;
}
