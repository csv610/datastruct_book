#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>

namespace dsa {

// ---- N-Queens ----
inline std::vector<std::vector<std::string>> n_queens(int n) {
    std::vector<std::vector<std::string>> solutions;
    std::vector<std::string> board(n, std::string(n, '.'));
    std::vector<bool> col(n, false), diag1(2 * n, false), diag2(2 * n, false);

    std::function<void(int)> solve = [&](int row) {
        if (row == n) {
            solutions.push_back(board);
            return;
        }
        for (int c = 0; c < n; ++c) {
            if (col[c] || diag1[row + c] || diag2[row - c + n]) continue;
            col[c] = diag1[row + c] = diag2[row - c + n] = true;
            board[row][c] = 'Q';
            solve(row + 1);
            board[row][c] = '.';
            col[c] = diag1[row + c] = diag2[row - c + n] = false;
        }
    };

    solve(0);
    return solutions;
}

// ---- Subset Sum ----
inline bool subset_sum(const std::vector<int>& nums, int target) {
    std::size_t n = nums.size();
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(target + 1, false));
    for (std::size_t i = 0; i <= n; ++i) dp[i][0] = true;

    for (std::size_t i = 1; i <= n; ++i)
        for (int s = 0; s <= target; ++s) {
            dp[i][s] = dp[i - 1][s];
            if (nums[i - 1] <= s)
                dp[i][s] = dp[i][s] || dp[i - 1][s - nums[i - 1]];
        }
    return dp[n][target];
}

// ---- Traveling Salesman Problem (DP, bitmask, O(n^2 * 2^n)) ----
inline int tsp(const std::vector<std::vector<int>>& dist) {
    int n = static_cast<int>(dist.size());
    if (n <= 1) return 0;
    int ALL = (1 << n) - 1;
    const int INF = 1e9;
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));
    dp[1][0] = 0;

    for (int mask = 1; mask <= ALL; ++mask) {
        for (int u = 0; u < n; ++u) {
            if (dp[mask][u] == INF) continue;
            if (!(mask & (1 << u))) continue;
            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue;
                int next = mask | (1 << v);
                dp[next][v] = std::min(dp[next][v], dp[mask][u] + dist[u][v]);
            }
        }
    }

    int best = INF;
    for (int u = 1; u < n; ++u)
        best = std::min(best, dp[ALL][u] + dist[u][0]);
    return best;
}

}  // namespace dsa
