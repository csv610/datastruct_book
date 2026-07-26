#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dsa {

// ---- MinHash for set similarity ----
class minhash {
public:
    explicit minhash(int num_hashes = 128, uint64_t seed = 42)
        : num_hashes_(num_hashes), hashes_(num_hashes, UINT64_MAX) {
        std::mt19937_64 rng(seed);
        a_.resize(num_hashes);
        b_.resize(num_hashes);
        for (int i = 0; i < num_hashes; ++i) {
            a_[i] = rng();
            b_[i] = rng();
        }
    }

    void add(uint64_t value) {
        for (int i = 0; i < num_hashes_; ++i) {
            uint64_t h = a_[i] * value + b_[i];
            hashes_[i] = std::min(hashes_[i], h);
        }
    }

    void add_set(const std::vector<uint64_t>& elements) {
        for (auto e : elements) add(e);
    }

    std::vector<uint64_t> signature() const { return hashes_; }

    static double jaccard(const std::vector<uint64_t>& a,
                          const std::vector<uint64_t>& b) {
        std::size_t matches = 0;
        for (std::size_t i = 0; i < a.size(); ++i)
            if (a[i] == b[i]) ++matches;
        return static_cast<double>(matches) / static_cast<double>(a.size());
    }

private:
    int num_hashes_;
    std::vector<uint64_t> hashes_;
    std::vector<uint64_t> a_, b_;
};

// ---- LSH Cosine Similarity ----
struct lsh_cosine_config {
    int num_planes = 64;
    int num_bands = 16;
};

class lsh_cosine {
public:
    explicit lsh_cosine(const lsh_cosine_config& cfg = {},
                        uint64_t seed = 42)
        : cfg_(cfg), rng_(seed), dist_(-1.0, 1.0) {
        for (int b = 0; b < cfg_.num_bands; ++b) {
            std::vector<std::vector<double>> band_planes;
            for (int p = 0; p < cfg_.num_planes; ++p) {
                std::vector<double> plane;
                for (int d = 0; d < max_dim_; ++d)
                    plane.push_back(dist_(rng_));
                band_planes.push_back(std::move(plane));
            }
            planes_.push_back(std::move(band_planes));
        }
    }

    std::vector<uint64_t> hash(const std::vector<double>& vec) const {
        std::vector<uint64_t> buckets;
        buckets.reserve(cfg_.num_bands);
        for (int b = 0; b < cfg_.num_bands; ++b) {
            uint64_t bucket = 0;
            for (int p = 0; p < cfg_.num_planes; ++p) {
                double dot = 0;
                std::size_t dim = std::min(vec.size(),
                                           static_cast<std::size_t>(max_dim_));
                for (std::size_t d = 0; d < dim; ++d)
                    dot += vec[d] * planes_[b][p][d];
                if (dot >= 0) bucket |= (1ULL << p);
            }
            buckets.push_back(bucket);
        }
        return buckets;
    }

    void insert(int id, const std::vector<double>& vec) {
        auto buckets = hash(vec);
        for (int b = 0; b < cfg_.num_bands; ++b)
            index_[b][buckets[b]].push_back(id);
        vecs_[id] = vec;
    }

    std::vector<int> query(const std::vector<double>& vec) const {
        auto buckets = hash(vec);
        std::vector<int> candidates;
        for (int b = 0; b < cfg_.num_bands; ++b) {
            auto band_it = index_.find(b);
            if (band_it == index_.end()) continue;
            auto it = band_it->second.find(buckets[b]);
            if (it != band_it->second.end()) {
                for (int id : it->second)
                    candidates.push_back(id);
            }
        }
        std::sort(candidates.begin(), candidates.end());
        candidates.erase(std::unique(candidates.begin(), candidates.end()),
                         candidates.end());
        return candidates;
    }

