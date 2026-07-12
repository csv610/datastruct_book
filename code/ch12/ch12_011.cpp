std::vector<weighted_edge> prim_mst(const graph_list<int>& g) {
    size_t n = g.size();
    std::vector<bool> in_mst(n, false);
    std::vector<int> key(n, INT_MAX);
    std::vector<size_t> parent(n, SIZE_MAX);
    
    using entry = std::pair<int, size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    
    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [_, v] = pq.top(); pq.pop();
        if (in_mst[v]) continue;
        in_mst[v] = true;

        for (const auto& [to, w] : g.neighbors(v)) {
            if (!in_mst[to] && w < key[to]) {
                key[to] = w;
                parent[to] = v;
                pq.push({w, to});
            }
        }
    }

    std::vector<weighted_edge> mst;
    for (size_t v = 1; v < n; ++v) {
        if (parent[v] != SIZE_MAX) {
            mst.push_back({parent[v], v, key[v]});
        }
    }
    return mst;
}
