std::vector<size_t> naive_match(std::string_view text, std::string_view pattern) {
    std::vector<size_t> matches;
    for (size_t i = 0; i <= text.size() - pattern.size(); ++i) {
        size_t j = 0;
        while (j < pattern.size() && text[i + j] == pattern[j]) ++j;
        if (j == pattern.size()) matches.push_back(i);
    }
    return matches;
}
