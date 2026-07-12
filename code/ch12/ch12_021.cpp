std::vector<int> welsh_powell_color(const graph_list<>& g, int n) {
    // Sort vertices by degree (descending)
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return g.degree(a) > g.degree(b);
    });

    std::vector<int> color(n, -1);
    std::vector<bool> available(n, false);

    for (int u : order) {
        for (auto [v, _] : g.neighbors(u))
            if (color[v] != -1) available[color[v]] = true;
        int c = 0;
        while (available[c]) ++c;
        color[u] = c;
        for (auto [v, _] : g.neighbors(u))
            if (color[v] != -1) available[color[v]] = false;
    }
    return color;
}