    static double cosine_similarity(const std::vector<double>& a,
                                    const std::vector<double>& b) {
        double dot = 0, norm_a = 0, norm_b = 0;
        std::size_t n = std::min(a.size(), b.size());
        for (std::size_t i = 0; i < n; ++i) {
            dot += a[i] * b[i];
            norm_a += a[i] * a[i];
            norm_b += b[i] * b[i];
        }
        if (norm_a == 0 || norm_b == 0) return 0;
        return dot / (std::sqrt(norm_a) * std::sqrt(norm_b));
    }

private:
    static constexpr int max_dim_ = 256;
    lsh_cosine_config cfg_;
    std::mt19937_64 rng_;
    std::uniform_real_distribution<double> dist_;
    std::vector<std::vector<std::vector<double>>> planes_;
    std::unordered_map<int, std::vector<double>> vecs_;
    std::unordered_map<int, std::unordered_map<uint64_t, std::vector<int>>> index_;
};

// ---- LSH Jaccard Similarity ----
// Uses a single shared set of minhash functions for all inserts and queries
struct lsh_jaccard_config {
    int num_hashes = 128;
    int num_bands = 16;
};

class lsh_jaccard {
public:
    explicit lsh_jaccard(const lsh_jaccard_config& cfg = {},
                         uint64_t seed = 42)  // NOLINT
        : cfg_(cfg), rng_(seed), a_(cfg.num_hashes), b_(cfg.num_hashes) {
        rows_per_band_ = cfg.num_hashes / cfg.num_bands;
        for (int i = 0; i < cfg.num_hashes; ++i) {
            a_[i] = rng_();
            b_[i] = rng_();
        }
    }

    void insert(int id, const std::vector<uint64_t>& elements) {
        auto sig = compute_signature(elements);
        for (int b = 0; b < cfg_.num_bands; ++b) {
            uint64_t bucket = 0;
            for (int r = 0; r < rows_per_band_; ++r) {
                int idx = b * rows_per_band_ + r;
                bucket ^= sig[idx] * static_cast<uint64_t>(r + 1);
            }
            index_[b][bucket].push_back(id);
        }
        signatures_[id] = sig;
    }

    std::vector<int> query(const std::vector<uint64_t>& elements) const {
        auto sig = compute_signature(elements);
        std::vector<int> candidates;
        for (int b = 0; b < cfg_.num_bands; ++b) {
            uint64_t bucket = 0;
            for (int r = 0; r < rows_per_band_; ++r) {
                int idx = b * rows_per_band_ + r;
                bucket ^= sig[idx] * static_cast<uint64_t>(r + 1);
            }
            auto band_it = index_.find(b);
            if (band_it == index_.end()) continue;
            auto it = band_it->second.find(bucket);
            if (it != band_it->second.end())
                for (int id : it->second)
                    candidates.push_back(id);
        }
        std::sort(candidates.begin(), candidates.end());
        candidates.erase(std::unique(candidates.begin(), candidates.end()),
                         candidates.end());
        return candidates;
    }

    double estimate_jaccard(int id_a, int id_b) const {
        return minhash::jaccard(signatures_.at(id_a), signatures_.at(id_b));
    }

private:
    std::vector<uint64_t> compute_signature(const std::vector<uint64_t>& elements) const {
        std::vector<uint64_t> sig(cfg_.num_hashes, UINT64_MAX);
        for (uint64_t val : elements) {
            for (int i = 0; i < cfg_.num_hashes; ++i) {
                uint64_t h = a_[i] * val + b_[i];
                sig[i] = std::min(sig[i], h);
            }
        }
        return sig;
    }

    lsh_jaccard_config cfg_;
    int rows_per_band_;
    std::mt19937_64 rng_;
    std::vector<uint64_t> a_, b_;
    std::unordered_map<int, std::vector<uint64_t>> signatures_;
    std::unordered_map<int, std::unordered_map<uint64_t, std::vector<int>>> index_;
};

}  // namespace dsa
