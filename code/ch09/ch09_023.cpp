#include <vector>
#include <queue>
#include <utility>

// Returns (diameter, farthest node from start)
std::pair<int,int> bfs_farthest(
        const std::vector<std::vector<int>>& adj, int start) {
    int n = adj.size();
    std::vector<int> dist(n, -1);
    std::queue<int> q;
    dist[start] = 0;
    q.push(start);
    int farthest = start;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int to : adj[v]) {
            if (dist[to] != -1) continue;
            dist[to] = dist[v] + 1;
            q.push(to);
            if (dist[to] > dist[farthest]) farthest = to;
        }
    }
    return {dist[farthest], farthest};
}

int tree_diameter(const std::vector<std::vector<int>>& adj) {
    auto [_, u] = bfs_farthest(adj, 0);
    auto [diam, _2] = bfs_farthest(adj, u);
    return diam;
}
