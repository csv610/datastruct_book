#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dsa {

// ---- Maximum Suffix (algorithm based on critical factorization) ----
// Returns (position, length) of the maximum suffix of s
// A maximum suffix is the lexicographically largest suffix
inline std::pair<std::size_t, std::size_t> maximum_suffix(std::string_view s) {
    if (s.empty()) return {0, 0};
    std::size_t n = s.size();
    std::size_t i = 0, j = 1, k = 0;
    while (j + k < n) {
        if (s[i + k] == s[j + k]) {
            ++k;
        } else if (s[i + k] < s[j + k]) {
            i = j;
            j = i + 1;
            k = 0;
        } else {
            j += k + 1;
            k = 0;
        }
    }
    return {i, n - i};
}

// Maximum suffix in constant space (two directions)
// Returns the starting position of the maximum suffix
inline std::size_t maximum_suffix_ascending(std::string_view s) {
    if (s.empty()) return 0;
    std::size_t n = s.size();
    std::size_t i = 0, j = 1, k = 0;
    while (j + k < n) {
        char a = s[(i + k) % n], b = s[(j + k) % n];
        if (a == b) { ++k; continue; }
        if (a < b) { i = j; j = i + 1; }
        else { j += k + 1; }
        k = 0;
        if (i == j) ++j;
    }
    return i;
}

inline std::size_t maximum_suffix_descending(std::string_view s) {
    if (s.empty()) return 0;
    std::size_t n = s.size();
    std::size_t i = 0, j = 1, k = 0;
    while (j + k < n) {
        char a = s[(i + k) % n], b = s[(j + k) % n];
        if (a == b) { ++k; continue; }
        if (a > b) { i = j; j = i + 1; }
        else { j += k + 1; }
        k = 0;
        if (i == j) ++j;
    }
    return i;
}

// ---- Duval's Lyndon Factorization ----
// Decomposes string into non-increasing sequence of Lyndon words
// A Lyndon word is strictly lexicographically smaller than all its rotations
struct lyndon_factor {
    std::size_t start;
    std::size_t length;
};

inline std::vector<lyndon_factor> lyndon_factorization(std::string_view s) {
    std::vector<lyndon_factor> factors;
    std::size_t i = 0;
    std::size_t n = s.size();
    while (i < n) {
        std::size_t j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j])
                k = i;
            else
                ++k;
            ++j;
        }
        while (i <= k) {
            factors.push_back({i, j - k});
            i += j - k;
        }
    }
    return factors;
}

// Lyndon word check: is s a Lyndon word?
inline bool is_lyndon_word(std::string_view s) {
    std::size_t n = s.size();
    if (n <= 1) return true;
    for (std::size_t i = 1; i < n; ++i) {
        std::size_t j = 0;
        while (j < n && s[(i + j) % n] == s[j]) ++j;
        if (j == n || s[(i + j) % n] < s[j]) return false;
    }
    return true;
}

// ---- Critical Factorization ----
// Finds position k such that the maximum suffix decomposition is unique
// Returns (k, l) where l is the period of s[k..n-1]
inline std::pair<std::size_t, std::size_t> critical_factorization(std::string_view s) {
    auto [pos, len] = maximum_suffix(s);
    std::size_t n = s.size();
    // Compute period of the maximum suffix
    std::size_t period = len;
    for (std::size_t i = pos + 1; i < n; ++i) {
        if (s[i] != s[pos + (i - pos) % period]) {
            period = i - pos;
            break;
        }
    }
    return {pos, period};
}

}  // namespace dsa
