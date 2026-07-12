std::vector<int> dijkstra(const std::vector<std::vector<std::pair<int,int>>>& adj,
                          int source) {
    int n = adj.size();
    std::vector<int> dist(n, INT_MAX);
    dist[source] = 0;

    // Min-heap: (distance, vertex)
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;  // stale entry

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
