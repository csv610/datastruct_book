int find_chromatic_number(int n,
                          const std::vector<std::pair<int,int>>& edges) {
    for (int m = 1; m <= n; ++m) {
        graph_coloring gc(n, edges, m);
        if (gc.solve()) return m;
    }
    return n;
}
