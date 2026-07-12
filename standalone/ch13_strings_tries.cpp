// ch13: String Algorithms and Tries — Trie, KMP, Rabin-Karp, Suffix Array + LCP
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// ---- Trie (lowercase a-z) ----
template <typename V>
class trie {
    static constexpr size_t alphabet_size = 26;
    struct node {
        std::array<std::unique_ptr<node>, alphabet_size> children;
        V value{};
        bool is_end = false;
    };
public:
    void insert(std::string_view key, const V& value) {
        node* n = &root_;
        for (char c : key) {
            size_t idx = c - 'a';
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
            size_t idx = c - 'a';
            if (!n->children[idx]) return std::nullopt;
            n = n->children[idx].get();
        }
        if (n->is_end) return n->value;
        return std::nullopt;
    }

    bool starts_with(std::string_view prefix) const {
        const node* n = &root_;
        for (char c : prefix) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return false;
            n = n->children[idx].get();
        }
        return true;
    }

    std::vector<std::string> keys_with_prefix(std::string_view prefix) const {
        std::vector<std::string> result;
        const node* n = &root_;
        for (char c : prefix) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return result;
            n = n->children[idx].get();
        }
        collect_keys(n, std::string(prefix), result);
        return result;
    }

private:
    void collect_keys(const node* n, std::string prefix,
                      std::vector<std::string>& result) const {
        if (n->is_end) result.push_back(prefix);
        for (size_t i = 0; i < alphabet_size; ++i) {
            if (n->children[i])
                collect_keys(n->children[i].get(), prefix + char('a' + i), result);
        }
    }
    node root_;
};

// ---- KMP String Matching ----
std::vector<size_t> compute_prefix(std::string_view pattern) {
    size_t m = pattern.size();
    std::vector<size_t> pi(m, 0);
    size_t k = 0;
    for (size_t q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q])
            k = pi[k - 1];
        if (pattern[k] == pattern[q]) ++k;
        pi[q] = k;
    }
    return pi;
}

std::vector<size_t> kmp_match(std::string_view text, std::string_view pattern) {
    std::vector<size_t> matches;
    auto pi = compute_prefix(pattern);
    size_t q = 0;
    for (size_t i = 0; i < text.size(); ++i) {
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

// ---- Rabin-Karp Rolling Hash ----
constexpr size_t base = 91138233;
constexpr size_t mod = 97266353;

std::vector<size_t> rabin_karp(std::string_view text, std::string_view pattern) {
    size_t n = text.size(), m = pattern.size();
    if (m > n) return {};

    size_t h = 1;
    for (size_t i = 0; i < m - 1; ++i) h = (h * base) % mod;

    size_t pattern_hash = 0;
    size_t window_hash = 0;
    for (size_t i = 0; i < m; ++i) {
        pattern_hash = (pattern_hash * base + pattern[i]) % mod;
        window_hash = (window_hash * base + text[i]) % mod;
    }

    std::vector<size_t> matches;
    for (size_t i = 0; i <= n - m; ++i) {
        if (pattern_hash == window_hash) {
            if (text.substr(i, m) == pattern)
                matches.push_back(i);
        }
        if (i < n - m) {
            window_hash = (window_hash - text[i] * h % mod + mod) % mod;
            window_hash = (window_hash * base + text[i + m]) % mod;
        }
    }
    return matches;
}

// ---- Suffix Array (naive O(n^2 log n)) ----
std::vector<size_t> build_suffix_array(std::string_view s) {
    size_t n = s.size();
    std::vector<size_t> sa(n);
    std::iota(sa.begin(), sa.end(), 0);
    std::sort(sa.begin(), sa.end(), [&](size_t a, size_t b) {
        return s.substr(a) < s.substr(b);
    });
    return sa;
}

// ---- LCP Array (Kasai's algorithm) ----
std::vector<size_t> build_lcp(std::string_view s,
                               const std::vector<size_t>& sa) {
    size_t n = s.size();
    std::vector<size_t> rank(n);
    for (size_t i = 0; i < n; ++i) rank[sa[i]] = i;

    std::vector<size_t> lcp(n, 0);
    size_t k = 0;
    for (size_t i = 0; i < n; ++i) {
        if (rank[i] == n - 1) { k = 0; continue; }
        size_t j = sa[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
        lcp[rank[i]] = k;
        if (k > 0) --k;
    }
    return lcp;
}

std::string longest_repeated_substring(std::string_view s) {
    auto sa = build_suffix_array(s);
    auto lcp = build_lcp(s, sa);
    size_t max_lcp = 0;
    size_t max_pos = 0;
    for (size_t i = 0; i < lcp.size(); ++i) {
        if (lcp[i] > max_lcp) {
            max_lcp = lcp[i];
            max_pos = sa[i];
        }
    }
    return std::string(s.substr(max_pos, max_lcp));
}

int main() {
    std::cout << "=== Trie ===\n\n";
    trie<int> t;
    t.insert("cat", 1); t.insert("car", 2); t.insert("dog", 3);
    t.insert("do", 4); t.insert("cart", 5);

    std::cout << "find(\"cat\"): " << t.find("cat").value_or(-1) << "\n";
    std::cout << "find(\"catalog\"): " << t.find("catalog").value_or(-1) << "\n";
    std::cout << "starts_with(\"ca\"): " << t.starts_with("ca") << "\n";
    std::cout << "starts_with(\"zz\"): " << t.starts_with("zz") << "\n";
    std::cout << "keys_with_prefix(\"ca\"):";
    for (const auto& k : t.keys_with_prefix("ca")) std::cout << " " << k;
    std::cout << "\n\n";

    std::cout << "=== KMP String Matching ===\n\n";
    std::string text = "ababcabcabababd";
    std::string pattern = "ababd";
    auto kmp_matches = kmp_match(text, pattern);
    std::cout << "KMP matches for \"" << pattern << "\" in \"" << text << "\":";
    for (auto pos : kmp_matches) std::cout << " " << pos;
    std::cout << "\n\n";

    std::cout << "=== Rabin-Karp ===\n\n";
    auto rk_matches = rabin_karp(text, pattern);
    std::cout << "Rabin-Karp matches:";
    for (auto pos : rk_matches) std::cout << " " << pos;
    std::cout << "\n\n";

    std::cout << "=== Suffix Array + LCP ===\n\n";
    std::string s = "banana";
    auto sa = build_suffix_array(s);
    auto lcp = build_lcp(s, sa);
    std::cout << "Suffix array of \"" << s << "\":";
    for (auto idx : sa) std::cout << " " << idx;
    std::cout << "\nLCP array:";
    for (auto l : lcp) std::cout << " " << l;
    std::cout << "\nLongest repeated substring: \""
              << longest_repeated_substring(s) << "\"\n\n";

    std::cout << "All ch13 tests passed.\n";
    return 0;
}
