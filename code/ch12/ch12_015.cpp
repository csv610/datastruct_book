std::vector<size_t> kosaraju_scc(const graph_list<>& g) {
    size_t n = g.size();
    std::vector<bool> visited(n, false);
    std::vector<size_t> order;
    
    // First pass: DFS to get finish order
    std::function<void(size_t)> dfs1 = [&](size_t v) {
        visited[v] = true;
        for (const auto& [to, _] : g.neighbors(v)) {
            if (!visited[to]) dfs1(to);
        }
        order.push_back(v);
    };
    for (size_t v = 0; v < n; ++v) if (!visited[v]) dfs1(v);

    // Second pass: DFS on reversed graph
    graph_list<> reversed(n);
    for (size_t v = 0; v < n; ++v)
        for (const auto& [to, _] : g.neighbors(v))
            reversed.add_edge(to, v, 0);

    std::vector<size_t> component(n, SIZE_MAX);
    size_t comp_id = 0;
    visited.assign(n, false);

    std::function<void(size_t)> dfs2 = [&](size_t v) {
        visited[v] = true;
        component[v] = comp_id;
        for (const auto& [to, _] : reversed.neighbors(v)) {
            if (!visited[to]) dfs2(to);
        }
    };

    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        if (!visited[*it]) {
            dfs2(*it);
            ++comp_id;
        }
    }
    return component;
}
