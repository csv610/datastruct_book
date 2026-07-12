#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <string>
#include <cstring>

// ----------------------------------------------------------------
// Edge struct (shared by Dinic and Edmonds-Karp)
// ----------------------------------------------------------------
struct edge {
    size_t to, rev;
    int capacity;
};

// ----------------------------------------------------------------
// Dinic's Algorithm
// ----------------------------------------------------------------
class dinic {
public:
    dinic(size_t n) : graph_(n) {}

    void add_edge(size_t from, size_t to, int capacity) {
        graph_[from].push_back({to, graph_[to].size(), capacity});
        graph_[to].push_back({from, graph_[from].size() - 1, 0});
    }

    int max_flow(size_t s, size_t t) {
        int flow = 0;
        std::vector<int> level;
        std::vector<size_t> it;

        while (bfs(s, t, level)) {
            it.assign(graph_.size(), 0);
            while (int pushed = dfs(s, t, INT_MAX, level, it)) {
                flow += pushed;
            }
        }
        return flow;
    }

    size_t vertex_count() const { return graph_.size(); }

    // Return all edges (for inspection / matching reconstruction)
    const std::vector<std::vector<edge>>& graph() const { return graph_; }

private:
    bool bfs(size_t s, size_t t, std::vector<int>& level) {
        level.assign(graph_.size(), -1);
        std::queue<size_t> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            size_t v = q.front(); q.pop();
            for (const auto& e : graph_[v]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(size_t v, size_t t, int f, std::vector<int>& level,
            std::vector<size_t>& it) {
        if (v == t) return f;
        for (size_t& i = it[v]; i < graph_[v].size(); ++i) {
            edge& e = graph_[v][i];
            if (e.capacity > 0 && level[v] < level[e.to]) {
                int pushed = dfs(e.to, t, std::min(f, e.capacity), level, it);
                if (pushed > 0) {
                    e.capacity -= pushed;
                    graph_[e.to][e.rev].capacity += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<edge>> graph_;
};

// ----------------------------------------------------------------
// Edmonds-Karp Algorithm
// ----------------------------------------------------------------
class edmonds_karp {
public:
    edmonds_karp(size_t n) : graph_(n) {}

    void add_edge(size_t from, size_t to, int capacity) {
        graph_[from].push_back({to, graph_[to].size(), capacity});
        graph_[to].push_back({from, graph_[from].size() - 1, 0});
    }

    int max_flow(size_t s, size_t t) {
        size_t n = graph_.size();
        int flow = 0;
        while (true) {
            std::vector<edge*> parent(n, nullptr);
            std::queue<size_t> q;
            q.push(s);
            while (!q.empty() && parent[t] == nullptr) {
                size_t v = q.front(); q.pop();
                for (auto& e : graph_[v]) {
                    if (e.capacity > 0 && parent[e.to] == nullptr && e.to != s) {
                        parent[e.to] = &e;
                        q.push(e.to);
                    }
                }
            }
            if (parent[t] == nullptr) break;

            int aug = INT_MAX;
            for (size_t v = t; v != s; ) {
                edge* e = parent[v];
                aug = std::min(aug, e->capacity);
                v = graph_[e->to][e->rev].to;
            }
            for (size_t v = t; v != s; ) {
                edge* e = parent[v];
                e->capacity -= aug;
                graph_[e->to][e->rev].capacity += aug;
                v = graph_[e->to][e->rev].to;
            }
            flow += aug;
        }
        return flow;
    }

private:
    std::vector<std::vector<edge>> graph_;
};

// ----------------------------------------------------------------
// Bipartite Matching (reduction to Dinic)
// ----------------------------------------------------------------
size_t bipartite_matching(const std::vector<std::vector<size_t>>& adj_left,
                           size_t n_right) {
    size_t n_left = adj_left.size();
    size_t source = n_left + n_right;
    size_t sink = n_left + n_right + 1;
    dinic flow(n_left + n_right + 2);

    for (size_t i = 0; i < n_left; ++i) {
        flow.add_edge(source, i, 1);
        for (size_t j : adj_left[i]) {
            flow.add_edge(i, n_left + j, 1);
        }
    }
    for (size_t j = 0; j < n_right; ++j) {
        flow.add_edge(n_left + j, sink, 1);
    }
    return flow.max_flow(source, sink);
}

// ----------------------------------------------------------------
// main — demonstrations
// ----------------------------------------------------------------
int main() {
    std::cout << "=== Dinic's Algorithm ===\n";
    dinic d(4);
    d.add_edge(0, 1, 10);
    d.add_edge(0, 2, 5);
    d.add_edge(1, 2, 15);
    d.add_edge(1, 3, 5);
    d.add_edge(2, 3, 10);
    int flow_val = d.max_flow(0, 3);
    std::cout << "Graph: s=0, a=1, b=2, t=3\n";
    std::cout << "Edges: 0->1(10), 0->2(5), 1->2(15), 1->3(5), 2->3(10)\n";
    std::cout << "Max flow (Dinic): " << flow_val << "\n";

    std::cout << "\n=== Edmonds-Karp ===\n";
    edmonds_karp ek(4);
    ek.add_edge(0, 1, 10);
    ek.add_edge(0, 2, 5);
    ek.add_edge(1, 2, 15);
    ek.add_edge(1, 3, 5);
    ek.add_edge(2, 3, 10);
    int ek_flow = ek.max_flow(0, 3);
    std::cout << "Max flow (Edmonds-Karp): " << ek_flow << "\n";

    std::cout << "\n=== Bipartite Matching ===\n";
    // Left: {A, B, C}, Right: {X, Y, Z}
    // Edges: A-X, A-Y, B-Y, C-Z
    std::vector<std::vector<size_t>> adj_left = {
        {0, 1},  // A -> X(0), Y(1)
        {1},     // B -> Y(1)
        {2}      // C -> Z(2)
    };
    size_t matching = bipartite_matching(adj_left, 3);
    std::cout << "Left: {A, B, C}, Right: {X, Y, Z}\n";
    std::cout << "Edges: A-X, A-Y, B-Y, C-Z\n";
    std::cout << "Maximum matching size: " << matching << "\n";

    return 0;
}
