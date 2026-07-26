#include "prim.h"
#include <cassert>
#include <tuple>
#include <vector>
#include <print>

int main() {
    {
        dsa::prim_mst pm(5);
        pm.add_edge(0, 1, 2);
        pm.add_edge(0, 3, 6);
        pm.add_edge(1, 2, 3);
        pm.add_edge(1, 3, 8);
        pm.add_edge(1, 4, 5);
        pm.add_edge(2, 4, 7);
        pm.add_edge(3, 4, 9);

        auto res = pm.compute();
        assert(res.total_weight == 16);  // 2 + 3 + 5 + 6 = 16
        assert(res.edges.size() == 4);
    }

    // Single vertex
    {
        dsa::prim_mst pm(1);
        auto res = pm.compute();
        assert(res.total_weight == 0);
        assert(res.edges.empty());
    }

    // Path graph
    {
        dsa::prim_mst pm(4);
        pm.add_edge(0, 1, 1);
        pm.add_edge(1, 2, 2);
        pm.add_edge(2, 3, 3);
        auto res = pm.compute();
        assert(res.total_weight == 6);
    }

    // Fully connected triangle
    {
        dsa::prim_mst pm(3);
        pm.add_edge(0, 1, 1);
        pm.add_edge(1, 2, 2);
        pm.add_edge(0, 2, 3);
        auto res = pm.compute();
        assert(res.total_weight == 3);  // edges 0-1 and 1-2
    }

    std::print("All Prim MST tests passed\n");
    return 0;
}
