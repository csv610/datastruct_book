struct weighted_edge {
    size_t from, to;
    int weight;
};

std::vector<weighted_edge> kruskal_mst(size_t n,
                                        std::vector<weighted_edge> edges) {
    std::sort(edges.begin(), edges.end(),
              [](const auto& a, const auto& b) { return a.weight < b.weight; });

    union_find uf(n);
    std::vector<weighted_edge> mst;

    for (const auto& e : edges) {
        if (!uf.same_set(e.from, e.to)) {
            uf.union_sets(e.from, e.to);
            mst.push_back(e);
            if (mst.size() == n - 1) break;
        }
    }
    return mst;
}
