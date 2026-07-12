std::vector<std::vector<int>> floyd_warshall(
    const std::vector<weighted_edge>& edges, size_t n) {
    
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INT_MAX / 2));
    for (size_t i = 0; i < n; ++i) dist[i][i] = 0;
    for (const auto& [u, v, w] : edges) dist[u][v] = w;

    for (size_t k = 0; k < n; ++k) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}
