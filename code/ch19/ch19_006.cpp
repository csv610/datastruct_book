class hyperloglog {
public:
    hyperloglog(int precision = 14) : precision_(precision) {
        m_ = 1 << precision;  // number of registers
        registers_.resize(m_, 0);
    }

    void add(std::string_view key) {
        auto hash = std::hash<std::string_view>{}(key);
        size_t idx = hash >> (64 - precision_);  // first p bits
        uint8_t zeros = count_leading_zeros(hash << precision_);
        registers_[idx] = std::max(registers_[idx], zeros + 1);
    }

    double estimate() const {
        double sum = 0.0;
        size_t zeros = 0;
        for (uint8_t r : registers_) {
            sum += 1.0 / (1 << r);
            if (r == 0) ++zeros;
        }
        double e = alpha_m_ * m_ * m_ / sum;

        // Small range correction
        if (e <= 2.5 * m_) {
            if (zeros > 0) e = m_ * std::log(static_cast<double>(m_) / zeros);
        }
        return e;
    }

private:
    static uint8_t count_leading_zeros(size_t x) {
        return __builtin_clzll(x);
    }

    int precision_;
    size_t m_;
    std::vector<uint8_t> registers_;
    static constexpr double alpha_m_ = 0.7213 / (1 + 1.079 / (1 << 14));
};
