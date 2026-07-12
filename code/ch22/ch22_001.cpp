class succinct_bitvector {
public:
    succinct_bitvector(const std::vector<bool>& bits)
        : n_(bits.size()),
          blocks_((n_ + 63) / 64),
          bits_(blocks_, 0),
          block_rank_(blocks_, 0)
    {
        for (size_t i = 0; i < n_; ++i)
            if (bits[i]) bits_[i / 64] |= 1ULL << (i % 64);

        uint64_t running = 0;
        for (size_t i = 0; i < blocks_; ++i) {
            block_rank_[i] = running;
            running += __builtin_popcountll(bits_[i]);
        }
    }

    uint64_t rank(uint64_t i) const {
        uint64_t block = i / 64;
        uint64_t pos = i % 64;
        uint64_t r = block > 0 ? block_rank_[block - 1] : 0;
        uint64_t mask = (pos == 64) ? ~0ULL : (1ULL << pos) - 1;
        r += __builtin_popcountll(bits_[block] & mask);
        return r;
    }

    uint64_t select(uint64_t k) const {
        uint64_t lo = 0, hi = n_;
        while (lo < hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (rank(mid) <= k) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    uint64_t operator[](uint64_t i) const {
        return (bits_[i / 64] >> (i % 64)) & 1;
    }

    uint64_t size() const { return n_; }

private:
    uint64_t n_, blocks_;
    std::vector<uint64_t> bits_;
    std::vector<uint64_t> block_rank_;
};
