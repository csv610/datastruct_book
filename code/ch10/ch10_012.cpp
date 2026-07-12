void dijkstra_fib(const graph& g, int source, std::vector<int>& dist) {
    int n = g.num_vertices();
    dist.assign(n, INT_MAX);
    fibonacci_heap<int> pq;
    std::vector<fibonacci_heap<int>::node*> handles(n);

    dist[source] = 0;
    for (int i = 0; i < n; ++i) {
        handles[i] = pq.insert(i);  // insert vertex indices
    }

    while (!pq.empty()) {
        int u = pq.extract_min();
        for (auto [v, weight] : g.neighbors(u)) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.decrease_key(handles[v], dist[v]);
            }
        }
    }
}
