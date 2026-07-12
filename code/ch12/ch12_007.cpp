std::vector<size_t> bfs(const graph_list<>& g, size_t start) {
    std::vector<size_t> distance(g.size(), SIZE_MAX);
    std::queue<size_t> q;
    
    distance[start] = 0;
    q.push(start);
    
    while (!q.empty()) {
        size_t v = q.front(); q.pop();
        for (const auto& [to, _] : g.neighbors(v)) {
            if (distance[to] == SIZE_MAX) {
                distance[to] = distance[v] + 1;
                q.push(to);
            }
        }
    }
    return distance;
}
