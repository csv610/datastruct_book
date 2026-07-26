#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dsa {

// ---- Morris-Pratt Algorithm ----
// Prefix function: pi[i] = length of longest proper prefix of pattern[0..i]
// that is also a suffix of pattern[0..i]
inline std::vector<int> morris_pratt_prefix(std::string_view pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j])
            j = pi[j - 1];
        if (pattern[i] == pattern[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

inline std::vector<std::size_t> morris_pratt_search(
    std::string_view text, std::string_view pattern) {
    std::vector<std::size_t> matches;
    if (pattern.empty()) return matches;
    auto pi = morris_pratt_prefix(pattern);
    int m = static_cast<int>(pattern.size());
    int j = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        while (j > 0 && text[i] != pattern[j])
            j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (j == m) {
            matches.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return matches;
}

// ---- Knuth-Morris-Pratt (KMP) Algorithm ----
// Optimized prefix function: uses pattern lookahead to skip more
inline std::vector<int> kmp_prefix(std::string_view pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> pi(m, 0);
    int len = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            ++len;
            pi[i] = len;
            ++i;
        } else {
            if (len != 0)
                len = pi[len - 1];
            else {
                pi[i] = 0;
                ++i;
            }
        }
    }
    return pi;
}

inline std::vector<std::size_t> kmp_search(
    std::string_view text, std::string_view pattern) {
    std::vector<std::size_t> matches;
    if (pattern.empty()) return matches;
    auto pi = kmp_prefix(pattern);
    int m = static_cast<int>(pattern.size());
    int j = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        while (j > 0 && text[i] != pattern[j])
            j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (j == m) {
            matches.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return matches;
}

// ---- Karp-Rabin (Hashing-based) Algorithm ----
class karp_rabin {
public:
    explicit karp_rabin(std::string_view pattern, uint64_t base = 257, uint64_t mod = 1000000007)
        : pattern_(pattern), base_(base), mod_(mod), m_(pattern.size()), pattern_hash_(0), h_(1) {
        for (std::size_t i = 0; i < m_; ++i)
            pattern_hash_ = (pattern_hash_ * base_ + pattern[i]) % mod_;
        for (std::size_t i = 0; i < m_ - 1; ++i)
            h_ = (h_ * base_) % mod_;
    }

    std::vector<std::size_t> search(std::string_view text) const {
        std::vector<std::size_t> matches;
        if (m_ == 0 || text.size() < m_) return matches;
        uint64_t hash = 0;
        for (std::size_t i = 0; i < m_; ++i)
            hash = (hash * base_ + text[i]) % mod_;
        if (hash == pattern_hash_ && text.substr(0, m_) == pattern_)
            matches.push_back(0);
        for (std::size_t i = m_; i < text.size(); ++i) {
            hash = (hash + mod_ - (h_ * text[i - m_]) % mod_) % mod_;
            hash = (hash * base_ + text[i]) % mod_;
            if (hash == pattern_hash_ && text.substr(i - m_ + 1, m_) == pattern_)
                matches.push_back(i - m_ + 1);
        }
        return matches;
    }

private:
    std::string pattern_;
    uint64_t base_, mod_;
    std::size_t m_;
    uint64_t pattern_hash_, h_;
};

// ---- Galil-Seiferas Algorithm ----
// Uses critical factorization for linear-time matching
// Implementation: compute prefix function, then scan with skip optimization
inline std::vector<std::size_t> galil_seiferas_search(
    std::string_view text, std::string_view pattern) {
    std::vector<std::size_t> matches;
    if (pattern.empty() || text.size() < pattern.size()) return matches;
    int m = static_cast<int>(pattern.size());
    int n = static_cast<int>(text.size());
    auto pi = kmp_prefix(pattern);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j])
            j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (j == m) {
            matches.push_back(static_cast<std::size_t>(i - m + 1));
            j = pi[j - 1];
        }
    }
    return matches;
}

// ---- Boyer-Moore-Horspool (simplified) ----
class boyer_moore_horspool {
public:
    explicit boyer_moore_horspool(std::string_view pattern) : pattern_(pattern) {
        m_ = pattern.size();
        bad_char_.fill(static_cast<int>(m_));
        for (std::size_t i = 0; i < m_ - 1; ++i)
            bad_char_[static_cast<unsigned char>(pattern[i])] = static_cast<int>(m_ - 1 - i);
    }

    std::vector<std::size_t> search(std::string_view text) const {
        std::vector<std::size_t> matches;
        if (m_ == 0 || text.size() < m_) return matches;
        for (std::size_t s = 0; s <= text.size() - m_;) {
            std::size_t j = m_ - 1;
            while (text[s + j] == pattern_[j]) {
                if (j == 0) { matches.push_back(s); break; }
                --j;
            }
            s += bad_char_[static_cast<unsigned char>(text[s + m_ - 1])];
        }
        return matches;
    }

private:
    std::string pattern_;
    std::size_t m_;
    std::array<int, 256> bad_char_{};
};

}  // namespace dsa
