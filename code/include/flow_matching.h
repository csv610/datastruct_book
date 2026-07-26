#pragma once
#include <algorithm>
#include <climits>
#include <cstddef>
#include <limits>
#include <queue>
#include <vector>

namespace dsa {

// ---- Ford-Fulkerson (DFS-based max flow) ----
class ford_fulkerson {
public:
    explicit ford_fulkerson(int n) : n_(n), adj_(n), cap_(n, std::vector<int>(n, 0)) {}

    void add_edge(int u, int v, int c) {
        cap_[u][v] += c;
        if (std::find(adj_[u].begin(), adj_[u].end(), v) == adj_[u].end())
            adj_[u].push_back(v);
        if (std::find(adj_[v].begin(), adj_[v].end(), u) == adj_[v].end())
            adj_[v].push_back(u);
    }

    int max_flow(int s, int t) {
        flow_.assign(n_, std::vector<int>(n_, 0));
        int total = 0;
        std::vector<int> parent(n_);
        while (dfs(s, t, parent)) {
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v])
                path_flow = std::min(path_flow, cap_[parent[v]][v] - flow_[parent[v]][v]);
            for (int v = t; v != s; v = parent[v]) {
                flow_[parent[v]][v] += path_flow;
                flow_[v][parent[v]] -= path_flow;
            }
            total += path_flow;
        }
        return total;
    }

private:
    bool dfs(int s, int t, std::vector<int>& parent) {
        std::vector<bool> visited(n_, false);
        std::vector<int> stack;
        stack.push_back(s);
        visited[s] = true;
        parent[s] = -1;
        while (!stack.empty()) {
            int u = stack.back(); stack.pop_back();
            for (int v : adj_[u]) {
                if (!visited[v] && cap_[u][v] - flow_[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    if (v == t) return true;
                    stack.push_back(v);
                }
            }
        }
        return false;
    }

    int n_;
    std::vector<std::vector<int>> adj_;
    std::vector<std::vector<int>> cap_;
    std::vector<std::vector<int>> flow_;
};

// ---- Edmonds-Karp (BFS-based max flow) ----
class edmonds_karp {
public:
    explicit edmonds_karp(int n) : n_(n), adj_(n), cap_(n, std::vector<int>(n, 0)) {}

    void add_edge(int u, int v, int c) {
        cap_[u][v] += c;
        if (std::find(adj_[u].begin(), adj_[u].end(), v) == adj_[u].end())
            adj_[u].push_back(v);
        if (std::find(adj_[v].begin(), adj_[v].end(), u) == adj_[v].end())
            adj_[v].push_back(u);
    }

    int max_flow(int s, int t) {
        flow_.assign(n_, std::vector<int>(n_, 0));
        int total = 0;
        std::vector<int> parent(n_);
        while (bfs(s, t, parent)) {
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v])
                path_flow = std::min(path_flow, cap_[parent[v]][v] - flow_[parent[v]][v]);
            for (int v = t; v != s; v = parent[v]) {
                flow_[parent[v]][v] += path_flow;
                flow_[v][parent[v]] -= path_flow;
            }
            total += path_flow;
        }
        return total;
    }

private:
    bool bfs(int s, int t, std::vector<int>& parent) {
        std::vector<bool> visited(n_, false);
        std::queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj_[u]) {
                if (!visited[v] && cap_[u][v] - flow_[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    if (v == t) return true;
                    q.push(v);
                }
            }
        }
        return false;
    }

    int n_;
    std::vector<std::vector<int>> adj_;
    std::vector<std::vector<int>> cap_;
    std::vector<std::vector<int>> flow_;
};

// ---- Bipartite Matching (Hopcroft-Karp) ----
class bipartite_matching {
public:
    bipartite_matching(int n_left, int n_right)
        : n_left_(n_left), n_right_(n_right),
          adj_(n_left), match_l_(n_left, -1), match_r_(n_right, -1),
          dist_(n_left) {}

    void add_edge(int u, int v) { adj_[u].push_back(v); }

    int max_matching() {
        int result = 0;
        while (bfs()) {
            for (int u = 0; u < n_left_; ++u)
                if (match_l_[u] == -1 && dfs(u))
                    ++result;
        }
        return result;
    }

private:
    bool bfs() {
        std::queue<int> q;
        for (int u = 0; u < n_left_; ++u) {
            if (match_l_[u] == -1) { dist_[u] = 0; q.push(u); }
            else dist_[u] = INT_MAX;
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj_[u]) {
                int m = match_r_[v];
                if (m == -1) found = true;
                else if (dist_[m] == INT_MAX) { dist_[m] = dist_[u] + 1; q.push(m); }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : adj_[u]) {
            int m = match_r_[v];
            if (m == -1 || (dist_[m] == dist_[u] + 1 && dfs(m))) {
                match_l_[u] = v;
                match_r_[v] = u;
                return true;
            }
        }
        dist_[u] = INT_MAX;
        return false;
    }

    int n_left_, n_right_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> match_l_, match_r_, dist_;
};

}  // namespace dsa
