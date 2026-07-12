#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <limits>
#include <queue>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

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

private:
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

#endif
