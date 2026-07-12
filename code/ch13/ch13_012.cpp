std::vector<size_t> build_lcp(std::string_view s,
                               const std::vector<size_t>& sa) {
    size_t n = s.size();
    std::vector<size_t> rank(n);
    for (size_t i = 0; i < n; ++i) rank[sa[i]] = i;

    std::vector<size_t> lcp(n, 0);
    size_t k = 0;

    for (size_t i = 0; i < n; ++i) {
        if (rank[i] == n - 1) { k = 0; continue; }
        size_t j = sa[rank[i] + 1];  // next suffix in SA
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
        lcp[rank[i]] = k;
        if (k > 0) --k;
    }
    return lcp;
}
