int edmonds_karp(const std::vector<edge>& graph, size_t s, size_t t) {
    int flow = 0;
    // Build residual adjacency on the fly; same edge struct as Dinic
    while (true) {
        std::vector<edge*> parent(graph.size(), nullptr);
        std::queue<size_t> q;
        q.push(s);
        while (!q.empty() && parent[t] == nullptr) {
            size_t v = q.front(); q.pop();
            for (auto& e : graph[v]) {
                if (e.capacity > 0 && parent[e.to] == nullptr && e.to != s) {
                    parent[e.to] = &e;
                    q.push(e.to);
                }
            }
        }
        if (parent[t] == nullptr) break;
        int aug = INT_MAX;
        for (size_t v = t; v != s; v = parent[v]->to)
            aug = std::min(aug, parent[v]->capacity);
        for (size_t v = t; v != s; v = parent[v]->to) {
            parent[v]->capacity -= aug;
            graph[parent[v]->to][parent[v]->rev].capacity += aug;
        }
        flow += aug;
    }
    return flow;
}
