std::string longest_common_substring(
        std::string_view a, std::string_view b) {
    std::string concat = std::string(a) + "$"
                       + std::string(b) + "#";
    int na = a.size();
    auto sa = build_suffix_array(concat);
    auto lcp = build_lcp(concat, sa);

    int best_len = 0, best_pos = 0;
    for (size_t i = 0; i + 1 < lcp.size(); ++i) {
        bool left_a = sa[i] < na;
        bool right_a = sa[i + 1] < na;
        if (left_a != right_a && lcp[i] > best_len) {
            best_len = lcp[i];
            best_pos = sa[i];
        }
    }
    return std::string(concat.substr(best_pos,
                                     best_len));
}
