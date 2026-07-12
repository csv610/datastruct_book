std::vector<int> dijkstra(const graph_list<int>& g, size_t source) {
    size_t n = g.size();
    std::vector<int> distance(n, INT_MAX);
    std::vector<bool> visited(n, false);
    
    using entry = std::pair<int, size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    
    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [dist, v] = pq.top(); pq.pop();
        if (visited[v]) continue;
        visited[v] = true;

        for (const auto& [to, w] : g.neighbors(v)) {
            if (dist + w < distance[to]) {
                distance[to] = dist + w;
                pq.push({distance[to], to});
            }
        }
    }
    return distance;
}
