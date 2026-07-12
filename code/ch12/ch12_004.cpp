class graph_matrix {
public:
    graph_matrix(size_t n) : n_(n), adj_(n, std::vector<bool>(n, false)) {}

    void add_edge(size_t u, size_t v) {
        adj_[u][v] = true;
        adj_[v][u] = true;  // undirected
    }

    bool has_edge(size_t u, size_t v) const {
        return adj_[u][v];
    }

    std::vector<size_t> neighbors(size_t v) const {
        std::vector<size_t> result;
        for (size_t i = 0; i < n_; ++i) {
            if (adj_[v][i]) result.push_back(i);
        }
        return result;
    }

    size_t size() const noexcept { return n_; }

private:
    size_t n_;
    std::vector<std::vector<bool>> adj_;
};
