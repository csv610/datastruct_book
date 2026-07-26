#include "scc.h"
#include <algorithm>
#include <cassert>
#include <vector>
#include <print>

int main() {
    // Kosaraju's
    {
        dsa::kosaraju_scc ks(8);
        ks.add_edge(0, 1); ks.add_edge(1, 2); ks.add_edge(2, 0);
        ks.add_edge(1, 3); ks.add_edge(3, 4); ks.add_edge(4, 5); ks.add_edge(5, 3);
        ks.add_edge(5, 6); ks.add_edge(6, 7); ks.add_edge(7, 6);

        auto sccs = ks.find_sccs();
        assert(sccs.size() == 3);
        // SCC sizes: {0,1,2}, {3,4,5}, {6,7}
        std::vector<int> sizes;
        for (auto& s : sccs) sizes.push_back(static_cast<int>(s.size()));
        std::sort(sizes.begin(), sizes.end());
        assert(sizes == std::vector<int>({2, 3, 3}));
    }

    // Tarjan's
    {
        dsa::tarjan_scc ts(8);
        ts.add_edge(0, 1); ts.add_edge(1, 2); ts.add_edge(2, 0);
        ts.add_edge(1, 3); ts.add_edge(3, 4); ts.add_edge(4, 5); ts.add_edge(5, 3);
        ts.add_edge(5, 6); ts.add_edge(6, 7); ts.add_edge(7, 6);

        auto sccs = ts.find_sccs();
        assert(sccs.size() == 3);
        std::vector<int> sizes;
        for (auto& s : sccs) sizes.push_back(static_cast<int>(s.size()));
        std::sort(sizes.begin(), sizes.end());
        assert(sizes == std::vector<int>({2, 3, 3}));
    }

    // Single SCC (all strongly connected)
    {
        dsa::tarjan_scc ts(3);
        ts.add_edge(0, 1); ts.add_edge(1, 2); ts.add_edge(2, 0);
        auto sccs = ts.find_sccs();
        assert(sccs.size() == 1);
        assert(sccs[0].size() == 3);
    }

    // No edges: each vertex is its own SCC
    {
        dsa::kosaraju_scc ks(4);
        auto sccs = ks.find_sccs();
        assert(sccs.size() == 4);
    }

    std::print("All SCC tests passed\n");
    return 0;
}
