constexpr size_t base = 91138233;
constexpr size_t mod = 97266353;

std::vector<size_t> rabin_karp(std::string_view text, std::string_view pattern) {
    size_t n = text.size(), m = pattern.size();
    if (m > n) return {};

    // Precompute base^(m-1) mod mod
    size_t h = 1;
    for (size_t i = 0; i < m - 1; ++i) h = (h * base) % mod;

    // Compute hash of pattern and first window
    size_t pattern_hash = 0;
    size_t window_hash = 0;
    for (size_t i = 0; i < m; ++i) {
        pattern_hash = (pattern_hash * base + pattern[i]) % mod;
        window_hash = (window_hash * base + text[i]) % mod;
    }

    std::vector<size_t> matches;
    for (size_t i = 0; i <= n - m; ++i) {
        if (pattern_hash == window_hash) {
            // Verify (in case of hash collision)
            if (text.substr(i, m) == pattern) {
                matches.push_back(i);
            }
        }
        // Roll the hash
        if (i < n - m) {
            window_hash = (window_hash - text[i] * h % mod + mod) % mod;
            window_hash = (window_hash * base + text[i + m]) % mod;
        }
    }
    return matches;
}
