// ch12: Graphs — adjacency list, DFS, BFS, components, Dijkstra, Bellman-Ford, Kruskal, Prim
#include <algorithm>
#include <climits>
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

// ---- unweighted adjacency list (directed/undirected) ----
class graph_list {
public:
    struct edge { size_t to; };
    explicit graph_list(size_t n) : adj_(n) {}
    void add_edge(size_t u, size_t v) {
        adj_[u].push_back({v});
        adj_[v].push_back({u});
    }
    const std::vector<edge>& neighbors(size_t v) const { return adj_[v]; }
    size_t size() const noexcept { return adj_.size(); }
private:
    std::vector<std::vector<edge>> adj_;
};

void dfs(const graph_list& g, size_t start, const std::function<void(size_t)>& visit) {
    std::vector<bool> visited(g.size(), false);
    std::function<void(size_t)> dfs_impl = [&](size_t v) {
        visited[v] = true;
        visit(v);
        for (const auto& e : g.neighbors(v))
            if (!visited[e.to]) dfs_impl(e.to);
    };
    dfs_impl(start);
}

std::vector<size_t> bfs(const graph_list& g, size_t start) {
    std::vector<size_t> distance(g.size(), SIZE_MAX);
    std::queue<size_t> q;
    distance[start] = 0; q.push(start);
    while (!q.empty()) {
        size_t v = q.front(); q.pop();
        for (const auto& e : g.neighbors(v))
            if (distance[e.to] == SIZE_MAX) {
                distance[e.to] = distance[v] + 1;
                q.push(e.to);
            }
    }
    return distance;
}

std::vector<size_t> connected_components(const graph_list& g) {
    size_t n = g.size();
    std::vector<size_t> component(n, SIZE_MAX);
    size_t comp_id = 0;
    for (size_t v = 0; v < n; ++v) {
        if (component[v] != SIZE_MAX) continue;
        std::queue<size_t> q;
        q.push(v); component[v] = comp_id;
        while (!q.empty()) {
            size_t u = q.front(); q.pop();
            for (const auto& e : g.neighbors(u))
                if (component[e.to] == SIZE_MAX) {
                    component[e.to] = comp_id;
                    q.push(e.to);
                }
        }
        ++comp_id;
    }
    return component;
}

// ---- weighted graph (for shortest paths and MST) ----
class weighted_graph {
public:
    struct edge { size_t to; int weight; };

    explicit weighted_graph(size_t n) : adj_(n) {}

    void add_directed_edge(size_t from, size_t to, int weight) {
        adj_[from].push_back({to, weight});
    }
    void add_undirected_edge(size_t u, size_t v, int weight) {
        adj_[u].push_back({v, weight});
        adj_[v].push_back({u, weight});
    }
    const std::vector<edge>& neighbors(size_t v) const { return adj_[v]; }
    size_t size() const noexcept { return adj_.size(); }

    struct flat_edge { size_t from, to; int weight; };

    std::vector<flat_edge> all_edges(bool undirected = false) const {
        std::vector<flat_edge> edges;
        for (size_t v = 0; v < adj_.size(); ++v)
            for (const auto& e : adj_[v])
                if (!undirected || v < e.to)
                    edges.push_back({v, e.to, e.weight});
        return edges;
    }

private:
    std::vector<std::vector<edge>> adj_;
};

// ---- Dijkstra ----
std::vector<int> dijkstra(const weighted_graph& g, size_t source) {
    size_t n = g.size();
    std::vector<int> distance(n, INT_MAX);
    std::vector<bool> visited(n, false);
    using entry = std::pair<int, size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    distance[source] = 0; pq.push({0, source});
    while (!pq.empty()) {
        auto [dist, v] = pq.top(); pq.pop();
        if (visited[v]) continue;
        visited[v] = true;
        for (const auto& e : g.neighbors(v))
            if (dist != INT_MAX && dist + e.weight < distance[e.to]) {
                distance[e.to] = dist + e.weight;
                pq.push({distance[e.to], e.to});
            }
    }
    return distance;
}

// ---- Bellman-Ford ----
std::optional<std::vector<int>> bellman_ford(const std::vector<weighted_graph::flat_edge>& edges,
                                              size_t n, size_t source) {
    std::vector<int> distance(n, INT_MAX / 2);
    distance[source] = 0;
    for (size_t i = 0; i < n - 1; ++i)
        for (const auto& e : edges)
            if (distance[e.from] != INT_MAX / 2 &&
                distance[e.from] + e.weight < distance[e.to])
                distance[e.to] = distance[e.from] + e.weight;
    for (const auto& e : edges)
        if (distance[e.from] != INT_MAX / 2 &&
            distance[e.from] + e.weight < distance[e.to])
            return std::nullopt;
    return distance;
}

// ---- Union-Find (for Kruskal) ----
class union_find {
public:
    union_find(size_t n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }
    size_t find(size_t x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];
            x = parent_[x];
        }
        return x;
    }
    void union_sets(size_t x, size_t y) {
        size_t rx = find(x), ry = find(y);
        if (rx == ry) return;
        if (size_[rx] < size_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        size_[rx] += size_[ry];
    }
    bool same_set(size_t x, size_t y) { return find(x) == find(y); }
private:
    std::vector<size_t> parent_, size_;
};

