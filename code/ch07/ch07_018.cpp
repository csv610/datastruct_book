bool is_bipartite(
    const std::vector<std::vector<int>>& adj, int n)
{
    std::vector<int> color(n, -1);  // -1 = uncolored

    for (int start = 0; start < n; ++start) {
        if (color[start] != -1) continue;  // already visited

        // BFS from this component.
        std::queue<int> q;
        q.push(start);
        color[start] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;  // flip color
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;  // same color = not bipartite
                }
            }
        }
    }
    return true;
}
