#pragma once
#include <algorithm>
#include <climits>
#include <vector>

namespace dsa {

// ---- Hungarian algorithm (assignment problem) ----
// Finds minimum-cost perfect matching in a bipartite graph
// cost[i][j] = cost of assigning worker i to job j
// Returns {min_cost, assignment} where assignment[i] = job assigned to worker i
struct assignment_result {
    long long cost;
    std::vector<int> assignment;
};

class hungarian {
public:
    explicit hungarian(const std::vector<std::vector<int>>& cost)
        : n_(static_cast<int>(cost.size())),
          m_(n_),
          cost_(cost) {}

    assignment_result solve() {
        int n = n_, m = m_;
        std::vector<int> u(n + 1, 0), v(m + 1, 0), p(m + 1, 0), way(m + 1, 0);

        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            std::vector<int> minv(m + 1, INT_MAX);
            std::vector<bool> used(m + 1, false);

            do {
                used[j0] = true;
                int i0 = p[j0], delta = INT_MAX, j1 = -1;
                for (int j = 1; j <= m; ++j) {
                    if (used[j]) continue;
                    int cur = cost_[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
                for (int j = 0; j <= m; ++j) {
                    if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                    else         { minv[j] -= delta; }
                }
                j0 = j1;
            } while (p[j0] != 0);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0 != 0);
        }

        std::vector<int> assignment(n);
        for (int j = 1; j <= m; ++j)
            if (p[j] > 0) assignment[p[j] - 1] = j - 1;

        return {-v[0], assignment};
    }

private:
    int n_, m_;
    const std::vector<std::vector<int>>& cost_;
};

// ---- Munkres algorithm (minimum cost, handles rectangular) ----
// Simplified Hungarian for square matrices
inline assignment_result hungarian_min(const std::vector<std::vector<int>>& cost) {
    hungarian h(cost);
    return h.solve();
}

}  // namespace dsa
