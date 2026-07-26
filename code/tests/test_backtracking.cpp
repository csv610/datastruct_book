#include "backtracking.h"
#include <cassert>
#include <iostream>

int main() {
    // N-Queens
    {
        auto sols = dsa::n_queens(4);
        assert(sols.size() == 2);
    }
    {
        auto sols = dsa::n_queens(1);
        assert(sols.size() == 1);
        assert(sols[0][0] == "Q");
    }
    {
        auto sols = dsa::n_queens(8);
        assert(sols.size() == 92);
    }

    // Subset sum
    {
        assert(dsa::subset_sum({3, 34, 4, 12, 5, 2}, 9) == true);
    }
    {
        assert(dsa::subset_sum({3, 34, 4, 12, 5, 2}, 30) == false);
    }
    {
        assert(dsa::subset_sum({}, 0) == true);
    }
    {
        assert(dsa::subset_sum({1, 2, 3}, 6) == true);
    }

    // TSP
    {
        std::vector<std::vector<int>> dist = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        assert(dsa::tsp(dist) == 80);  // 0->1->3->2->0 = 10+25+30+15=80
    }
    {
        std::vector<std::vector<int>> dist = {{0}};
        assert(dsa::tsp(dist) == 0);
    }

    std::cout << "All backtracking tests passed\n";
    return 0;
}
