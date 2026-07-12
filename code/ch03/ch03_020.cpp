#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

class Graph {
public:
    explicit Graph(int num_vertices)
        : adj_(num_vertices) {}

    void add_edge(int u, int v) {
        adj_[u].push_back(v);
        adj_[v].push_back(u);
    }

    void remove_edge(int u, int v) {
        adj_[u].remove(v);
        adj_[v].remove(u);
    }

    bool has_edge(int u, int v) const {
        for (int neighbor : adj_[u]) {
            if (neighbor == v) return true;
        }
        return false;
    }

    int degree(int v) const {
        return static_cast<int>(adj_[v].size());
    }

    std::vector<int> bfs(int start) const {
        std::vector<int> visited(adj_.size(), 0);
        std::vector<int> order;
        std::queue<int> q;
        visited[start] = 1;
        q.push(start);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            order.push_back(v);
            for (int w : adj_[v]) {
                if (!visited[w]) {
                    visited[w] = 1;
                    q.push(w);
                }
            }
        }
        return order;
    }

    std::vector<int> dfs(int start) const {
        std::vector<int> visited(adj_.size(), 0);
        std::vector<int> order;
        dfs_helper(start, visited, order);
        return order;
    }

    int num_vertices() const {
        return static_cast<int>(adj_.size());
    }

private:
    std::vector<std::list<int>> adj_;

    void dfs_helper(int v, std::vector<int>& visited,
                    std::vector<int>& order) const {
        visited[v] = 1;
        order.push_back(v);
        for (int w : adj_[v]) {
            if (!visited[w]) {
                dfs_helper(w, visited, order);
            }
        }
    }
};
