#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cstring>
#include <numeric>

// ----------------------------------------------------------------
// 0/1 Knapsack — 2D DP table
// ----------------------------------------------------------------
int knapsack_01(const std::vector<int>& weights,
                const std::vector<int>& values,
                int capacity) {
    int n = static_cast<int>(weights.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= capacity; ++w) {
            if (weights[i - 1] > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = std::max(dp[i - 1][w],
                                    values[i - 1] + dp[i - 1][w - weights[i - 1]]);
            }
        }
    }
    return dp[n][capacity];
}

// ----------------------------------------------------------------
// 0/1 Knapsack — 1D space-optimized
// ----------------------------------------------------------------
int knapsack_01_optimized(const std::vector<int>& weights,
                           const std::vector<int>& values,
                           int capacity) {
    std::vector<int> dp(capacity + 1, 0);
    for (size_t i = 0; i < weights.size(); ++i) {
        for (int w = capacity; w >= weights[i]; --w) {
            dp[w] = std::max(dp[w], values[i] + dp[w - weights[i]]);
        }
    }
    return dp[capacity];
}

// ----------------------------------------------------------------
// 0/1 Knapsack — reconstruct selected items
// ----------------------------------------------------------------
std::vector<size_t> knapsack_items(const std::vector<int>& weights,
                                    const std::vector<int>& values,
                                    int capacity) {
    int n = static_cast<int>(weights.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= capacity; ++w) {
            if (weights[i - 1] > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = std::max(dp[i - 1][w],
                                    values[i - 1] + dp[i - 1][w - weights[i - 1]]);
            }
        }
    }
    std::vector<size_t> selected;
    int w = capacity;
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(static_cast<size_t>(i - 1));
            w -= weights[i - 1];
        }
    }
    return selected;
}

// ----------------------------------------------------------------
// Matrix Chain Multiplication
// ----------------------------------------------------------------
int matrix_chain_order(const std::vector<int>& dims) {
    int n = static_cast<int>(dims.size()) - 1;
    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = m[i][k] + m[k + 1][j]
                         + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < m[i][j]) m[i][j] = cost;
            }
        }
    }
    return m[0][n - 1];
}

// ----------------------------------------------------------------
// Longest Common Subsequence — length only
// ----------------------------------------------------------------
int lcs_length(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size()), n = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

// ----------------------------------------------------------------
// Longest Common Subsequence — with backtracking
// ----------------------------------------------------------------
std::string lcs(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size()), n = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    std::string result;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result.push_back(a[i - 1]);
            --i; --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ----------------------------------------------------------------
// Edit Distance (Levenshtein)
// ----------------------------------------------------------------
int edit_distance(const std::string& a, const std::string& b) {
    int m = static_cast<int>(a.size()), n = static_cast<int>(b.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({dp[i - 1][j] + 1,
                                     dp[i][j - 1] + 1,
                                     dp[i - 1][j - 1] + 1});
            }
        }
    }
    return dp[m][n];
}

// ----------------------------------------------------------------
// Coin Change (Unbounded) — minimum coins
// ----------------------------------------------------------------
int coin_change(const std::vector<int>& coins, int amount) {
    std::vector<int> dp(amount + 1, INT_MAX / 2);
    dp[0] = 0;
    for (int c = 1; c <= amount; ++c) {
        for (int coin : coins) {
            if (coin <= c) {
                dp[c] = std::min(dp[c], dp[c - coin] + 1);
            }
        }
    }
    return dp[amount] == INT_MAX / 2 ? -1 : dp[amount];
}

// ----------------------------------------------------------------
// main — demonstrations
// ----------------------------------------------------------------
int main() {
    std::cout << "=== 0/1 Knapsack (2D) ===\n";
    std::vector<int> w = {2, 3, 4, 5};
    std::vector<int> v = {3, 4, 5, 6};
    int cap = 8;
    int val = knapsack_01(w, v, cap);
    std::cout << "Items: (w,v) = (2,3) (3,4) (4,5) (5,6), capacity=" << cap << "\n";
    std::cout << "Optimal value (2D): " << val << "\n";

    std::cout << "\n=== 0/1 Knapsack (1D space-optimized) ===\n";
    int val_opt = knapsack_01_optimized(w, v, cap);
    std::cout << "Optimal value (1D): " << val_opt << "\n";

    std::cout << "\n=== Knapsack Reconstruction ===\n";
    auto selected = knapsack_items(w, v, cap);
    std::cout << "Selected items (indices): ";
    for (auto idx : selected) std::cout << idx << " ";
    std::cout << "\n";

    std::cout << "\n=== Matrix Chain Multiplication ===\n";
    std::vector<int> dims = {10, 20, 30, 40, 30};
    int mcm = matrix_chain_order(dims);
    std::cout << "Dimensions: [10, 20, 30, 40, 30]\n";
    std::cout << "Minimum scalar multiplications: " << mcm << "\n";

    std::cout << "\n=== Longest Common Subsequence ===\n";
    std::string x = "ABCBDAB", y = "BDCABA";
    int lcs_len = lcs_length(x, y);
    std::string lcs_str = lcs(x, y);
    std::cout << "X = \"" << x << "\", Y = \"" << y << "\"\n";
    std::cout << "LCS length: " << lcs_len << "\n";
    std::cout << "LCS string: \"" << lcs_str << "\"\n";

    std::cout << "\n=== Edit Distance (Levenshtein) ===\n";
    std::string s1 = "kitten", s2 = "sitting";
    int ed = edit_distance(s1, s2);
    std::cout << "A = \"" << s1 << "\", B = \"" << s2 << "\"\n";
    std::cout << "Edit distance: " << ed << "\n";

    std::cout << "\n=== Coin Change (Unbounded) ===\n";
    std::vector<int> coins = {1, 2, 5};
    int amount = 11;
    int min_coins = coin_change(coins, amount);
    std::cout << "Coins: [1, 2, 5], amount=" << amount << "\n";
    std::cout << "Minimum coins needed: " << min_coins << "\n";

    return 0;
}
