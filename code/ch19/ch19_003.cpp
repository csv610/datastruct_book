class cuckoo_filter {
public:
    cuckoo_filter(size_t capacity, double target_fpr = 0.01)
        : buckets_(capacity), bucket_size_(4) {
        // fingerprint size in bits: ceil(log2(1/fpr)) + 3 for amortized analysis
        fp_bits_ = static_cast<int>(std::ceil(std::log2(1.0 / target_fpr))) + 3;
        fp_mask_ = (1ULL << fp_bits_) - 1;
    }

    void insert(std::string_view key) {
        auto f = fingerprint(key);
        auto [i1, i2] = bucket_indices(key, f);

        // Try to insert into either bucket
        if (try_insert(i1, f)) return;
        if (try_insert(i2, f)) return;

        // Both full -- evict from the first bucket
        for (int n = 0; n < max_kicks; ++n) {
            size_t victim_idx = (n % 2 == 0) ? i1 : i2;
            auto evicted = kickout(victim_idx, f);
            f = evicted;
            // Compute alternate bucket for the evicted fingerprint
            victim_idx = alt_index(victim_idx, evicted);
            if (try_insert(victim_idx, f)) return;
        }
        // If we get here, filter is too full -- rebuild with larger array
    }

    bool contains(std::string_view key) const {
        auto f = fingerprint(key);
        auto [i1, i2] = bucket_indices(key, f);
        return bucket_contains(i1, f) || bucket_contains(i2, f);
    }

    void erase(std::string_view key) {
        auto f = fingerprint(key);
        auto [i1, i2] = bucket_indices(key, f);
        if (remove_from_bucket(i1, f)) return;
        remove_from_bucket(i2, f);
    }

private:
    static constexpr int max_kicks = 500;
    static constexpr int bucket_size_ = 4;

    struct bucket {
        uint64_t fingerprints[4] = {0, 0, 0, 0};
        uint8_t count = 0;
    };

    std::vector<bucket> buckets_;
    int fp_bits_;
    uint64_t fp_mask_;

    uint64_t fingerprint(std::string_view key) const {
        uint64_t h = std::hash<std::string_view>{}(key);
        return (h ^ (h >> 32)) & fp_mask_;
    }

    std::pair<size_t, size_t> bucket_indices(std::string_view key, uint64_t f) const {
        uint64_t h = std::hash<std::string_view>{}(key);
        size_t i1 = h % buckets_.size();
        size_t i2 = alt_index(i1, f);
        return {i1, i2};
    }

    // Partial-key hashing: derive the alternate index from the bucket index
    // and fingerprint alone (no need to re-hash the original key)
    size_t alt_index(size_t index, uint64_t f) const {
        return index ^ static_cast<size_t>(
            ((f >> 16) ^ (f * 0x517cc1b727220a95ULL)) % buckets_.size());
    }

    bool try_insert(size_t idx, uint64_t f) {
        if (buckets_[idx].count < bucket_size_) {
            buckets_[idx].fingerprints[buckets_[idx].count++] = f;
            return true;
        }
        return false;
    }

    uint64_t kickout(size_t idx, uint64_t new_f) {
        int slot = buckets_[idx].count - 1;
        uint64_t old_f = buckets_[idx].fingerprints[slot];
        buckets_[idx].fingerprints[slot] = new_f;
        return old_f;
    }

    bool bucket_contains(size_t idx, uint64_t f) const {
        for (int i = 0; i < buckets_[idx].count; ++i)
            if (buckets_[idx].fingerprints[i] == f) return true;
        return false;
    }

    bool remove_from_bucket(size_t idx, uint64_t f) {
        for (int i = 0; i < buckets_[idx].count; ++i) {
            if (buckets_[idx].fingerprints[i] == f) {
                buckets_[idx].fingerprints[i] =
                    buckets_[idx].fingerprints[--buckets_[idx].count];
                return true;
            }
        }
        return false;
    }
};
