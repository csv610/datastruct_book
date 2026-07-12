std::vector<int> greedy_color(const graph_list<>& g, int n) {
    std::vector<int> color(n, -1);
    color[0] = 0;

    std::vector<bool> available(n, false);

    for (int u = 1; u < n; ++u) {
        // Mark colors of adjacent vertices as unavailable
        for (auto [v, _] : g.neighbors(u))
            if (color[v] != -1) available[color[v]] = true;

        // Find first available color
        int c = 0;
        while (available[c]) ++c;
        color[u] = c;

        // Reset available array
        for (auto [v, _] : g.neighbors(u))
            if (color[v] != -1) available[color[v]] = false;
    }
    return color;
}
