#include <vector>
#include <unordered_set>
#include <cstdint>
#include <random>
#include <algorithm>
#include <numeric>

class minhash {
public:
    minhash(size_t num_hashes, size_t seed = 42)
        : k_(num_hashes), hashes_(num_hashes)
    {
        // Generate k independent hash function parameters
        std::mt19937_64 rng(seed);
        for (size_t i = 0; i < k_; ++i) {
            uint64_t a = rng(), b = rng();
            hashes_[i] = [a, b](uint64_t x) -> uint64_t {
                // 64-bit multiply-shift hash
                return a * x + b;
            };
        }
    }

    // Compute MinHash signature for a set of elements
    std::vector<uint64_t> compute_signature(
        const std::unordered_set<std::string>& set) const
    {
        std::vector<uint64_t> sig(k_, UINT64_MAX);
        for (const auto& elem : set) {
            uint64_t h = std::hash<std::string>{}(elem);
            for (size_t i = 0; i < k_; ++i) {
                uint64_t val = hashes_[i](h);
                sig[i] = std::min(sig[i], val);
            }
        }
        return sig;
    }

    // Estimate Jaccard similarity from two signatures
    static double estimate_jaccard(
        const std::vector<uint64_t>& sig_a,
        const std::vector<uint64_t>& sig_b)
    {
        size_t matches = 0;
        size_t k = sig_a.size();
        for (size_t i = 0; i < k; ++i)
            if (sig_a[i] == sig_b[i]) ++matches;
        return static_cast<double>(matches) / k;
    }

    size_t num_hashes() const { return k_; }

private:
    size_t k_;
    std::vector<std::function<uint64_t(uint64_t)>> hashes_;
};
