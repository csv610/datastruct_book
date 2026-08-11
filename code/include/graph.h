#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <concepts>
#include <deque>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

// ---- Graph (adjacency list, undirected) ----
template <typename Weight = int>
class graph {
public:
    struct edge {
        std::size_t to;
        Weight weight;
    };

    explicit graph(std::size_t n) : adj_(n) {}

    void add_edge(std::size_t u, std::size_t v, Weight w = Weight{}) {
        adj_[u].push_back({v, w});
        adj_[v].push_back({u, w});
    }

    void add_directed_edge(std::size_t from, std::size_t to, Weight w = Weight{}) {
        adj_[from].push_back({to, w});
    }

    const std::vector<edge>& neighbors(std::size_t v) const {
        return adj_[v];
    }

    std::size_t vertex_count()  const noexcept { return adj_.size(); }
    std::size_t edge_count() const {
        std::size_t e = 0;
        for (const auto& adj : adj_) e += adj.size();
        return e / 2;
    }

    // ---- Depth-First Search ----
    template <typename Visitor>
    void dfs(std::size_t start, Visitor&& visit) const {
        std::vector<bool> visited(adj_.size(), false);
        std::function<void(std::size_t)> impl = [&](std::size_t v) {
            visited[v] = true;
            visit(v);
            for (const auto& [to, _] : adj_[v])
                if (!visited[to]) impl(to);
        };
        impl(start);
    }

