#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace dsa {

// ---- Kasai et al. Algorithm ----
// Builds LCP array from suffix array in O(n) time
// lcp[i] = longest common prefix of suffixes sa[i] and sa[i-1]
inline std::vector<std::size_t> kasai_lcp(std::string_view text,
                                           const std::vector<std::size_t>& sa) {
    std::size_t n = text.size();
    std::vector<std::size_t> rank(n, 0);
    for (std::size_t i = 0; i < n; ++i) rank[sa[i]] = i;

    std::vector<std::size_t> lcp(n, 0);
    std::size_t k = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (rank[i] == 0) { k = 0; continue; }
        std::size_t j = sa[rank[i] - 1];
        while (i + k < n && j + k < n && text[i + k] == text[j + k]) ++k;
        lcp[rank[i]] = k;
        if (k > 0) --k;
    }
    return lcp;
}

// ---- Suffix Array Search (naive O(m log n)) ----
// Binary search on suffix array
inline std::vector<std::size_t> suffix_array_search(
    std::string_view text, const std::vector<std::size_t>& sa,
    std::string_view pattern) {
    std::vector<std::size_t> results;
    std::size_t m = pattern.size();
    std::size_t n = text.size();
    if (m == 0) return results;

    // Binary search for lower bound
    std::size_t lo = 0, hi = n;
    while (lo < hi) {
        std::size_t mid = (lo + hi) / 2;
        std::size_t suff_len = n - sa[mid];
        std::size_t cmp_len = std::min(m, suff_len);
        int cmp = 0;
        for (std::size_t i = 0; i < cmp_len; ++i) {
            if (text[sa[mid] + i] != pattern[i]) {
                cmp = (text[sa[mid] + i] < pattern[i]) ? -1 : 1;
                break;
            }
        }
        if (cmp == 0 && suff_len >= m) cmp = 0;
        else if (cmp == 0 && suff_len < m) cmp = -1;

        if (cmp < 0) lo = mid + 1;
        else hi = mid;
    }
    std::size_t start = lo;

    // Binary search for upper bound
    lo = start; hi = n;
    while (lo < hi) {
        std::size_t mid = (lo + hi) / 2;
        std::size_t suff_len = n - sa[mid];
        std::size_t cmp_len = std::min(m, suff_len);
        int cmp = 0;
        for (std::size_t i = 0; i < cmp_len; ++i) {
            if (text[sa[mid] + i] != pattern[i]) {
                cmp = (text[sa[mid] + i] < pattern[i]) ? -1 : 1;
                break;
            }
        }
        if (cmp == 0 && suff_len >= m) cmp = 0;
        else if (cmp == 0 && suff_len < m) cmp = -1;

        if (cmp <= 0) lo = mid + 1;
        else hi = mid;
    }
    std::size_t end = lo;

    for (std::size_t i = start; i < end; ++i)
        results.push_back(sa[i]);
    std::sort(results.begin(), results.end());
    return results;
}

// ---- Longest Previous Factor ----
// lpf[i] = length of longest previous factor ending at position i
// i.e., longest match of text[lpf[i]..i] with some earlier occurrence
inline std::vector<std::size_t> longest_previous_factor(
    std::string_view text, const std::vector<std::size_t>& sa,
    const std::vector<std::size_t>& lcp) {
    std::size_t n = text.size();
    std::vector<std::size_t> rank(n, 0);
    for (std::size_t i = 0; i < n; ++i) rank[sa[i]] = i;

    std::vector<std::size_t> lpf(n, 0);
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t r = rank[i];
        if (r > 0) lpf[i] = lcp[r];
    }
    return lpf;
}

// ---- φ array (Kärkkäinen-Manzini-Puglisi) ----
// phi[i] = sa[rank[i] - 1] (the suffix before suffix i in sorted order)
inline std::vector<std::size_t> phi_array(const std::vector<std::size_t>& sa) {
    std::size_t n = sa.size();
    std::vector<std::size_t> rank(n, 0);
    for (std::size_t i = 0; i < n; ++i) rank[sa[i]] = i;
    std::vector<std::size_t> phi(n, 0);
    for (std::size_t i = 0; i < n; ++i) {
        if (rank[i] == 0) phi[i] = n;
        else phi[i] = sa[rank[i] - 1];
    }
    return phi;
}

}  // namespace dsa
