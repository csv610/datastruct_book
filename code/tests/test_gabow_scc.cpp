#include "scc.h"
#include <algorithm>
#include <cassert>
#include <print>
#include <vector>

int main() {
    // Gabow SCC - same test as Kosaraju and Tarjan
    {
        // Graph: 0->1, 1->2, 2->0, 1->3, 3->4, 4->5, 5->3
        dsa::gabow_scc g(6);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        g.add_edge(2, 0);
        g.add_edge(1, 3);
        g.add_edge(3, 4);
        g.add_edge(4, 5);
        g.add_edge(5, 3);

        auto sccs = g.find_sccs();
        // Sort each SCC and the SCCs themselves for comparison
        for (auto& scc : sccs) std::sort(scc.begin(), scc.end());
        std::sort(sccs.begin(), sccs.end());

        assert(sccs.size() == 2);
        assert((sccs[0] == std::vector<int>{0, 1, 2}));
        assert((sccs[1] == std::vector<int>{3, 4, 5}));
    }

    // Single node
    {
        dsa::gabow_scc g(1);
        auto sccs = g.find_sccs();
        assert(sccs.size() == 1);
        assert(sccs[0] == std::vector<int>{0});
    }

    // DAG - each node is its own SCC
    {
        dsa::gabow_scc g(3);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        auto sccs = g.find_sccs();
        assert(sccs.size() == 3);
    }

    std::println("All Gabow SCC tests passed");
    return 0;
}
