#include <vector>
#include <stack>

void dfs_topo(int u, const std::vector<std::vector<int>>& adj,
              std::vector<bool>& visited,
              std::stack<int>& order) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs_topo(v, adj, visited, order);
        }
    }
    order.push(u);
}

std::vector<int> topological_sort(
        const std::vector<std::vector<int>>& adj) {
    int n = adj.size();
    std::vector<bool> visited(n, false);
    std::stack<int> order;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs_topo(i, adj, visited, order);
        }
    }
    std::vector<int> result;
    while (!order.empty()) {
        result.push_back(order.top());
        order.pop();
    }
    return result;
}
