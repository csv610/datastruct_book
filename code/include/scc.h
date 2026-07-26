#pragma once
#include <algorithm>
#include <stack>
#include <vector>

namespace dsa {

// ---- Kosaraju's Algorithm (SCC) ----
class kosaraju_scc {
public:
    explicit kosaraju_scc(int n) : n_(n), adj_(n), radj_(n) {}

    void add_edge(int u, int v) {
        adj_[u].push_back(v);
        radj_[v].push_back(u);
    }

    std::vector<std::vector<int>> find_sccs() {
        std::vector<bool> visited(n_, false);
        std::stack<int> order;
        for (int i = 0; i < n_; ++i)
            if (!visited[i]) dfs1(i, visited, order);

        std::fill(visited.begin(), visited.end(), false);
        std::vector<std::vector<int>> sccs;
        while (!order.empty()) {
            int v = order.top(); order.pop();
            if (!visited[v]) {
                std::vector<int> comp;
                dfs2(v, visited, comp);
                sccs.push_back(std::move(comp));
            }
        }
        return sccs;
    }

private:
    void dfs1(int v, std::vector<bool>& visited, std::stack<int>& order) {
        visited[v] = true;
        for (int u : adj_[v])
            if (!visited[u]) dfs1(u, visited, order);
        order.push(v);
    }

    void dfs2(int v, std::vector<bool>& visited, std::vector<int>& comp) {
        visited[v] = true;
        comp.push_back(v);
        for (int u : radj_[v])
            if (!visited[u]) dfs2(u, visited, comp);
    }

    int n_;
    std::vector<std::vector<int>> adj_;
    std::vector<std::vector<int>> radj_;
};

// ---- Tarjan's Algorithm (SCC) ----
class tarjan_scc {
public:
    explicit tarjan_scc(int n) : n_(n), adj_(n) {}

    void add_edge(int u, int v) { adj_[u].push_back(v); }

    std::vector<std::vector<int>> find_sccs() {
        index_ = 0;
        indices_.assign(n_, -1);
        lowlink_.assign(n_, 0);
        on_stack_.assign(n_, false);
        sccs_.clear();

        for (int i = 0; i < n_; ++i)
            if (indices_[i] == -1) strongconnect(i);

        return sccs_;
    }

private:
    void strongconnect(int v) {
        indices_[v] = lowlink_[v] = index_++;
        stack_.push(v);
        on_stack_[v] = true;

        for (int w : adj_[v]) {
            if (indices_[w] == -1) {
                strongconnect(w);
                lowlink_[v] = std::min(lowlink_[v], lowlink_[w]);
            } else if (on_stack_[w]) {
                lowlink_[v] = std::min(lowlink_[v], indices_[w]);
            }
        }

        if (lowlink_[v] == indices_[v]) {
            std::vector<int> comp;
            int w;
            do {
                w = stack_.top(); stack_.pop();
                on_stack_[w] = false;
                comp.push_back(w);
            } while (w != v);
            sccs_.push_back(std::move(comp));
        }
    }

    int n_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> indices_;
    std::vector<int> lowlink_;
    std::vector<bool> on_stack_;
    std::stack<int> stack_;
    int index_ = 0;
    std::vector<std::vector<int>> sccs_;
};

}  // namespace dsa
