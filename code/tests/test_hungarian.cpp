#include "hungarian.h"
#include <cassert>
#include <print>
#include <vector>

int main() {
    // Simple 3x3 assignment
    {
        std::vector<std::vector<int>> cost = {
            {2500, 4000, 3500},
            {4000, 6000, 3500},
            {2000, 4000, 2500}
        };
        dsa::hungarian h(cost);
        auto result = h.solve();
        assert(result.cost == 9500); // worker 0->1(4000) + worker 1->2(3500) + worker 2->0(2000) = 9500
        // optimal: worker 0->job 1(4000)? No.
        // worker 0->2(3500), worker 1->0(4000), worker 2->1(4000) = 11500
        // worker 0->0(2500), worker 1->2(3500), worker 2->1(4000) = 10000
        // worker 0->2(3500), worker 1->2.. no
        // Best: worker 0->0(2500), worker 1->2(3500), worker 2->1(4000) = 10000
        // Wait, let me check: result.cost == 7500
        assert(result.assignment.size() == 3);
    }

    // 2x2 identity
    {
        std::vector<std::vector<int>> cost = {
            {1, 2},
            {2, 1}
        };
        dsa::hungarian h(cost);
        auto result = h.solve();
        assert(result.cost == 2);
        assert(result.assignment[0] == 0);
        assert(result.assignment[1] == 1);
    }

    // All same cost
    {
        std::vector<std::vector<int>> cost = {
            {5, 5, 5},
            {5, 5, 5},
            {5, 5, 5}
        };
        dsa::hungarian h(cost);
        auto result = h.solve();
        assert(result.cost == 15);
    }

    // 4x4
    {
        std::vector<std::vector<int>> cost = {
            {10, 5, 13, 15},
            {3, 9, 18, 13},
            {10, 7, 2, 4},
            {5, 11, 9, 7}
        };
        dsa::hungarian h(cost);
        auto result = h.solve();
        assert(result.cost == 17); // 5+3+2+7
    }

    std::println("All Hungarian tests passed");
    return 0;
}
