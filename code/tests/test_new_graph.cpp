#include "graph.h"
#include <cassert>
#include <print>
#include <vector>

int main() {
    // Bipartiteness check - bipartite graph
    {
        dsa::graph<> g(4);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        g.add_edge(2, 3);
        g.add_edge(3, 0);
        auto color = g.is_bipartite();
        assert(!color.empty());
        assert(color[0] != color[1]);
    }

    // Not bipartite (odd cycle)
    {
        dsa::graph<> g(3);
        g.add_edge(0, 1);
        g.add_edge(1, 2);
        g.add_edge(2, 0);
        auto color = g.is_bipartite();
        assert(color.empty());
    }

    // Transitive closure
    {
        dsa::graph<> g(4);
        g.add_directed_edge(0, 1);
        g.add_directed_edge(1, 2);
        g.add_directed_edge(2, 3);
        auto tc = g.transitive_closure();
        assert(tc[0][3]); // 0 -> 1 -> 2 -> 3
        assert(!tc[3][0]);
    }

    // DAG shortest path
    {
        dsa::graph<int> g(5);
        g.add_directed_edge(0, 1, 1);
        g.add_directed_edge(0, 2, 10);
        g.add_directed_edge(1, 3, 2);
        g.add_directed_edge(2, 3, 3);
        g.add_directed_edge(3, 4, 1);
        auto dist = g.dag_shortest_path(0);
        assert(dist[0] == 0);
        assert(dist[1] == 1);
        assert(dist[2] == 10);
        assert(dist[3] == 3);
        assert(dist[4] == 4);
    }

    // Eulerian cycle (directed)
    {
        dsa::graph<> g(3);
        g.add_directed_edge(0, 1);
        g.add_directed_edge(1, 2);
        g.add_directed_edge(2, 0);
        auto cycle = g.eulerian_cycle();
        assert(cycle.size() == 4); // 3 edges + return to start
    }

    // Boruvka MST
    {
        std::vector<dsa::weighted_edge> edges = {
            {0, 1, 4}, {0, 2, 8}, {1, 2, 11},
            {1, 3, 2}, {2, 3, 7}
        };
        auto mst = dsa::boruvka_mst(4, edges);
        assert(mst.size() == 3);
        int total = 0;
        for (auto& e : mst) total += e.weight;
        assert(total == 13);
    }

    std::println("All new graph tests passed");
    return 0;
}
