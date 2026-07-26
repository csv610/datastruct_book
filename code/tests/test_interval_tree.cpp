#include "interval_tree.h"
#include <cassert>
#include <print>

int main() {
    dsa::interval_tree<int>::interval segs[] = {
        {1, 5, 0}, {3, 7, 1}, {8, 10, 2}, {12, 15, 3}, {5, 12, 4}
    };
    dsa::interval_tree<int> tree(std::vector<dsa::interval_tree<int>::interval>(
        segs, segs + 5));

    auto q1 = tree.query(4);
    assert(q1.size() == 2);

    auto q2 = tree.query(6);
    assert(q2.size() == 2);

    auto q3 = tree.query(10);
    assert(q3.size() == 2);

    auto q4 = tree.query(0);
    assert(q4.size() == 0);

    auto q5 = tree.query(16);
    assert(q5.size() == 0);

    auto q6 = tree.query(5);
    assert(q6.size() == 3);

    std::print("All interval tree tests passed\n");
    return 0;
}
