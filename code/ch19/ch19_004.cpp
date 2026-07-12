class count_min_sketch {
public:
    count_min_sketch(double epsilon, double delta) {
        // width = ceil(e / epsilon), depth = ceil(ln(1 / delta))
        width_ = static_cast<size_t>(std::ceil(std::exp(1) / epsilon));
        depth_ = static_cast<size_t>(std::ceil(std::log(1.0 / delta)));
        counts_.resize(depth_, std::vector<size_t>(width_, 0));
    }

    void add(std::string_view key, size_t count = 1) {
        for (size_t i = 0; i < depth_; ++i) {
            size_t pos = hash_i(key, i) % width_;
            counts_[i][pos] += count;
        }
    }

    size_t estimate(std::string_view key) const {
        size_t min_val = SIZE_MAX;
        for (size_t i = 0; i < depth_; ++i) {
            size_t pos = hash_i(key, i) % width_;
            min_val = std::min(min_val, counts_[i][pos]);
        }
        return min_val;
    }

private:
    size_t hash_i(std::string_view key, size_t i) const {
        return std::hash<std::string_view>{}(key) + i * 0x9e3779b97f4a7c15ULL;
    }

    size_t width_, depth_;
    std::vector<std::vector<size_t>> counts_;
};
