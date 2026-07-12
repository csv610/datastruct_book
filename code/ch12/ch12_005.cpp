template <typename Weight = int>
class graph_list {
public:
    struct edge {
        size_t to;
        Weight weight;
    };

    explicit graph_list(size_t n) : adj_(n) {}

    void add_edge(size_t u, size_t v, Weight w = Weight{}) {
        adj_[u].push_back({v, w});
        adj_[v].push_back({u, w});  // undirected
    }

    const std::vector<edge>& neighbors(size_t v) const {
        return adj_[v];
    }

    size_t size() const noexcept { return adj_.size(); }
    size_t edge_count() const {
        size_t e = 0;
        for (const auto& adj : adj_) e += adj.size();
        return e / 2;  // undirected
    }

private:
    std::vector<std::vector<edge>> adj_;
};
