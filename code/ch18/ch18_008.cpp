class tsp_solver {
public:
    tsp_solver(std::vector<std::vector<int>> cost_matrix)
        : cost_(std::move(cost_matrix)), n_(cost_.size()), best_cost_(INT_MAX) {}

    int solve() {
        std::vector<bool> visited(n_, false);
        visited[0] = true;
        branch_and_bound(0, 1, 0, visited);
        return best_cost_;
    }

private:
    void branch_and_bound(int current, int count, int current_cost,
                           std::vector<bool>& visited) {
        if (count == n_) {
            current_cost += cost_[current][0];  // return to start
            best_cost_ = std::min(best_cost_, current_cost);
            return;
        }

        for (int next = 0; next < n_; ++next) {
            if (!visited[next]) {
                int new_cost = current_cost + cost_[current][next];
                // Bound: if already worse than best, prune
                if (new_cost < best_cost_) {
                    visited[next] = true;
                    branch_and_bound(next, count + 1, new_cost, visited);
                    visited[next] = false;
                }
            }
        }
    }

    const std::vector<std::vector<int>>& cost_;
    size_t n_;
    int best_cost_;
};
