void find_articulation_points(const graph_list<>& g, int n,
        std::vector<bool>& is_ap) {
    std::vector<int> disc(n, -1), low(n, -1);
    int timer = 0;
    is_ap.assign(n, false);

    auto dfs = [&](auto&& self, int u, int parent) -> void {
        disc[u] = low[u] = timer++;
        int children = 0;

        for (auto [v, _] : g.neighbors(u)) {
            if (disc[v] == -1) {
                ++children;
                self(self, v, u);
                low[u] = std::min(low[u], low[v]);

                // u is root with 2+ children
                if (parent == -1 && children > 1) is_ap[u] = true;
                // u is not root and no back edge above u
                if (parent != -1 && low[v] >= disc[u]) is_ap[u] = true;
            } else if (v != parent) {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    for (int i = 0; i < n; ++i)
        if (disc[i] == -1) dfs(dfs, i, -1);
}

void find_bridges(const graph_list<>& g, int n,
        std::vector<std::pair<int,int>>& bridges) {
    std::vector<int> disc(n, -1), low(n, -1);
    int timer = 0;

    auto dfs = [&](auto&& self, int u, int parent) -> void {
        disc[u] = low[u] = timer++;
        for (auto [v, _] : g.neighbors(u)) {
            if (disc[v] == -1) {
                self(self, v, u);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > disc[u])
                    bridges.emplace_back(u, v);
            } else if (v != parent) {
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    for (int i = 0; i < n; ++i)
        if (disc[i] == -1) dfs(dfs, i, -1);
}
