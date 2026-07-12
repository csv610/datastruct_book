class bit_array {
public:
    explicit bit_array(size_t n)
        : data_((n + 63) / 64, 0), size_(n) {}

    bool get(size_t i) const {
        return (data_[i / 64] >> (i % 64)) & 1;
    }

    void set(size_t i, bool val = true) {
        if (val)
            data_[i / 64] |= (1ULL << (i % 64));
        else
            data_[i / 64] &= ~(1ULL << (i % 64));
    }

    void clear(size_t i) { set(i, false); }

    // Count set bits (population count) using hardware intrinsic
    size_t count() const {
        size_t total = 0;
        for (auto word : data_)
            total += std::popcount(word);
        return total;
    }

    // Bitwise operations
    bit_array operator&(const bit_array& o) const {
        bit_array result(size_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] & o.data_[i];
        return result;
    }
    bit_array operator|(const bit_array& o) const { /* similar */ }
    bit_array operator~() const {
        bit_array result(size_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = ~data_[i];
        return result;
    }

    size_t size() const noexcept { return size_; }

private:
    std::vector<uint64_t> data_;
    size_t size_;
};
