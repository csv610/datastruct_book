std::vector<size_t> connected_components(const graph_list<>& g) {
    size_t n = g.size();
    std::vector<size_t> component(n, SIZE_MAX);
    size_t comp_id = 0;

    for (size_t v = 0; v < n; ++v) {
        if (component[v] != SIZE_MAX) continue;
        
        // BFS from v
        std::queue<size_t> q;
        q.push(v);
        component[v] = comp_id;
        
        while (!q.empty()) {
            size_t u = q.front(); q.pop();
            for (const auto& [to, _] : g.neighbors(u)) {
                if (component[to] == SIZE_MAX) {
                    component[to] = comp_id;
                    q.push(to);
                }
            }
        }
        ++comp_id;
    }
    return component;
}
