#include "orthogonal_range_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::orthogonal_range_tree<int>::point pts[] = {
        {1, 2, 0}, {3, 4, 1}, {5, 6, 2}, {7, 8, 3}, {2, 1, 4}
    };
    dsa::orthogonal_range_tree<int> tree(
        std::vector<dsa::orthogonal_range_tree<int>::point>(pts, pts + 5));

    auto q1 = tree.query(0, 4, 0, 5);
    assert(q1.size() == 3);

    auto q2 = tree.query(6, 10, 7, 10);
    assert(q2.size() == 1);

    auto q3 = tree.query(10, 20, 10, 20);
    assert(q3.size() == 0);

    std::print("All orthogonal range tree tests passed\n");
    return 0;
}
