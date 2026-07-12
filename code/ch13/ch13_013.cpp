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