    // ---- Breadth-First Search ----
    template <typename Visitor>
    void bfs(std::size_t start, Visitor&& visit) const {
        std::vector<bool> visited(adj_.size(), false);
        std::queue<std::size_t> q;
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            std::size_t v = q.front(); q.pop();
            visit(v);
            for (const auto& [to, _] : adj_[v]) {
                if (!visited[to]) {
                    visited[to] = true;
                    q.push(to);
                }
            }
        }
    }

    // ---- Connected components ----
    std::vector<std::size_t> connected_components() const {
        std::size_t n = adj_.size();
        std::vector<std::size_t> comp(n, n);  // n = sentinel
        std::size_t comp_id = 0;

        for (std::size_t v = 0; v < n; ++v) {
            if (comp[v] != n) continue;
            std::queue<std::size_t> q;
            q.push(v);
            comp[v] = comp_id;
            while (!q.empty()) {
                auto u = q.front(); q.pop();
                for (const auto& [to, _] : adj_[u])
                    if (comp[to] == n) {
                        comp[to] = comp_id;
                        q.push(to);
                    }
            }
            ++comp_id;
        }
        return comp;
    }

    // ---- BFS distances (unweighted) ----
    std::vector<std::size_t> bfs_distances(std::size_t start) const {
        std::size_t n = adj_.size();
        std::vector<std::size_t> dist(n, n);  // n = "infinity"
        std::queue<std::size_t> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            auto v = q.front(); q.pop();
            for (const auto& [to, _] : adj_[v])
                if (dist[to] == n) {
                    dist[to] = dist[v] + 1;
                    q.push(to);
                }
        }
        return dist;
    }

    // ---- Dijkstra (non-negative weights) ----
    std::vector<Weight> dijkstra(std::size_t start) const {
        std::size_t n = adj_.size();
        std::vector<Weight> dist(n, std::numeric_limits<Weight>::max());
        using entry = std::pair<Weight, std::size_t>;
        std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;

        dist[start] = Weight{};
        pq.push({Weight{}, start});

        while (!pq.empty()) {
            auto [d, v] = pq.top(); pq.pop();
            if (d != dist[v]) continue;
            for (const auto& [to, w] : adj_[v]) {
                if (dist[v] + w < dist[to]) {
                    dist[to] = dist[v] + w;
                    pq.push({dist[to], to});
                }
            }
        }
        return dist;
    }

    // ---- Topological sort (directed acyclic) ----
    std::vector<std::size_t> topological_sort() const {
        std::size_t n = adj_.size();
        std::vector<std::size_t> in_degree(n, 0);
        for (std::size_t v = 0; v < n; ++v)
            for (const auto& [to, _] : adj_[v])
                ++in_degree[to];

        std::queue<std::size_t> q;
        for (std::size_t v = 0; v < n; ++v)
            if (in_degree[v] == 0) q.push(v);

        std::vector<std::size_t> result;
        while (!q.empty()) {
            auto v = q.front(); q.pop();
            result.push_back(v);
            for (const auto& [to, _] : adj_[v])
                if (--in_degree[to] == 0) q.push(to);
        }
        return result;
    }

    // ---- Bellman-Ford (handles negative weights, detects negative cycles) ----
    // Returns empty vector if negative cycle detected
    std::vector<Weight> bellman_ford(std::size_t start) const {
        std::size_t n = adj_.size();
        std::vector<Weight> dist(n, std::numeric_limits<Weight>::max());
        dist[start] = Weight{};

        for (std::size_t iter = 0; iter < n; ++iter) {
            bool changed = false;
            for (std::size_t v = 0; v < n; ++v) {
                if (dist[v] == std::numeric_limits<Weight>::max()) continue;
                for (const auto& [to, w] : adj_[v]) {
                    if (dist[v] + w < dist[to]) {
                        dist[to] = dist[v] + w;
                        changed = true;
                    }
                }
            }
            if (!changed) break;
            if (iter == n - 1) return {};  // negative cycle
        }
        return dist;
    }

    // ---- Floyd-Warshall (all-pairs shortest paths) ----
    // Returns empty matrix if negative cycle exists
    std::vector<std::vector<Weight>> floyd_warshall() const {
        std::size_t n = adj_.size();
        auto inf = std::numeric_limits<Weight>::max();
        std::vector<std::vector<Weight>> dist(n, std::vector<Weight>(n, inf));

        for (std::size_t i = 0; i < n; ++i) dist[i][i] = Weight{};

        for (std::size_t v = 0; v < n; ++v)
            for (const auto& [to, w] : adj_[v])
                dist[v][to] = w;

        for (std::size_t k = 0; k < n; ++k)
            for (std::size_t i = 0; i < n; ++i)
                for (std::size_t j = 0; j < n; ++j)
                    if (dist[i][k] != inf && dist[k][j] != inf)
                        if (dist[i][k] + dist[k][j] < dist[i][j])
                            dist[i][j] = dist[i][k] + dist[k][j];

        for (std::size_t i = 0; i < n; ++i)
            if (dist[i][i] < Weight{}) return {};

        return dist;
    }

    // ---- Cycle detection (directed graph) ----
    bool has_cycle() const {
        std::size_t n = adj_.size();
        enum State { Unvisited, InStack, Done };
        std::vector<State> state(n, Unvisited);

        std::function<bool(std::size_t)> dfs_cycle = [&](std::size_t v) -> bool {
            state[v] = InStack;
            for (const auto& [to, _] : adj_[v]) {
                if (state[to] == InStack) return true;
                if (state[to] == Unvisited && dfs_cycle(to)) return true;
            }
            state[v] = Done;
            return false;
        };

        for (std::size_t v = 0; v < n; ++v)
            if (state[v] == Unvisited && dfs_cycle(v)) return true;
        return false;
    }

    // ---- Bipartiteness check ----
    // Returns color vector (0=uncolored, 1=white, 2=black) or empty if not bipartite
    std::vector<std::size_t> is_bipartite() const {
        std::size_t n = adj_.size();
        std::vector<std::size_t> color(n, 0);

        for (std::size_t start = 0; start < n; ++start) {
            if (color[start] != 0) continue;
            color[start] = 1;
            std::queue<std::size_t> q;
            q.push(start);
            while (!q.empty()) {
                auto v = q.front(); q.pop();
                for (const auto& [to, _] : adj_[v]) {
                    if (color[to] == 0) {
                        color[to] = 3 - color[v];
                        q.push(to);
                    } else if (color[to] == color[v]) {
                        return {};
                    }
                }
            }
        }
        return color;
    }

    // ---- Transitive closure (reachability matrix) ----
    std::vector<std::vector<bool>> transitive_closure() const {
        std::size_t n = adj_.size();
        std::vector<std::vector<bool>> reach(n, std::vector<bool>(n, false));
        for (std::size_t v = 0; v < n; ++v) reach[v][v] = true;

        // Process in reverse topological order for efficiency
        for (std::size_t v = n; v-- > 0;) {
            for (const auto& [to, _] : adj_[v]) {
                reach[v][to] = true;
                for (std::size_t w = 0; w < n; ++w)
                    if (reach[to][w]) reach[v][w] = true;
            }
        }
        return reach;
    }

    // ---- DAG shortest path (topological order) ----
    std::vector<Weight> dag_shortest_path(std::size_t start) const {
        std::size_t n = adj_.size();
        std::vector<Weight> dist(n, std::numeric_limits<Weight>::max());
        dist[start] = Weight{};

        auto topo = topological_sort();
        for (auto v : topo) {
            if (dist[v] == std::numeric_limits<Weight>::max()) continue;
            for (const auto& [to, w] : adj_[v]) {
                if (dist[v] + w < dist[to])
                    dist[to] = dist[v] + w;
            }
        }
        return dist;
    }

    // ---- Eulerian cycle (directed, Hierholzer's) ----
    // Returns vertex sequence of Eulerian cycle, or empty if none exists
    std::vector<std::size_t> eulerian_cycle() const {
        std::size_t n = adj_.size();
        std::vector<std::size_t> in_degree(n, 0), out_degree(n, 0);
        for (std::size_t v = 0; v < n; ++v) {
            out_degree[v] = adj_[v].size();
            for (const auto& [to, _] : adj_[v])
                ++in_degree[to];
        }

        for (std::size_t v = 0; v < n; ++v)
            if (in_degree[v] != out_degree[v]) return {};

        // Make mutable copy of adjacency lists for edge removal
        std::vector<std::deque<std::size_t>> adj(n);
        for (std::size_t v = 0; v < n; ++v)
            for (const auto& [to, _] : adj_[v])
                adj[v].push_back(to);

        std::vector<std::size_t> stack, cycle;
        stack.push_back(0);

        while (!stack.empty()) {
            auto v = stack.back();
            if (!adj[v].empty()) {
                auto to = adj[v].front();
                adj[v].pop_front();
                stack.push_back(to);
            } else {
                cycle.push_back(v);
                stack.pop_back();
            }
        }
        std::reverse(cycle.begin(), cycle.end());
        return cycle;
    }

    // ---- Articulation points and bridges (undirected simple graphs) ----
    std::vector<std::size_t> articulation_points() const {
        const auto result = connectivity_analysis();
        return result.first;
    }

    std::vector<std::pair<std::size_t, std::size_t>> bridges() const {
        const auto result = connectivity_analysis();
        return result.second;
    }

