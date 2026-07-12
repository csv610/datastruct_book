struct edge { int u, v, weight; };

std::vector<edge> kruskal(int n, std::vector<edge> edges) {
    std::sort(edges.begin(), edges.end(),
              [](const edge& a, const edge& b) { return a.weight < b.weight; });

    union_find uf(n);
    std::vector<edge> mst;

    for (const auto& [u, v, w] : edges) {
        if (uf.find(u) != uf.find(v)) {
            mst.push_back({u, v, w});
            uf.unite(u, v);
        }
    }
    return mst;
}
