#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace dsa {

// ---- Reservoir Sampling (select k items uniformly from stream) ----
template <typename T>
std::vector<T> reservoir_sample(std::size_t k, std::vector<T> stream) {
    if (stream.size() <= k) return stream;
    std::mt19937 rng(42);
    for (std::size_t i = k; i < stream.size(); ++i) {
        std::uniform_int_distribution<std::size_t> dist(0, i);
        std::size_t j = dist(rng);
        if (j < k) stream[j] = stream[i];
    }
    stream.resize(k);
    return stream;
}

// ---- HyperLogLog (cardinality estimation) ----
class hyperloglog {
public:
    explicit hyperloglog(int precision = 10)
        : p_(precision), m_(1 << precision), regs_(m_, 0) {}

    void add(uint64_t hash) {
        int idx = hash >> (64 - p_);
        uint64_t w = hash << p_ | ((1ULL << p_) - 1);
        int zeros = count_leading_zeros(w) + 1;
        regs_[idx] = std::max(regs_[idx], static_cast<uint8_t>(zeros));
    }

    double estimate() const {
        double sum = 0.0;
        int zeros = 0;
        for (int r : regs_) {
            sum += std::pow(2.0, -r);
            if (r == 0) ++zeros;
        }
        double raw = (m_ * m_) / sum;
        if (raw <= 2.5 * m_ && zeros > 0) {
            return static_cast<double>(m_) * std::log(static_cast<double>(m_) / zeros);
        }
        if (raw > (1ULL << 32) / 30.0) {
            return -std::pow(2.0, 32) * std::log(1.0 - raw / std::pow(2.0, 32));
        }
        return raw;
    }

private:
    static int count_leading_zeros(uint64_t x) {
        if (x == 0) return 64;
#ifdef __GNUC__
        return __builtin_clzll(x);
#else
        int n = 0;
        for (int i = 63; i >= 0; --i) {
            if ((x >> i) & 1) break;
            ++n;
        }
        return n;
#endif
    }

    int p_;
    int m_;
    std::vector<uint8_t> regs_;
};

// ---- Count-Min Sketch ----
class count_min_sketch {
public:
    explicit count_min_sketch(int width = 1024, int depth = 5)
        : width_(width), depth_(depth), table_(depth, std::vector<int>(width, 0)),
          seeds_(depth) {
        std::mt19937 rng(42);
        for (int i = 0; i < depth; ++i)
            seeds_[i] = rng();
    }

    void add(const std::string& key, int count = 1) {
        for (int i = 0; i < depth_; ++i) {
            std::size_t h = hash_func(key, seeds_[i]) % width_;
            table_[i][h] += count;
        }
    }

    int estimate(const std::string& key) const {
        int min_val = INT32_MAX;
        for (int i = 0; i < depth_; ++i) {
            std::size_t h = hash_func(key, seeds_[i]) % width_;
            min_val = std::min(min_val, table_[i][h]);
        }
        return min_val;
    }

private:
    static std::size_t hash_func(const std::string& key, uint32_t seed) {
        std::size_t h = seed;
        for (char c : key) {
            h ^= static_cast<std::size_t>(c) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }

    int width_;
    int depth_;
    std::vector<std::vector<int>> table_;
    std::vector<uint32_t> seeds_;
};

}  // namespace dsa
