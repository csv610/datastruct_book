std::vector<size_t> topological_sort(const graph_list<>& g) {
    size_t n = g.size();
    std::vector<size_t> in_degree(n, 0);
    
    // Compute in-degrees
    for (size_t v = 0; v < n; ++v) {
        for (const auto& [to, _] : g.neighbors(v)) {
            ++in_degree[to];
        }
    }

    // Kahn's algorithm
    std::queue<size_t> q;
    for (size_t v = 0; v < n; ++v) {
        if (in_degree[v] == 0) q.push(v);
    }

    std::vector<size_t> result;
    while (!q.empty()) {
        size_t v = q.front(); q.pop();
        result.push_back(v);
        for (const auto& [to, _] : g.neighbors(v)) {
            if (--in_degree[to] == 0) q.push(to);
        }
    }
    return result;
}
