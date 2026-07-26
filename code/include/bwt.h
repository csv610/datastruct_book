#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dsa {

// ---- Burrows-Wheeler Transform ----
inline std::string bwt_encode(std::string_view s) {
    std::size_t n = s.size();
    std::string t(s);
    t += '\0';  // sentinel
    n = t.size();

    std::vector<std::size_t> sa(n);
    for (std::size_t i = 0; i < n; ++i) sa[i] = i;

    // SA-IS could be used; for simplicity, use the O(n log^2 n) doubling
    std::vector<std::size_t> rank_(n), tmp(n);
    for (std::size_t i = 0; i < n; ++i) rank_[i] = t[i];

    for (std::size_t gap = 1; gap < n; gap *= 2) {
        auto cmp = [&rank_, gap, n](std::size_t a, std::size_t b) {
            if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
            std::size_t ra = (a + gap < n) ? rank_[a + gap] : 0;
            std::size_t rb = (b + gap < n) ? rank_[b + gap] : 0;
            return ra < rb;
        };
        std::sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (std::size_t i = 1; i < n; ++i)
            tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        rank_ = tmp;
        if (rank_[sa[n - 1]] == n - 1) break;
    }

    // BWT: last column of sorted rotations
    std::string result;
    result.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
        result += t[(sa[i] + n - 1) % n];
    return result;
}

inline std::string bwt_decode(std::string_view bwt) {
    std::size_t n = bwt.size();
    if (n == 0) return "";

    // Count occurrences of each character
    std::array<std::size_t, 256> count{};
    for (char c : bwt) count[static_cast<unsigned char>(c)]++;

    // Compute cumulative counts (C array)
    std::array<std::size_t, 256> cumulative{};
    std::size_t total = 0;
    for (int i = 0; i < 256; ++i) {
        cumulative[i] = total;
        total += count[i];
    }

    // Build LF-mapping
    std::vector<std::size_t> lf(n);
    std::array<std::size_t, 256> occ{};
    for (std::size_t i = 0; i < n; ++i) {
        unsigned char c = static_cast<unsigned char>(bwt[i]);
        lf[i] = cumulative[c] + occ[c]++;
    }

    // Reconstruct by following LF-mapping from sentinel
    std::size_t sentinel_pos = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (bwt[i] == '\0') { sentinel_pos = i; break; }
    }

    std::string result;
    result.reserve(n > 0 ? n - 1 : 0);
    std::size_t idx = lf[sentinel_pos];
    for (std::size_t i = 0; i < n - 1; ++i) {
        result += bwt[idx];
        idx = lf[idx];
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ---- FM-Index (built on BWT) ----
class fm_index {
public:
    explicit fm_index(std::string_view text) : text_(text) {
        std::string t(text);
        t += '\0';
        std::size_t n = t.size();
        bwt_.reserve(n);
        std::vector<std::size_t> sa(n);
        for (std::size_t i = 0; i < n; ++i) sa[i] = i;

        std::vector<std::size_t> rank_(n), tmp(n);
        for (std::size_t i = 0; i < n; ++i) rank_[i] = t[i];

        for (std::size_t gap = 1; gap < n; gap *= 2) {
            auto cmp = [&rank_, gap, n](std::size_t a, std::size_t b) {
                if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
                std::size_t ra = (a + gap < n) ? rank_[a + gap] : 0;
                std::size_t rb = (b + gap < n) ? rank_[b + gap] : 0;
                return ra < rb;
            };
            std::sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (std::size_t i = 1; i < n; ++i)
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            rank_ = tmp;
            if (rank_[sa[n - 1]] == n - 1) break;
        }

        for (std::size_t i = 0; i < n; ++i)
            bwt_ += t[(sa[i] + n - 1) % n];

        // Build C array and Occ array
        build_c_and_occ();
    }

    // Count occurrences of pattern in text
    std::size_t count(std::string_view pattern) const {
        auto [lo, hi] = search_range(pattern);
        return (lo <= hi) ? hi - lo + 1 : 0;
    }

    // Find all positions of pattern
    std::vector<std::size_t> locate(std::string_view pattern) const {
        auto [lo, hi] = search_range(pattern);
        std::vector<std::size_t> results;
        if (lo > hi) return results;

        // Build SA from text to map SA-index back to positions
        std::string t(text_);
        t += '\0';
        std::size_t n = t.size();
        std::vector<std::size_t> sa(n);
        for (std::size_t i = 0; i < n; ++i) sa[i] = i;
        std::vector<std::size_t> rank_(n), tmp(n);
        for (std::size_t i = 0; i < n; ++i) rank_[i] = t[i];
        for (std::size_t gap = 1; gap < n; gap *= 2) {
            auto cmp = [&rank_, gap, n](std::size_t a, std::size_t b) {
                if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
                std::size_t ra = (a + gap < n) ? rank_[a + gap] : 0;
                std::size_t rb = (b + gap < n) ? rank_[b + gap] : 0;
                return ra < rb;
            };
            std::sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (std::size_t i = 1; i < n; ++i)
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            rank_ = tmp;
            if (rank_[sa[n - 1]] == n - 1) break;
        }

        for (std::size_t i = lo; i <= hi; ++i)
            results.push_back(sa[i]);
        std::sort(results.begin(), results.end());
        return results;
    }

    // Restore original text
    std::string extract() const {
        return bwt_decode(bwt_);
    }

private:
    std::pair<std::size_t, std::size_t> search_range(std::string_view pattern) const {
        if (pattern.empty()) return {0, 0};
        std::size_t lo = 0, hi = bwt_.size() - 1;
        for (int i = static_cast<int>(pattern.size()) - 1; i >= 0; --i) {
            unsigned char c = static_cast<unsigned char>(pattern[i]);
            lo = C_[c] + occ_[c][lo];
            hi = C_[c] + occ_[c][hi + 1] - 1;
            if (lo > hi) return {1, 0};
        }
        return {lo, hi};
    }

    void build_c_and_occ() {
        C_.fill(0);
        std::size_t n = bwt_.size();

        // Count characters
        std::array<std::size_t, 256> count{};
        for (char c : bwt_) count[static_cast<unsigned char>(c)]++;

        // C[c] = number of characters strictly less than c
        std::size_t total = 0;
        for (int i = 0; i < 256; ++i) {
            C_[i] = total;
            total += count[i];
        }

        // Occ[c][i] = number of occurrences of c in BWT[0..i-1]
        occ_.assign(256, std::vector<std::size_t>(n + 1, 0));
        for (int c = 0; c < 256; ++c) {
            for (std::size_t i = 0; i < n; ++i) {
                occ_[c][i + 1] = occ_[c][i] + (bwt_[i] == static_cast<char>(c) ? 1 : 0);
            }
        }
    }

    std::string text_;
    std::string bwt_;
    std::array<std::size_t, 256> C_{};
    std::vector<std::vector<std::size_t>> occ_;
};

}  // namespace dsa
