class genome_index {
public:
    explicit genome_index(std::string_view genome)
        : genome_(genome), sa_(build_suffix_array(genome)) {}

    bool contains(std::string_view pattern) const {
        // Binary search on suffix array
        auto comp = [&](size_t pos, std::string_view pat) {
            return genome_.substr(pos, pat.size()) < pat;
        };

        auto it = std::lower_bound(sa_.begin(), sa_.end(), pattern, comp);
        if (it == sa_.end()) return false;
        return genome_.substr(*it, pattern.size()) == pattern;
    }

    std::vector<size_t> find_all(std::string_view pattern) const {
        // Find range of matching suffixes
        auto [lo, hi] = std::equal_range(sa_.begin(), sa_.end(), pattern,
            [&](size_t a, std::string_view pat) {
                if (a == SIZE_MAX) return true;
                return genome_.substr(a, pat.size()) < pat;
            });
        return std::vector<size_t>(lo, hi);
    }

private:
    std::string genome_;
    std::vector<size_t> sa_;
};
