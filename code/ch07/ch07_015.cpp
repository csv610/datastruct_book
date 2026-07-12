// Dijkstra using std::priority_queue.
// Graph: adjacency list, (neighbor, weight) pairs.
void dijkstra(
    const std::vector<std::vector<std::pair<int,int>>>& adj,
    int source, int n)
{
    // Min-priority queue: (distance, vertex).
    using pii = std::pair<int,int>;
    std::priority_queue<pii, std::vector<pii>,
                        std::greater<pii>> pq;

    std::vector<int> dist(n, INT_MAX);
    std::vector<int> prev(n, -1);
    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;  // stale entry

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        std::cout << i << ": dist=" << dist[i]
                  << " path=";
        // Reconstruct path.
        for (int v = i; v != -1; v = prev[v]) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
}
