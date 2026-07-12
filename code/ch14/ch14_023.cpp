class sqrt_decomposition {
public:
    explicit sqrt_decomposition(std::span<const int> data)
        : n_(data.size()),
          block_size_(static_cast<size_t>(std::sqrt(n_) + 1)),
          num_blocks_((n_ + block_size_ - 1) / block_size_) {
        block_sum_.assign(num_blocks_, 0);
        block_min_.assign(num_blocks_, INT_MAX);

        for (size_t i = 0; i < n_; ++i) {
            block_sum_[i / block_size_] += data[i];
            block_min_[i / block_size_] = std::min(
                block_min_[i / block_size_], data[i]);
        }
        data_.assign(data.begin(), data.end());
    }

    // Range sum query over [l, r] inclusive
    int query_sum(size_t l, size_t r) const {
        int result = 0;
        size_t bl = l / block_size_;
        size_t br = r / block_size_;

        if (bl == br) {
            for (size_t i = l; i <= r; ++i) result += data_[i];
            return result;
        }
        // Left partial block
        for (size_t i = l; i < (bl + 1) * block_size_; ++i)
            result += data_[i];
        // Full blocks
        for (size_t b = bl + 1; b < br; ++b)
            result += block_sum_[b];
        // Right partial block
        for (size_t i = br * block_size_; i <= r; ++i)
            result += data_[i];
        return result;
    }

    // Range min query over [l, r] inclusive
    int query_min(size_t l, size_t r) const {
        int result = INT_MAX;
        size_t bl = l / block_size_;
        size_t br = r / block_size_;

        if (bl == br) {
            for (size_t i = l; i <= r; ++i)
                result = std::min(result, data_[i]);
            return result;
        }
        for (size_t i = l; i < (bl + 1) * block_size_; ++i)
            result = std::min(result, data_[i]);
        for (size_t b = bl + 1; b < br; ++b)
            result = std::min(result, block_min_[b]);
        for (size_t i = br * block_size_; i <= r; ++i)
            result = std::min(result, data_[i]);
        return result;
    }

    // Point update
    void update(size_t pos, int val) {
        size_t b = pos / block_size_;
        block_sum_[b] -= data_[pos];
        block_sum_[b] += val;
        data_[pos] = val;
        // Recompute block min (O(B) worst case)
        block_min_[b] = INT_MAX;
        size_t start = b * block_size_;
        size_t end = std::min(start + block_size_, n_);
        for (size_t i = start; i < end; ++i)
            block_min_[b] = std::min(block_min_[b], data_[i]);
    }

private:
    size_t n_, block_size_, num_blocks_;
    std::vector<int> data_;
    std::vector<int> block_sum_;
    std::vector<int> block_min_;
};