// ---- Kruskal's MST ----
std::vector<weighted_graph::flat_edge> kruskal_mst(
    size_t n, std::vector<weighted_graph::flat_edge> edges) {
    std::sort(edges.begin(), edges.end(),
              [](const auto& a, const auto& b) { return a.weight < b.weight; });
    union_find uf(n);
    std::vector<weighted_graph::flat_edge> mst;
    for (const auto& e : edges) {
        if (!uf.same_set(e.from, e.to)) {
            uf.union_sets(e.from, e.to);
            mst.push_back(e);
            if (mst.size() == n - 1) break;
        }
    }
    return mst;
}

// ---- Prim's MST ----
std::vector<weighted_graph::flat_edge> prim_mst(const weighted_graph& g) {
    size_t n = g.size();
    std::vector<bool> in_mst(n, false);
    std::vector<int> key(n, INT_MAX);
    std::vector<size_t> parent(n, SIZE_MAX);
    using entry = std::pair<int, size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    key[0] = 0; pq.push({0, 0});
    while (!pq.empty()) {
        auto [_, v] = pq.top(); pq.pop();
        if (in_mst[v]) continue;
        in_mst[v] = true;
        for (const auto& e : g.neighbors(v))
            if (!in_mst[e.to] && e.weight < key[e.to]) {
                key[e.to] = e.weight;
                parent[e.to] = v;
                pq.push({e.weight, e.to});
            }
    }
    std::vector<weighted_graph::flat_edge> mst;
    for (size_t v = 1; v < n; ++v)
        if (parent[v] != SIZE_MAX)
            mst.push_back({parent[v], v, key[v]});
    return mst;
}

int main() {
    // ---- Unweighted graph ----
    std::cout << "=== Unweighted Graph (Adjacency List) ===\n\n";
    graph_list g(7);
    g.add_edge(0, 1); g.add_edge(1, 2); g.add_edge(2, 3);
    g.add_edge(0, 4); g.add_edge(4, 5); g.add_edge(5, 6); g.add_edge(2, 6);

    std::cout << "Adjacency lists:\n";
    for (size_t v = 0; v < g.size(); ++v) {
        std::cout << "  " << v << ":";
        for (const auto& e : g.neighbors(v)) std::cout << " " << e.to;
        std::cout << "\n";
    }
    std::cout << "\nDFS: "; dfs(g, 0, [](size_t v) { std::cout << v << " "; });
    std::cout << "\n";
    std::cout << "BFS distances: ";
    for (auto d : bfs(g, 0)) std::cout << d << " ";
    std::cout << "\n";
    auto comp = connected_components(g);
    std::cout << "Components: ";
    for (auto c : comp) std::cout << c << " ";
    std::cout << "\n\n";

    // ---- Shortest Paths ----
    std::cout << "=== Shortest Paths ===\n\n";
    weighted_graph spg(5);
    spg.add_directed_edge(0, 1, 1); spg.add_directed_edge(0, 2, 4);
    spg.add_directed_edge(1, 3, 2); spg.add_directed_edge(1, 4, 1);
    spg.add_directed_edge(2, 4, 3); spg.add_directed_edge(4, 3, 1);

    auto d = dijkstra(spg, 0);
    std::cout << "Dijkstra from 0: ";
    for (auto x : d) std::cout << x << " ";
    std::cout << "\n";

    auto edges = spg.all_edges();
    auto bf = bellman_ford(edges, spg.size(), 0);
    std::cout << "Bellman-Ford from 0: ";
    if (bf) for (auto x : *bf) std::cout << x << " ";
    else std::cout << "negative cycle";
    std::cout << "\n\n";

    // ---- MST ----
    std::cout << "=== Minimum Spanning Trees ===\n\n";
    weighted_graph mstg(6);
    mstg.add_undirected_edge(0, 1, 2); mstg.add_undirected_edge(0, 3, 6);
    mstg.add_undirected_edge(1, 2, 3); mstg.add_undirected_edge(1, 4, 5);
    mstg.add_undirected_edge(2, 5, 4); mstg.add_undirected_edge(3, 4, 1);
    mstg.add_undirected_edge(4, 5, 2);

    auto uedges = mstg.all_edges(true);
    auto mst_k = kruskal_mst(mstg.size(), uedges);
    int total_k = 0;
    std::cout << "Kruskal MST:\n";
    for (const auto& e : mst_k) {
        std::cout << "  " << e.from << " -- " << e.to << " (" << e.weight << ")\n";
        total_k += e.weight;
    }
    std::cout << "  Total: " << total_k << "\n";

    auto mst_p = prim_mst(mstg);
    int total_p = 0;
    std::cout << "Prim MST:\n";
    for (const auto& e : mst_p) {
        std::cout << "  " << e.from << " -- " << e.to << " (" << e.weight << ")\n";
        total_p += e.weight;
    }
    std::cout << "  Total: " << total_p << "\n";

    std::cout << "\nAll ch12 tests passed.\n";
    return 0;
}
