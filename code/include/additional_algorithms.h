#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace dsa {

// ---- Landau-Vishkin Algorithm (k-mismatches with wildcards) ----
// Uses LCP/LCS arrays for O(nk) matching with Hamming distance
inline std::vector<std::size_t> landau_vishkin(
    std::string_view text, std::string_view pattern, int k) {
    std::size_t m = pattern.size();
    std::vector<std::size_t> results;
    if (m == 0 || text.size() < m) return results;

    // Build prefix function for pattern
    std::vector<int> pi(m, 0);
    for (std::size_t i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j]) j = pi[j - 1];
        if (pattern[i] == pattern[j]) ++j;
        pi[i] = j;
    }

    // For each position, count mismatches
    for (std::size_t i = 0; i + m <= text.size(); ++i) {
        int mismatches = 0;
        for (std::size_t p = 0; p < m; ++p) {
            if (text[i + p] != pattern[p]) {
                ++mismatches;
                if (mismatches > k) break;
            }
        }
        if (mismatches <= k)
            results.push_back(i);
    }
    return results;
}

// ---- Hirschberg's Algorithm (Linear-space LCS) ----
// Computes LCS length in O(mn) time and O(min(m,n)) space
inline std::size_t hirschberg_lcs_length(std::string_view a, std::string_view b) {
    std::size_t m = a.size(), n = b.size();
    if (m == 0 || n == 0) return 0;
    if (m < n) return hirschberg_lcs_length(b, a);

    // Only need two rows
    std::vector<std::size_t> prev(n + 1, 0), curr(n + 1, 0);
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1])
                curr[j] = prev[j - 1] + 1;
            else
                curr[j] = std::max(prev[j], curr[j - 1]);
        }
        std::swap(prev, curr);
        std::fill(curr.begin(), curr.end(), 0);
    }
    return prev[n];
}

// Hirschberg's divide-and-conquer: returns the actual LCS string
inline std::string hirschberg_lcs(std::string_view a, std::string_view b) {
    if (a.empty() || b.empty()) return "";

    std::size_t m = a.size(), n = b.size();

    // Base case: single character
    if (m == 1) {
        for (std::size_t j = 0; j < n; ++j)
            if (a[0] == b[j]) return std::string(1, a[0]);
        return "";
    }

    // Divide: compute LCS lengths for top and bottom halves of a
    std::size_t mid = m / 2;

    // Forward pass: LCS lengths for a[0..mid-1] vs b
    std::vector<std::size_t> prev(n + 1, 0), curr(n + 1, 0);
    std::vector<std::size_t> left_scores(n + 1, 0);
    for (std::size_t i = 1; i <= mid; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1])
                curr[j] = prev[j - 1] + 1;
            else
                curr[j] = std::max(prev[j], curr[j - 1]);
        }
        std::swap(prev, curr);
        std::fill(curr.begin(), curr.end(), 0);
    }
    left_scores = prev;

    // Backward pass: LCS lengths for reversed a[mid..m-1] vs reversed b
    std::fill(prev.begin(), prev.end(), 0);
    std::fill(curr.begin(), curr.end(), 0);
    std::vector<std::size_t> right_scores(n + 1, 0);
    for (std::size_t i = 1; i <= m - mid; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[mid + i - 1] == b[n - j])
                curr[j] = prev[j - 1] + 1;
            else
                curr[j] = std::max(prev[j], curr[j - 1]);
        }
        std::swap(prev, curr);
        std::fill(curr.begin(), curr.end(), 0);
    }
    for (std::size_t j = 0; j <= n; ++j)
        right_scores[j] = prev[n - j];

    // Find optimal split point in b
    std::size_t best_j = 0;
    std::size_t best_score = 0;
    for (std::size_t j = 0; j <= n; ++j) {
        std::size_t score = left_scores[j] + right_scores[j];
        if (score > best_score) {
            best_score = score;
            best_j = j;
        }
    }

    // Conquer: recurse on both halves
    std::string left = hirschberg_lcs(a.substr(0, mid), b.substr(0, best_j));
    std::string right = hirschberg_lcs(a.substr(mid), b.substr(best_j));
    return left + right;
}

// ---- Shortest Common Superstring (Greedy overlap) ----
// Greedy algorithm: repeatedly merge the pair with maximum overlap
inline std::string shortest_common_superstring(const std::vector<std::string>& strs) {
    if (strs.empty()) return "";
    if (strs.size() == 1) return strs[0];

    std::vector<std::string> remaining = strs;
    std::vector<bool> used(remaining.size(), false);

    while (true) {
        int best_i = -1, best_j = -1;
        std::size_t best_overlap = 0;
        for (std::size_t i = 0; i < remaining.size(); ++i) {
            if (used[i]) continue;
            for (std::size_t j = 0; j < remaining.size(); ++j) {
                if (i == j || used[j]) continue;
                // Find max overlap where remaining[i] is prefix, remaining[j] is suffix
                std::size_t max_k = std::min(remaining[i].size(), remaining[j].size());
                for (std::size_t k = max_k; k > 0; --k) {
                    if (remaining[i].substr(remaining[i].size() - k) ==
                        remaining[j].substr(0, k)) {
                        if (k > best_overlap) {
                            best_overlap = k;
                            best_i = static_cast<int>(i);
                            best_j = static_cast<int>(j);
                        }
                        break;
                    }
                }
            }
        }
        if (best_i == -1 || best_overlap == 0) break;
        // Merge: append remaining[best_j] after best_overlap
        remaining[best_i] += remaining[best_j].substr(best_overlap);
        used[best_j] = true;
    }

    std::string result;
    for (std::size_t i = 0; i < remaining.size(); ++i)
        if (!used[i]) result += remaining[i];
    return result;
}

// ---- Longest Common Subsequence (trace back) ----
inline std::string lcs_string(std::string_view a, std::string_view b) {
    std::size_t m = a.size(), n = b.size();
    std::vector<std::vector<std::size_t>> dp(m + 1, std::vector<std::size_t>(n + 1, 0));
    for (std::size_t i = 1; i <= m; ++i)
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    // Trace back
    std::string result;
    std::size_t i = m, j = n;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result += a[i - 1];
            --i; --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

}  // namespace dsa
