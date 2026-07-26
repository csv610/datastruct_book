#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace dsa {

// ---- Hamming Distance ----
inline std::size_t hamming_distance(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return std::string::npos;
    std::size_t dist = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i] != b[i]) ++dist;
    return dist;
}

// ---- Levenshtein Distance (edit distance) ----
inline std::size_t levenshtein(std::string_view a, std::string_view b) {
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

// ---- Myers' Bit-Parallel Approximate Matching ----
// Finds all positions in text where pattern matches with <= k mismatches
// Uses the shift-and bit-vector algorithm (Myers 1999 formulation)
inline std::vector<std::size_t> myers_approximate(
    std::string_view text, std::string_view pattern, int k) {

    std::size_t m = pattern.size();
    std::vector<std::size_t> results;
    if (m == 0) return results;
    if (m > 64) {
        for (std::size_t i = 0; i + m <= text.size(); ++i)
            if (static_cast<int>(hamming_distance(text.substr(i, m), pattern)) <= k)
                results.push_back(i);
        return results;
    }

    // B[c] has bit j set iff pattern[j] == c
    std::array<uint64_t, 256> B{};
    for (std::size_t j = 0; j < m; ++j)
        B[static_cast<unsigned char>(pattern[j])] |= (1ULL << j);

    uint64_t all_ones = (m == 64) ? ~0ULL : ((1ULL << m) - 1);
    uint64_t MSB = 1ULL << (m - 1);

    // D[e] = state with exactly e errors allowed
    // In shift-and: bit j = 1 means pattern[0..j] matches current text window
    std::vector<uint64_t> D(k + 1, 0);

    for (std::size_t i = 0; i < text.size(); ++i) {
        uint64_t prev = all_ones;
        for (int e = 0; e <= k; ++e) {
            uint64_t tmp = D[e];
            D[e] = ((D[e] << 1) | 1) & B[static_cast<unsigned char>(text[i])];
            if (e > 0) D[e] |= ((prev << 1) | 1);
            D[e] &= all_ones;
            prev = tmp;
        }
        if (i >= m - 1 && (D[k] & MSB) != 0)
            results.push_back(i - m + 1);
    }
    return results;
}

// ---- Shift-Or (Baeza-Yates-Gonnet) approximate matching ----
// Bit-parallel for small alphabets, within-k-errors
inline std::vector<std::size_t> shift_or_approximate(
    std::string_view text, std::string_view pattern, int k) {

    std::size_t m = pattern.size();
    std::vector<std::size_t> results;
    if (m == 0) return results;
    if (m > 64) {
        for (std::size_t i = 0; i + m <= text.size(); ++i)
            if (static_cast<int>(hamming_distance(text.substr(i, m), pattern)) <= k)
                results.push_back(i);
        return results;
    }

    // B[c] has bit j set iff pattern[j] == c
    std::array<uint64_t, 256> B{};
    for (std::size_t j = 0; j < m; ++j)
        B[static_cast<unsigned char>(pattern[j])] |= (1ULL << j);

    uint64_t all_ones = (m == 64) ? ~0ULL : ((1ULL << m) - 1);
    uint64_t MSB = 1ULL << (m - 1);

    // D[e] = state with exactly e errors allowed
    std::vector<uint64_t> D(k + 1, 0);

    for (std::size_t i = 0; i < text.size(); ++i) {
        uint64_t prev = all_ones;
        for (int e = 0; e <= k; ++e) {
            uint64_t tmp = D[e];
            D[e] = ((D[e] << 1) | 1) & B[static_cast<unsigned char>(text[i])];
            if (e > 0) D[e] |= ((prev << 1) | 1);
            D[e] &= all_ones;
            prev = tmp;
        }
        if (i >= m - 1 && (D[k] & MSB) != 0)
            results.push_back(i - m + 1);
    }
    return results;
}

// ---- Approximate pattern matching within edit distance k ----
// Returns all (position, distance) pairs where edit_distance <= k
inline std::vector<std::pair<std::size_t, std::size_t>> approximate_search(
    std::string_view text, std::string_view pattern, std::size_t k) {

    std::vector<std::pair<std::size_t, std::size_t>> results;
    std::size_t m = pattern.size();
    if (m == 0) return results;

    for (std::size_t i = 0; i + m <= text.size(); ++i) {
        auto d = levenshtein(text.substr(i, m), pattern);
        if (d <= k)
            results.push_back({i, d});
    }
    return results;
}

}  // namespace dsa
