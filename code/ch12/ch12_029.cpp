void biconnected_components(const graph_list<>& g, int n,
        std::vector<std::vector<std::pair<int,int>>>& components) {
    std::vector<int> disc(n, -1), low(n, -1);
    int timer = 0;
    std::stack<std::pair<int,int>> stk;

    auto dfs = [&](auto&& self, int u, int parent) -> void {
        disc[u] = low[u] = timer++;
        int children = 0;

        for (auto [v, _] : g.neighbors(u)) {
            if (disc[v] == -1) {
                ++children;
                stk.push({u, v});
                self(self, v, u);
                low[u] = std::min(low[u], low[v]);

                // Articulation point: pop one BCC
                if ((parent == -1 && children > 1) ||
                    (parent != -1 && low[v] >= disc[u])) {
                    components.emplace_back();
                    while (true) {
                        auto e = stk.top(); stk.pop();
                        components.back().push_back(e);
                        if (e == std::make_pair(u, v)) break;
                    }
                }
            } else if (v != parent && disc[v] < disc[u]) {
                // Back edge to ancestor (only push once)
                stk.push({u, v});
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    for (int i = 0; i < n; ++i)
        if (disc[i] == -1) {
            dfs(dfs, i, -1);
            // Remaining edges on the stack form the last BCC
            if (!stk.empty()) {
                components.emplace_back();
                while (!stk.empty()) {
                    components.back().push_back(stk.top());
                    stk.pop();
                }
            }
        }
}
