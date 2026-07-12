class bloom_filter {
public:
    bloom_filter(size_t expected_elements, double false_positive_rate = 0.01) {
        // Optimal size and hash count
        m_ = -expected_elements * std::log(false_positive_rate) 
             / (std::log(2) * std::log(2));
        k_ = std::max(1, static_cast<int>(std::round(
             (m_ / expected_elements) * std::log(2))));
        bits_.resize(m_, false);
    }

    void insert(std::string_view key) {
        auto [h1, h2] = hash(key);
        for (int i = 0; i < k_; ++i) {
            size_t pos = (h1 + i * h2) % m_;
            bits_[pos] = true;
        }
    }

    bool contains(std::string_view key) const {
        auto [h1, h2] = hash(key);
        for (int i = 0; i < k_; ++i) {
            size_t pos = (h1 + i * h2) % m_;
            if (!bits_[pos]) return false;  // definitely not present
        }
        return true;  // probably present
    }

private:
    // Double hashing
    std::pair<size_t, size_t> hash(std::string_view key) const {
        std::hash<std::string_view> h;
        size_t hval = h(key);
        return {hval, hval >> 16 | (hval << 16)};
    }

    size_t m_;  // bits in filter
    int k_;     // number of hash functions
    std::vector<bool> bits_;
};
