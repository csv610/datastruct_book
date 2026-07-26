#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

namespace dsa {

// ---- 0/1 Knapsack (Dynamic Programming) ----
struct knapsack_item {
    int weight;
    int value;
};

inline int knapsack_01(int capacity, const std::vector<knapsack_item>& items) {
    std::size_t n = items.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (std::size_t i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            dp[i][w] = dp[i - 1][w];
            if (items[i - 1].weight <= w)
                dp[i][w] = std::max(dp[i][w],
                    dp[i - 1][w - items[i - 1].weight] + items[i - 1].value);
        }
    }
    return dp[n][capacity];
}

// ---- Longest Common Subsequence ----
inline std::string lcs(const std::string& a, const std::string& b) {
    std::size_t m = a.size(), n = b.size();
    std::vector<std::vector<std::size_t>> dp(m + 1, std::vector<std::size_t>(n + 1, 0));

    for (std::size_t i = 1; i <= m; ++i)
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }

    std::string result;
    std::size_t i = m, j = n;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result += a[i - 1];
            --i; --j;
        } else if (dp[i - 1][j] > dp[i][j]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ---- Edit Distance (Levenshtein) ----
inline std::size_t edit_distance(const std::string& a, const std::string& b) {
    std::size_t m = a.size(), n = b.size();
    std::vector<std::vector<std::size_t>> dp(m + 1, std::vector<std::size_t>(n + 1));

    for (std::size_t i = 0; i <= m; ++i) dp[i][0] = i;
    for (std::size_t j = 0; j <= n; ++j) dp[0][j] = j;

    for (std::size_t i = 1; i <= m; ++i)
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    return dp[m][n];
}

// ---- Longest Increasing Subsequence (O(n log n)) ----
inline std::size_t lis_length(const std::vector<int>& a) {
    std::vector<int> tails;
    for (int x : a) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}

}  // namespace dsa
