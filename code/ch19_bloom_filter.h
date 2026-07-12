#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <array>
#include <bit>
#include <cstddef>
#include <functional>
#include <string_view>
#include <vector>

// ---- Bloom Filter ----
template <std::size_t Bits = 1024, std::size_t K = 3>
class bloom_filter {
public:
    bloom_filter() : bits_(Bits / 64 + (Bits % 64 != 0), 0) {}

    void insert(std::string_view key) {
        for (auto h : hashes(key))
            bits_[h / 64] |= (1ULL << (h % 64));
    }

    [[nodiscard]] bool might_contain(std::string_view key) const {
        for (auto h : hashes(key))
            if (!(bits_[h / 64] & (1ULL << (h % 64))))
                return false;
        return true;
    }

    // For union/intersection with other bloom filters
    void merge_with(const bloom_filter& other) {
        for (std::size_t i = 0; i < bits_.size(); ++i)
            bits_[i] |= other.bits_[i];
    }

    void intersect_with(const bloom_filter& other) {
        for (std::size_t i = 0; i < bits_.size(); ++i)
            bits_[i] &= other.bits_[i];
    }

    double false_positive_rate() const {
        // Theoretical rate: (1 - e^(-K * N / Bits))^K
        // We don't know N here, so this is just an estimate based on set bits
        std::size_t set_bits = 0;
        for (auto w : bits_) set_bits += std::popcount(w);
        double p = static_cast<double>(set_bits) / Bits;
        double rate = 1.0;
        for (std::size_t i = 0; i < K; ++i)
            rate *= p;
        return rate;
    }

private:
    std::array<std::size_t, K> hashes(std::string_view key) const {
        std::array<std::size_t, K> result{};
        result[0] = fnv1a(key);
        result[1] = djb2(key);
        for (std::size_t i = 2; i < K; ++i) {
            result[i] = result[i - 1] * 0x9e3779b97f4a7c15ULL
                      + result[i - 2];
            result[i] ^= result[i] >> 47;
        }
        for (auto& h : result)
            h = h % Bits;
        return result;
    }

    static std::size_t fnv1a(std::string_view s) {
        std::size_t h = 14695981039346656037ULL;
        for (auto c : s) {
            h ^= static_cast<std::size_t>(c);
            h *= 1099511628211ULL;
        }
        return h;
    }

    static std::size_t djb2(std::string_view s) {
        std::size_t h = 5381;
        for (auto c : s)
            h = ((h << 5) + h) + static_cast<std::size_t>(c);
        return h;
    }

    std::vector<std::uint64_t> bits_;
};

#endif
