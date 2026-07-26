#include "dynamic_programming.h"
#include <cassert>
#include <iostream>

int main() {
    // 0/1 Knapsack
    {
        std::vector<dsa::knapsack_item> items = {{2, 40}, {3, 50}, {2, 70}, {5, 90}};
        int result = dsa::knapsack_01(7, items);
        // {2,40}+{3,50}+{2,70} = w7 v160
        assert(result == 160);
    }
    {
        std::vector<dsa::knapsack_item> items = {{10, 60}, {20, 100}, {30, 120}};
        assert(dsa::knapsack_01(50, items) == 220);
    }
    {
        std::vector<dsa::knapsack_item> items = {{5, 10}};
        assert(dsa::knapsack_01(3, items) == 0);
    }

    // LCS
    {
        std::string a = "ABCBDAB", b = "BDCAB";
        assert(dsa::lcs(a, b).size() == 4);  // "BCAB" or "BDAB"
    }
    {
        assert(dsa::lcs("AGGTAB", "GXTXAYB") == "GTAB");
    }
    {
        assert(dsa::lcs("abc", "def") == "");
    }
    {
        assert(dsa::lcs("abc", "abc") == "abc");
    }

    // Edit distance
    {
        assert(dsa::edit_distance("kitten", "sitting") == 3);
    }
    {
        assert(dsa::edit_distance("saturday", "sunday") == 3);
    }
    {
        assert(dsa::edit_distance("abc", "abc") == 0);
    }
    {
        assert(dsa::edit_distance("", "abc") == 3);
    }

    // LIS
    {
        assert(dsa::lis_length({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    }
    {
        assert(dsa::lis_length({0, 1, 0, 3, 2, 3}) == 4);
    }
    {
        assert(dsa::lis_length({7, 7, 7, 7}) == 1);
    }

    std::cout << "All DP tests passed\n";
    return 0;
}
