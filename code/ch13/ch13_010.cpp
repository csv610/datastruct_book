std::vector<size_t> build_suffix_array(std::string_view s) {
    size_t n = s.size();
    std::vector<size_t> sa(n);
    std::iota(sa.begin(), sa.end(), 0);

    std::sort(sa.begin(), sa.end(), [&](size_t a, size_t b) {
        return s.substr(a) < s.substr(b);
    });
    return sa;
}
