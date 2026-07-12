// Compute the prefix function (failure function)
std::vector<size_t> compute_prefix(std::string_view pattern) {
    size_t m = pattern.size();
    std::vector<size_t> pi(m, 0);
    size_t k = 0;

    for (size_t q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q]) {
            k = pi[k - 1];
        }
        if (pattern[k] == pattern[q]) ++k;
        pi[q] = k;
    }
    return pi;
}

std::vector<size_t> kmp_match(std::string_view text, std::string_view pattern) {
    std::vector<size_t> matches;
    auto pi = compute_prefix(pattern);
    size_t q = 0;  // number of matched characters

    for (size_t i = 0; i < text.size(); ++i) {
        while (q > 0 && pattern[q] != text[i]) {
            q = pi[q - 1];
        }
        if (pattern[q] == text[i]) ++q;
        if (q == pattern.size()) {
            matches.push_back(i - pattern.size() + 1);
            q = pi[q - 1];  // look for next match
        }
    }
    return matches;
}
