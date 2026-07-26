#include "flow_matching.h"
#include <cassert>
#include <iostream>

int main() {
    // Ford-Fulkerson
    {
        dsa::ford_fulkerson ff(6);
        ff.add_edge(0, 1, 16); ff.add_edge(0, 2, 13);
        ff.add_edge(1, 2, 10); ff.add_edge(1, 3, 12);
        ff.add_edge(2, 1, 4); ff.add_edge(2, 4, 14);
        ff.add_edge(3, 2, 9); ff.add_edge(3, 5, 20);
        ff.add_edge(4, 3, 7); ff.add_edge(4, 5, 4);
        assert(ff.max_flow(0, 5) == 23);
    }

    // Edmonds-Karp
    {
        dsa::edmonds_karp ek(6);
        ek.add_edge(0, 1, 16); ek.add_edge(0, 2, 13);
        ek.add_edge(1, 2, 10); ek.add_edge(1, 3, 12);
        ek.add_edge(2, 1, 4); ek.add_edge(2, 4, 14);
        ek.add_edge(3, 2, 9); ek.add_edge(3, 5, 20);
        ek.add_edge(4, 3, 7); ek.add_edge(4, 5, 4);
        assert(ek.max_flow(0, 5) == 23);
    }

    // Bipartite matching
    {
        dsa::bipartite_matching bm(4, 4);
        bm.add_edge(0, 0); bm.add_edge(0, 1);
        bm.add_edge(1, 1); bm.add_edge(1, 2);
        bm.add_edge(2, 2); bm.add_edge(2, 3);
        bm.add_edge(3, 3);
        assert(bm.max_matching() == 4);
    }
    {
        // No edges
        dsa::bipartite_matching bm(3, 3);
        assert(bm.max_matching() == 0);
    }
    {
        // Star: left[0] connects to all right
        dsa::bipartite_matching bm(3, 3);
        bm.add_edge(0, 0); bm.add_edge(0, 1); bm.add_edge(0, 2);
        assert(bm.max_matching() == 1);
    }

    std::cout << "All flow/matching tests passed\n";
    return 0;
}
