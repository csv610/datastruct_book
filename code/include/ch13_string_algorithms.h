#ifndef STRING_ALGORITHMS_H
#define STRING_ALGORITHMS_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// ---- Trie (lowercase letters) ----
template <typename V>
class trie {
    static constexpr std::size_t A = 26;
    struct node {
        std::array<std::unique_ptr<node>, A> children;
        V value{};
        bool is_end = false;
    };

public:
    void insert(std::string_view key, const V& value) {
        node* n = &root_;
        for (char c : key) {
            if (c < 'a' || c > 'z') continue;
            std::size_t idx = static_cast<std::size_t>(c - 'a');
            if (!n->children[idx])
                n->children[idx] = std::make_unique<node>();
            n = n->children[idx].get();
        }
        n->value = value;
        n->is_end = true;
    }

    std::optional<V> find(std::string_view key) const {
        const node* n = &root_;
        for (char c : key) {
            if (c < 'a' || c > 'z') return std::nullopt;
            std::size_t idx = static_cast<std::size_t>(c - 'a');
            if (!n->children[idx]) return std::nullopt;
            n = n->children[idx].get();
        }
        if (n->is_end) return n->value;
        return std::nullopt;
    }

    bool contains(std::string_view key) const { return find(key).has_value(); }

    bool starts_with(std::string_view prefix) const {
        const node* n = &root_;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') return false;
            std::size_t idx = static_cast<std::size_t>(c - 'a');
            if (!n->children[idx]) return false;
            n = n->children[idx].get();
        }
        return true;
    }

    std::vector<std::string> keys_with_prefix(std::string_view prefix) const {
        std::vector<std::string> result;
        const node* n = &root_;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') return result;
            std::size_t idx = static_cast<std::size_t>(c - 'a');
            if (!n->children[idx]) return result;
            n = n->children[idx].get();
        }
        std::string s(prefix);
        collect(n, s, result);
        return result;
    }

private:
    void collect(const node* n, std::string& prefix,
                 std::vector<std::string>& result) const {
        if (n->is_end) result.push_back(prefix);
        for (std::size_t i = 0; i < A; ++i) {
            if (n->children[i]) {
                prefix.push_back(static_cast<char>('a' + i));
                collect(n->children[i].get(), prefix, result);
                prefix.pop_back();
            }
        }
    }

    node root_;
};

// ---- KMP string matching ----
inline std::vector<std::size_t> compute_prefix(std::string_view pattern) {
    std::size_t m = pattern.size();
    std::vector<std::size_t> pi(m, 0);
    std::size_t k = 0;

    for (std::size_t q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q])
            k = pi[k - 1];
        if (pattern[k] == pattern[q]) ++k;
        pi[q] = k;
    }
    return pi;
}

// Returns start indices of all matches
inline std::vector<std::size_t> kmp_search(std::string_view text,
                                            std::string_view pattern) {
    if (pattern.empty()) return {};
    auto pi = compute_prefix(pattern);
    std::vector<std::size_t> matches;
    std::size_t q = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        while (q > 0 && pattern[q] != text[i])
            q = pi[q - 1];
        if (pattern[q] == text[i]) ++q;
        if (q == pattern.size()) {
            matches.push_back(i - pattern.size() + 1);
            q = pi[q - 1];
        }
    }
    return matches;
}

// ---- Rabin-Karp string matching ----
inline std::vector<std::size_t> rabin_karp_search(std::string_view text,
                                                    std::string_view pattern) {
    constexpr std::size_t base = 91138233;
    constexpr std::size_t mod = 97266353;

    std::size_t n = text.size(), m = pattern.size();
    if (m > n || m == 0) return {};

    // Precompute base^(m-1) % mod
    std::size_t h = 1;
    for (std::size_t i = 0; i < m - 1; ++i)
        h = (h * base) % mod;

    // Hash of pattern and first window
    std::size_t pat_hash = 0;
    std::size_t win_hash = 0;
    for (std::size_t i = 0; i < m; ++i) {
        pat_hash = (pat_hash * base + pattern[i]) % mod;
        win_hash = (win_hash * base + text[i]) % mod;
    }

    std::vector<std::size_t> matches;
    for (std::size_t i = 0; i <= n - m; ++i) {
        if (pat_hash == win_hash && text.substr(i, m) == pattern)
            matches.push_back(i);
        if (i < n - m) {
            win_hash = (win_hash - text[i] * h % mod + mod) % mod;
            win_hash = (win_hash * base + text[i + m]) % mod;
        }
    }
    return matches;
}

#endif
