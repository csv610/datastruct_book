std::vector<edge> prim(const std::vector<std::vector<std::pair<int,int>>>& adj) {
    int n = adj.size();
    std::vector<bool> in_tree(n, false);
    std::priority_queue<std::pair<int,std::pair<int,int>>,
                        std::vector<std::pair<int,std::pair<int,int>>>,
                        std::greater<>> pq;

    in_tree[0] = true;
    for (auto [v, w] : adj[0]) pq.push({w, {0, v}});

    std::vector<edge> mst;
    while (mst.size() < n - 1 && !pq.empty()) {
        auto [w, u_v] = pq.top(); pq.pop();
        auto [u, v] = u_v;
        if (in_tree[v]) continue;
        in_tree[v] = true;
        mst.push_back({u, v, w});
        for (auto [next, nw] : adj[v]) {
            if (!in_tree[next]) pq.push({nw, {v, next}});
        }
    }
    return mst;
}
