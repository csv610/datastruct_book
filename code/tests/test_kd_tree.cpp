#include "kd_tree.h"
#include <cassert>
#include <cmath>
#include <print>

int main() {
    dsa::kd_tree<2, double>::point pts[] = {
        {{1.0, 2.0}, 0}, {{3.0, 4.0}, 1}, {{5.0, 6.0}, 2},
        {{7.0, 8.0}, 3}, {{2.0, 1.0}, 4}
    };
    dsa::kd_tree<2, double> tree(
        std::vector<dsa::kd_tree<2, double>::point>(pts, pts + 5));

    auto [nn, dist] = tree.nearest({1.5, 2.5});
    assert(std::abs(nn.coords[0] - 1.0) < 0.01);
    assert(std::abs(nn.coords[1] - 2.0) < 0.01);

    auto range = tree.range_query({4.0, 5.0}, 3.0);
    assert(range.size() >= 1);

    std::print("All kd-tree tests passed\n");
    return 0;
}
