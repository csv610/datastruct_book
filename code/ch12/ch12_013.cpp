std::optional<std::vector<int>> bellman_ford(
    const std::vector<weighted_edge>& edges, size_t n, size_t source) {
    
    std::vector<int> distance(n, INT_MAX / 2);
    distance[source] = 0;

    // Relax all edges V-1 times
    for (size_t i = 0; i < n - 1; ++i) {
        for (const auto& [u, v, w] : edges) {
            if (distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
            }
        }
    }

    // Check for negative cycles
    for (const auto& [u, v, w] : edges) {
        if (distance[u] + w < distance[v]) {
            return std::nullopt;  // negative cycle detected
        }
    }
    return distance;
}
