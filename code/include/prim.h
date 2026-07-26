#pragma once
#include <climits>
#include <queue>
#include <vector>

namespace dsa {

struct prim_edge {
    int to;
    int weight;
};

// Prim's MST using min-heap, returns vector of {parent, weight} or MST weight
class prim_mst {
public:
    explicit prim_mst(int n) : n_(n), adj_(n) {}

    void add_edge(int u, int v, int w) {
        adj_[u].push_back({v, w});
        adj_[v].push_back({u, w});
    }

    // Returns {total_weight, edges} where edges = {parent, vertex, weight}
    struct mst_result {
        int total_weight;
        std::vector<std::tuple<int, int, int>> edges;  // from, to, weight
    };

    mst_result compute() {
        std::vector<int> key(n_, INT_MAX);
        std::vector<int> parent(n_, -1);
        std::vector<bool> in_mst(n_, false);

        using pii = std::pair<int, int>;  // key, vertex
        std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

        key[0] = 0;
        pq.push({0, 0});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (in_mst[u]) continue;
            in_mst[u] = true;

            for (auto& [v, w] : adj_[u]) {
                if (!in_mst[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                    pq.push({w, v});
                }
            }
        }

        mst_result res;
        res.total_weight = 0;
        for (int v = 1; v < n_; ++v) {
            if (parent[v] != -1) {
                res.edges.push_back({parent[v], v, key[v]});
                res.total_weight += key[v];
            }
        }
        return res;
    }

    bool is_connected() const {
        prim_mst tmp(n_);
        for (int u = 0; u < n_; ++u)
            for (auto& [v, w] : adj_[u])
                if (u < v) tmp.add_edge(u, v, w);
        auto res = tmp.compute();
        return static_cast<int>(res.edges.size()) == n_ - 1;
    }

private:
    int n_;
    std::vector<std::vector<prim_edge>> adj_;
};

}  // namespace dsa
