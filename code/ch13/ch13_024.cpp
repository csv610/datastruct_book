std::vector<int> compute_z(const std::string& s) {
    int n = s.size();
    std::vector<int> z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
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

std::vector<int> z_search(const std::string& text,
                           const std::string& pattern) {
    std::string concat = pattern + "$" + text;
    auto z = compute_z(concat);
    int m = pattern.size();
    std::vector<int> matches;
    for (int i = m + 1; i < (int)z.size(); ++i) {
        if (z[i] == m) matches.push_back(i - m - 1);
    }
    return matches;
}
