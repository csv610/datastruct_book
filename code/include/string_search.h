#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace dsa {

// ---- Z-Algorithm ----
// z[i] = length of longest substring starting at i that is also a prefix
inline std::vector<std::size_t> z_function(std::string_view s) {
    std::size_t n = s.size();
    std::vector<std::size_t> z(n, 0);
    z[0] = n;
    std::size_t l = 0, r = 0;
    for (std::size_t i = 1; i < n; ++i) {
        if (i < r)
            z[i] = std::min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

// Pattern matching using Z-algorithm: returns all match positions
inline std::vector<std::size_t> z_search(std::string_view text,
                                          std::string_view pattern) {
    if (pattern.empty()) return {};
    std::string combined = std::string(pattern) + "$" + std::string(text);
    auto z = z_function(combined);
    std::size_t m = pattern.size();
    std::vector<std::size_t> matches;
    for (std::size_t i = m + 1; i < combined.size(); ++i)
        if (z[i] == m)
            matches.push_back(i - m - 1);
    return matches;
}

// ---- Boyer-Moore string search ----
class boyer_moore {
public:
    explicit boyer_moore(std::string_view pattern) : pattern_(pattern) {
        m_ = pattern.size();
        if (m_ == 0) return;

        // Bad character rule: for each character, distance from right end
        bad_char_.fill(static_cast<int>(m_));
        for (std::size_t i = 0; i < m_ - 1; ++i)
            bad_char_[static_cast<unsigned char>(pattern[i])] = static_cast<int>(m_ - 1 - i);

        // Good suffix: build suffix array of pattern
        std::vector<std::size_t> suff(m_);
        suff[m_ - 1] = m_;
        std::size_t g = m_ - 1, f = 0;
        for (std::size_t i = m_ - 1; i > 0; --i) {
            if (i > g && suff[i + m_ - 1 - f] < i - g)
                suff[i] = suff[i + m_ - 1 - f];
            else {
                if (i < g) g = i;
                f = i;
                while (g > 0 && pattern[g - 1] == pattern[g + m_ - 1 - f])
                    --g;
                suff[i] = f - g;
            }
        }

        // Build good suffix shift table
        good_suffix_.assign(m_ + 1, m_);
        for (std::size_t i = 0; i < m_; ++i)
            good_suffix_[i] = m_;
        for (std::size_t i = m_ - 1; i < m_; --i)
            if (suff[i] == i + 1)
                for (std::size_t j = 0; j < m_ - 1 - i; ++j)
                    if (good_suffix_[j] == m_)
                        good_suffix_[j] = m_ - 1 - i;
        for (std::size_t i = 0; i <= m_ - 2; ++i)
            good_suffix_[m_ - 1 - suff[i]] = m_ - 1 - i;
    }

    std::vector<std::size_t> search(std::string_view text) const {
        std::vector<std::size_t> matches;
        if (m_ == 0 || text.size() < m_) return matches;

        int s = 0;
        while (s <= static_cast<int>(text.size()) - static_cast<int>(m_)) {
            int j = static_cast<int>(m_) - 1;
            while (j >= 0 && pattern_[j] == text[s + j])
                --j;
            if (j < 0) {
                matches.push_back(static_cast<std::size_t>(s));
                s += static_cast<int>(good_suffix_[0]);
            } else {
                int bc_shift = bad_char_[static_cast<unsigned char>(text[s + j])] - (static_cast<int>(m_) - 1 - j);
                s += std::max(1, std::max(bc_shift, static_cast<int>(good_suffix_[j])));
            }
        }
        return matches;
    }

private:
    std::string pattern_;
    std::size_t m_;
    std::array<int, 256> bad_char_{};
    std::vector<std::size_t> good_suffix_;
};

// ---- Longest Common Prefix of two strings ----
inline std::size_t lcp(std::string_view a, std::string_view b) {
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    return i;
}

// ---- Longest Palindromic Substring (Manacher's O(n)) ----
inline std::string longest_palindrome(std::string_view s) {
    if (s.empty()) return "";
    std::string t(2 * s.size() + 1, '#');
    for (std::size_t i = 0; i < s.size(); ++i)
        t[2 * i + 1] = s[i];

    std::vector<int> p(t.size(), 0);
    int center = 0, right = 0;
    for (int i = 0; i < static_cast<int>(t.size()); ++i) {
        if (i < right)
            p[i] = std::min(right - i, p[2 * center - i]);
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < static_cast<int>(t.size()) &&
               t[i - p[i] - 1] == t[i + p[i] + 1])
            ++p[i];
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }
    }
    int max_len = 0, max_center = 0;
    for (int i = 0; i < static_cast<int>(p.size()); ++i)
        if (p[i] > max_len) { max_len = p[i]; max_center = i; }
    int start = (max_center - max_len) / 2;
    return std::string(s.substr(start, max_len));
}

}  // namespace dsa