private:
    using connectivity_result = std::pair<
        std::vector<std::size_t>,
        std::vector<std::pair<std::size_t, std::size_t>>>;

    connectivity_result connectivity_analysis() const {
        const std::size_t n = adj_.size();
        std::vector<std::size_t> discovery(n, n), low(n, n);
        std::vector<bool> cut(n, false);
        std::vector<std::pair<std::size_t, std::size_t>> result;
        std::size_t timer = 0;

        std::function<void(std::size_t, std::size_t)> visit =
            [&](std::size_t v, std::size_t parent) {
                discovery[v] = low[v] = timer++;
                std::size_t children = 0;
                for (const auto& e : adj_[v]) {
                    if (e.to == parent) continue;
                    if (discovery[e.to] == n) {
                        ++children;
                        visit(e.to, v);
                        low[v] = std::min(low[v], low[e.to]);
                        if (parent != n && low[e.to] >= discovery[v])
                            cut[v] = true;
                        if (low[e.to] > discovery[v])
                            result.push_back({std::min(v, e.to),
                                              std::max(v, e.to)});
                    } else {
                        low[v] = std::min(low[v], discovery[e.to]);
                    }
                }
                if (parent == n && children > 1) cut[v] = true;
            };

        for (std::size_t v = 0; v < n; ++v)
            if (discovery[v] == n) visit(v, n);

        std::vector<std::size_t> points;
        for (std::size_t v = 0; v < n; ++v)
            if (cut[v]) points.push_back(v);
        std::sort(result.begin(), result.end());
        return {std::move(points), std::move(result)};
    }

    std::vector<std::vector<edge>> adj_;
};

// ---- Union-Find (for Kruskal's MST) ----
class union_find {
public:
    explicit union_find(std::size_t n) : parent_(n), rank_(n, 0) {
        for (std::size_t i = 0; i < n; ++i) parent_[i] = i;
    }

    std::size_t find(std::size_t x) {
        if (parent_[x] != x)
            parent_[x] = find(parent_[x]);
        return parent_[x];
    }

    void unite(std::size_t x, std::size_t y) {
        std::size_t rx = find(x);
        std::size_t ry = find(y);
        if (rx == ry) return;
        if (rank_[rx] < rank_[ry]) {
            parent_[rx] = ry;
        } else if (rank_[rx] > rank_[ry]) {
            parent_[ry] = rx;
        } else {
            parent_[ry] = rx;
            ++rank_[rx];
        }
    }

    bool same(std::size_t x, std::size_t y) { return find(x) == find(y); }

private:
    std::vector<std::size_t> parent_;
    std::vector<std::size_t> rank_;
};

// ---- Kruskal's MST ----
struct weighted_edge {
    std::size_t from, to;
    int weight;
};

inline std::vector<weighted_edge> kruskal_mst(
    std::size_t n, std::vector<weighted_edge> edges) {

    std::sort(edges.begin(), edges.end(),
        [](const auto& a, const auto& b) { return a.weight < b.weight; });

    union_find uf(n);
    std::vector<weighted_edge> mst;

    for (const auto& e : edges) {
        if (!uf.same(e.from, e.to)) {
            uf.unite(e.from, e.to);
            mst.push_back(e);
            if (mst.size() == n - 1) break;
        }
    }
    return mst;
}

// ---- Boruvka's MST ----
inline std::vector<weighted_edge> boruvka_mst(
    std::size_t n, std::vector<weighted_edge> edges) {

    union_find uf(n);
    std::vector<weighted_edge> mst;
    std::size_t num_components = n;

    while (num_components > 1 && mst.size() < n - 1) {
        // Find cheapest edge leaving each component
        std::vector<std::optional<weighted_edge>> cheapest(n);
        for (const auto& e : edges) {
            auto cu = uf.find(e.from), cv = uf.find(e.to);
            if (cu == cv) continue;
            if (!cheapest[cu] || e.weight < cheapest[cu]->weight)
                cheapest[cu] = e;
            if (!cheapest[cv] || e.weight < cheapest[cv]->weight)
                cheapest[cv] = e;
        }

        bool merged = false;
        for (std::size_t i = 0; i < n; ++i) {
            if (!cheapest[i]) continue;
            auto [u, v, w] = *cheapest[i];
            if (!uf.same(u, v)) {
                uf.unite(u, v);
                mst.push_back(*cheapest[i]);
                --num_components;
                merged = true;
            }
        }
        if (!merged) break;
    }
    return mst;
}

}  // namespace dsa

#endif
