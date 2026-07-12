#include <vector>
#include <cstdint>
#include <functional>
#include <optional>

class quotient_filter {
public:
    quotient_filter(size_t capacity, size_t fingerprint_bits = 8)
        : r_(fingerprint_bits),
          size_(1ULL << (capacity > 0 ? compute_q_bits(capacity, fingerprint_bits) : 4)),
          mask_(size_ - 1),
          fingerprints_(size_, 0),
          occupied_(size_, 0),
          continuations_(size_, 0),
          counts_(size_, 0),
          size__(0)
    {
        q_bits_ = compute_q_bits(capacity, fingerprint_bits);
    }

    bool insert(uint64_t key) {
        uint64_t f = fingerprint(key);
        uint64_t q = f >> r_;
        uint64_t rem = f & ((1ULL << r_) - 1);
        if (counts_[q] == 0) {
            // Empty cluster -- place directly
            size_t pos = q;
            while (occupied_[pos]) pos = (pos + 1) & mask_;
            place(pos, q, rem);
            return true;
        }
        // Find the run for quotient q
        size_t run_start = find_run_start(q);
        // Check for duplicate
        size_t pos = run_start;
        for (size_t i = 0; i < counts_[q]; ++i) {
            if (fingerprints_[pos] == rem) return false;  // already present
            pos = (pos + 1) & mask_;
        }
        // Insert at end of run
        place(pos, q, rem);
        return true;
    }

    bool lookup(uint64_t key) const {
        uint64_t f = fingerprint(key);
        uint64_t q = f >> r_;
        uint64_t rem = f & ((1ULL << r_) - 1);
        if (counts_[q] == 0) return false;
        size_t pos = find_run_start(q);
        for (size_t i = 0; i < counts_[q]; ++i) {
            if (fingerprints_[pos] == rem) return true;
            pos = (pos + 1) & mask_;
        }
        return false;
    }

    bool remove(uint64_t key) {
        uint64_t f = fingerprint(key);
        uint64_t q = f >> r_;
        uint64_t rem = f & ((1ULL << r_) - 1);
        if (counts_[q] == 0) return false;
        size_t pos = find_run_start(q);
        for (size_t i = 0; i < counts_[q]; ++i) {
            if (fingerprints_[pos] == rem) {
                // Shift run left and compact
                compact(pos, q);
                return true;
            }
            pos = (pos + 1) & mask_;
        }
        return false;
    }

    size_t size() const { return size__; }

private:
    size_t r_, q_bits_, size_, mask_;
    size_t size__;
    std::vector<uint64_t> fingerprints_;
    std::vector<bool> occupied_;
    std::vector<bool> continuations_;
    std::vector<size_t> counts_;

    static size_t compute_q_bits(size_t cap, size_t r) {
        size_t total_slots = cap * 2;  // 50% load factor
        size_t q = 0;
        while ((1ULL << q) < total_slots) ++q;
        return q;
    }

    uint64_t fingerprint(uint64_t key) const {
        // SplitMix64-style hash
        key ^= key >> 30;
        key *= 0xbf58476d1ce4e5b9ULL;
        key ^= key >> 27;
        key *= 0x94d049bb133111ebULL;
        key ^= key >> 31;
        return key & ((1ULL << (q_bits_ + r_)) - 1);
    }

    size_t find_run_start(uint64_t q) const {
        // Count occupied slots before q
        size_t count = 0;
        for (size_t i = 0; i < q; ++i)
            if (occupied_[i]) ++count;
        // Scan from q to find the (count+1)-th occupied cluster
        size_t pos = q;
        size_t seen = 0;
        while (seen < count) {
            pos = (pos + 1) & mask_;
            if (occupied_[pos]) ++seen;
        }
        return (pos + 1) & mask_;
    }

    void place(size_t pos, uint64_t q, uint64_t rem) {
        fingerprints_[pos] = rem;
        occupied_[q] = true;
        ++counts_[q];
        ++size__;
    }

    void compact(size_t pos, uint64_t q) {
        // Shift elements left to fill the gap
        size_t next = (pos + 1) & mask_;
        while (counts_[q] > 1 || (next != pos && occupied_[next])) {
            fingerprints_[pos] = fingerprints_[next];
            pos = next;
            next = (next + 1) & mask_;
        }
        // Clear the last slot
        occupied_[q] = false;
        if (counts_[q] > 0) --counts_[q];
        --size__;
    }
};
