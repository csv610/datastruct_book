class graph_coloring {
public:
    graph_coloring(int n, const std::vector<std::pair<int,int>>& edges,
                   int m)
        : n_(n), m_(m), colors_(n, 0) {
        adj_.resize(n);
        for (auto [u, v] : edges) {
            adj_[u].push_back(v);
            adj_[v].push_back(u);
        }
    }

    bool solve() { return backtrack(0); }

    std::vector<int> get_colors() const { return colors_; }

private:
    bool backtrack(int v) {
        if (v == n_) return true;

        for (int c = 1; c <= m_; ++c) {
            if (is_safe(v, c)) {
                colors_[v] = c;
                if (backtrack(v + 1)) return true;
                colors_[v] = 0;  // backtrack
            }
        }
        return false;
    }

    bool is_safe(int v, int c) {
        for (int u : adj_[v]) {
            if (colors_[u] == c) return false;
        }
        return true;
    }

    int n_, m_;
    std::vector<std::vector<int>> adj_;
    std::vector<int> colors_;
};
