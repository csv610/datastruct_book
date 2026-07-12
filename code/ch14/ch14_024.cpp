class sparse_table_gcd {
public:
    explicit sparse_table_gcd(std::span<const int> data)
        : n_(data.size()), log_(std::bit_width(n_) - 1) {
        st_.assign(n_, std::vector<int>(log_ + 1));
        for (size_t i = 0; i < n_; ++i)
            st_[i][0] = data[i];

        for (size_t j = 1; j <= log_; ++j) {
            for (size_t i = 0; i + (1ULL << j) <= n_; ++i) {
                st_[i][j] = std::gcd(st_[i][j-1],
                                      st_[i + (1ULL << (j-1))][j-1]);
            }
        }
    }

    // Query gcd over [l, r] (0-indexed, inclusive)
    int query_gcd(size_t l, size_t r) const {
        size_t k = std::bit_width(r - l + 1) - 1;
        return std::gcd(st_[l][k], st_[r - (1ULL << k) + 1][k]);
    }

private:
    size_t n_, log_;
    std::vector<std::vector<int>> st_;
};
