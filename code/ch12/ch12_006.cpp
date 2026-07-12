void dfs(const graph_list<>& g, size_t start, auto& visit) {
    std::vector<bool> visited(g.size(), false);
    std::function<void(size_t)> dfs_impl = [&](size_t v) {
        visited[v] = true;
        visit(v);
        for (const auto& [to, _] : g.neighbors(v)) {
            if (!visited[to]) dfs_impl(to);
        }
    };
    dfs_impl(start);
}
