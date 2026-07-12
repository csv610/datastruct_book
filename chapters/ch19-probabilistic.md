# Probabilistic Data Structures

## Introduction

Not all problems require exact answers. When processing massive data streams, probabilistic data structures provide memory-efficient approximate answers with bounded error.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Bloom Filters

A **Bloom filter** tests set membership with no false negatives and a controllable false positive rate. It uses k hash functions and a bit array of size m.

### Operations

```cpp
class bloom_filter {
public:
    bloom_filter(size_t expected_elements, double false_positive_rate = 0.01) {
        // Optimal size and hash count
        m_ = -expected_elements * std::log(false_positive_rate) 
             / (std::log(2) * std::log(2));
        k_ = std::max(1, static_cast<int>(std::round(
             (m_ / expected_elements) * std::log(2))));
        bits_.resize(m_, false);
    }

    void insert(std::string_view key) {
        auto [h1, h2] = hash(key);
        for (int i = 0; i < k_; ++i) {
            size_t pos = (h1 + i * h2) % m_;
            bits_[pos] = true;
        }
    }

    bool contains(std::string_view key) const {
        auto [h1, h2] = hash(key);
        for (int i = 0; i < k_; ++i) {
            size_t pos = (h1 + i * h2) % m_;
            if (!bits_[pos]) return false;  // definitely not present
        }
        return true;  // probably present
    }

private:
    // Double hashing
    std::pair<size_t, size_t> hash(std::string_view key) const {
        std::hash<std::string_view> h;
        size_t hval = h(key);
        return {hval, hval >> 16 | (hval << 16)};
    }

    size_t m_;  // bits in filter
    int k_;     // number of hash functions
    std::vector<bool> bits_;
};
```

**Properties:**
- **No false negatives:** if contains() returns false, the key was definitely not inserted
- **False positives:** contains() may return true for a key that was not inserted
- **Cannot delete** (without extensions like counting Bloom filters)

**Optimal parameters:** m = -n ln P / (ln 2)², k = (m/n) ln 2, where n = expected elements, P = target false positive rate.

**Application:** Cache filtering, spell checkers, database join optimization, cryptocurrency SPV nodes.

## Count-Min Sketch

Count approximate frequencies of items in a stream using a 2D array of counters and hash functions.

```cpp
class count_min_sketch {
public:
    count_min_sketch(double epsilon, double delta) {
        // width = ceil(e / epsilon), depth = ceil(ln(1 / delta))
        width_ = static_cast<size_t>(std::ceil(std::exp(1) / epsilon));
        depth_ = static_cast<size_t>(std::ceil(std::log(1.0 / delta)));
        counts_.resize(depth_, std::vector<size_t>(width_, 0));
    }

    void add(std::string_view key, size_t count = 1) {
        for (size_t i = 0; i < depth_; ++i) {
            size_t pos = hash_i(key, i) % width_;
            counts_[i][pos] += count;
        }
    }

    size_t estimate(std::string_view key) const {
        size_t min_val = SIZE_MAX;
        for (size_t i = 0; i < depth_; ++i) {
            size_t pos = hash_i(key, i) % width_;
            min_val = std::min(min_val, counts_[i][pos]);
        }
        return min_val;
    }

private:
    size_t hash_i(std::string_view key, size_t i) const {
        return std::hash<std::string_view>{}(key) + i * 0x9e3779b97f4a7c15ULL;
    }

    size_t width_, depth_;
    std::vector<std::vector<size_t>> counts_;
};
```

## HyperLogLog

Estimate the number of distinct elements in a large stream using O(log log n) space.

```cpp
class hyperloglog {
public:
    hyperloglog(int precision = 14) : precision_(precision) {
        m_ = 1 << precision;  // number of registers
        registers_.resize(m_, 0);
    }

    void add(std::string_view key) {
        auto hash = std::hash<std::string_view>{}(key);
        size_t idx = hash >> (64 - precision_);  // first p bits
        uint8_t zeros = count_leading_zeros(hash << precision_);
        registers_[idx] = std::max(registers_[idx], zeros + 1);
    }

    double estimate() const {
        double sum = 0.0;
        size_t zeros = 0;
        for (uint8_t r : registers_) {
            sum += 1.0 / (1 << r);
            if (r == 0) ++zeros;
        }
        double e = alpha_m_ * m_ * m_ / sum;

        // Small range correction
        if (e <= 2.5 * m_) {
            if (zeros > 0) e = m_ * std::log(static_cast<double>(m_) / zeros);
        }
        return e;
    }

private:
    static uint8_t count_leading_zeros(size_t x) {
        return __builtin_clzll(x);
    }

    int precision_;
    size_t m_;
    std::vector<uint8_t> registers_;
    static constexpr double alpha_m_ = 0.7213 / (1 + 1.079 / (1 << 14));
};
```

## Reservoir Sampling

Select k random elements from a stream of unknown length.

```cpp
std::vector<int> reservoir_sample(std::span<const int> stream, size_t k) {
    std::vector<int> reservoir(stream.begin(), stream.begin() + k);
    std::mt19937 rng(std::random_device{}());

    for (size_t i = k; i < stream.size(); ++i) {
        size_t j = std::uniform_int_distribution<size_t>(0, i)(rng);
        if (j < k) reservoir[j] = stream[i];
    }
    return reservoir;
}
```

Each element has probability k/n of being in the final sample.

## Consistent Hashing

Distribute keys across a changing set of servers while minimizing remapping when servers join or leave. Each server is mapped to points on a hash ring, and each key is assigned to the nearest clockwise server.

```cpp
class consistent_hash_ring {
public:
    void add_server(std::string_view name, int virtual_nodes = 100) {
        for (int i = 0; i < virtual_nodes; ++i) {
            auto h = hash(std::string(name) + ":" + std::to_string(i));
            ring_[h] = name;
        }
    }

    void remove_server(std::string_view name) {
        std::erase_if(ring_, [&](const auto& p) {
            return p.second == name;
        });
    }

    std::string_view get_server(std::string_view key) const {
        if (ring_.empty()) return "";
        auto it = ring_.lower_bound(hash(key));
        if (it == ring_.end()) it = ring_.begin();
        return it->second;
    }

private:
    static uint64_t hash(std::string_view s) {
        return std::hash<std::string_view>{}(s);
    }

    std::map<uint64_t, std::string> ring_;
};
```

When a server is removed, only its keys migrate to the next server; every other key stays put. Virtual nodes improve load balance by spreading each server across many ring positions.

## Locality-Sensitive Hashing

Approximate nearest-neighbor search in high dimensions by hashing inputs so that similar items collide with high probability.

The basic LSH family for cosine similarity (simhash):
- Generate k random hyperplanes
- For each item, compute the sign of each hyperplane dot product — the k-bit signature captures the angular neighborhood

```cpp
class simhash {
public:
    simhash(size_t bits, size_t dim)
        : bits_(bits), planes_(bits, std::vector<double>(dim)) {
        std::mt19937 rng(std::random_device{}());
        std::normal_distribution<double> dist(0.0, 1.0);
        for (auto& v : planes_)
            for (auto& x : v) x = dist(rng);
    }

    uint64_t signature(std::span<const double> vec) const {
        uint64_t sig = 0;
        for (size_t i = 0; i < bits_; ++i) {
            double dot = 0.0;
            for (size_t j = 0; j < vec.size(); ++j)
                dot += planes_[i][j] * vec[j];
            if (dot > 0) sig |= (1ULL << i);
        }
        return sig;
    }

    // Hamming distance on signatures approximates cosine distance
    static double cosine_similarity(uint64_t a, uint64_t b) {
        return 1.0 - static_cast<double>(std::popcount(a ^ b)) / 64.0;
    }

private:
    size_t bits_;
    std::vector<std::vector<double>> planes_;
};
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Bloom filter false positive rate exceeds requirements | Too many false positives, wasted queries | Calculate optimal k and m from n and desired P; verify with test data |
| Using Bloom filter when deletions are needed | Standard Bloom filter doesn't support deletion | Use counting Bloom filter or switch to a Cuckoo filter |
| Forgetting Consistent Hashing's virtual nodes | Skewed distribution for non-uniform servers | Add 100–200 virtual nodes per physical server |
| Hash collision in MinHash / Simhash | Wrong similarity estimate | Use multiple hash functions (k = 100+ for MinHash) |
| HyperLogLog with too few registers (m too small) | High estimation error | Use m = 2¹² (4096 registers) for ~1.6% error |
| Using reservoir sampling without shuffling first n elements | First k elements overrepresented, bias | Implement Fisher-Yates for the first k positions |
| LSH hash table with too few bands | Poor recall (many near neighbors missed) | Tune band count b and row count r for desired (recall, precision) |

## Summary

| Structure | Problem | Space | Error | Supports Deletion |
|---|---|---|---|---|
| Bloom filter | Membership | O(m) bits | False positive rate P | With counting variant |
| Count-Min Sketch | Frequency | O(dw) counters | Additive ε·n | No |
| HyperLogLog | Cardinality | O(m log log n) | 1.04/√m relative | No |
| Reservoir Sampling | Random sample | O(k) | Uniform — exact | No |
| Consistent Hashing | Key distribution | O(nV) mapping | Near-perfect balance | Yes |
| LSH / Simhash | Approx NN | O(k) per item | Depends on k | — |

These trade-offs are essential for streaming data, big data analytics, distributed systems, and ML pipelines.

## Exercises

### Drill

1. For a Bloom filter with m = 100 bits, k = 3 hash functions, and n = 10 elements inserted, what is the expected false positive rate? Compute using the formula (1 - e^{-kn/m})^k.

2. How many bits are needed for a Bloom filter to achieve a false positive rate of 1% with 10⁶ elements? What if we need 0.1%?

3. For Count-Min Sketch, if we want to guarantee that all frequency estimates have error at most 0.1% with probability 99%, how many rows and columns do we need?

4. For HyperLogLog with 14-bit precision (m = 16384 registers), what is the standard error? If we halve the precision (use 13 bits), how does the error change?

5. Explain why Bloom filters have false positives but no false negatives. Give a concrete example of a false positive.

### Application

6. Implement a **counting Bloom filter** that supports deletion by replacing each bit with a small counter (typically 4 bits). Compare space with the standard Bloom filter.

7. Implement **Bloom filter union and intersection** — given two Bloom filters with the same parameters, compute the union (OR) and intersection (AND). Verify that the union correctly represents the union of the two sets.

8. Compare Count-Min Sketch vs `std::unordered_map` for frequency estimation on a stream of 10⁶ items with 10⁵ distinct keys. Measure the error in the sketch's estimates and the memory used.

9. Use HyperLogLog to estimate the number of distinct IP addresses in a network log with 10⁷ entries. Compare accuracy at precision values p = 10, 12, 14, 16.

10. Implement **reservoir sampling** for k = 100 from a stream of unknown length. Verify that each element has probability k/n of being in the sample through simulation.

11. Implement a **heavy hitters** algorithm (lossy counting or space-saving) that finds all elements with frequency > φ·n in a stream. Compare with Count-Min Sketch.

12. Implement a **Bloom filter** for a web crawler to avoid revisiting URLs. How does the false positive rate affect the number of missed pages?

13. Implement **consistent hashing** for a distributed cache with 5 servers and 100 keys. Show the distribution of keys when a server joins and when one leaves. Use 50 virtual nodes per server.

14. Use **simhash** to find near-duplicate documents in a collection of 100 text snippets. Compute the cosine similarity from signatures and compare with exact cosine similarity of TF-IDF vectors.

### Research

15. **(MinHash)** Read about MinHash for estimating set similarity (Jaccard coefficient). Implement MinHash with k hash functions and compare with exact Jaccard computation for random sets.

16. **(Cuckoo filter)** Research the Cuckoo filter (Fan et al., 2014) — a Bloom filter alternative that supports deletion. Implement it and compare its space efficiency with the counting Bloom filter.

17. **(HyperLogLog++)** Read about Google's HyperLogLog++ improvements (Heule et al., 2013) including bias correction and sparse representation. Implement the sparse representation for small cardinalities.

18. **(Count-Min Sketch variants)** Research the Conservative Update variant of Count-Min Sketch. Implement it and measure the improvement in accuracy.

19. **(DDSketch)** Research the DDSketch algorithm for streaming quantiles. Implement it and compare accuracy with the Greenwald-Khanna algorithm on skewed distributions.

20. **(Rendezvous hashing)** Implement HRW (highest random weight) hashing and compare load distribution with consistent hashing. Which achieves better balance with fewer virtual nodes?

## References and Further Reading

- Burton H. Bloom, "Space/Time Trade-offs in Hash Coding with Allowable Errors" — CACM, 1970.
- Graham Cormode and S. Muthukrishnan, "An Improved Data Stream Summary: The Count-Min Sketch and Its Applications" — Journal of Algorithms, 2005.
- Philippe Flajolet, Éric Fusy, Olivier Gandouet, and Frédéric Meunier, "HyperLogLog: the Analysis of a Near-Optimal Cardinality Estimation Algorithm" — AOFA, 2007.
- Jeffrey S. Vitter, "Random Sampling with a Reservoir" — ACM TOMS, 1985.
- Andrei Z. Broder, "On the Resemblance and Containment of Documents" — Compression and Complexity of Sequences, 1997.
- Bin Fan, Dave G. Andersen, Michael Kaminsky, and Michael D. Mitzenmacher, "Cuckoo Filter: Practically Better Than Bloom" — ACM CoNEXT, 2014.
- Stefan Heule, Marc Nunkesser, and Alexander Hall, "HyperLogLog in Practice: Algorithmic Engineering of a State of the Art Cardinality Estimation Algorithm" — EDBT, 2013.
- Jayadev Misra and David Gries, "Finding Repeated Elements" — Science of Computer Programming, 1982.
- David Karger et al., "Consistent Hashing and Random Trees: Distributed Caching Protocols for Relieving Hot Spots on the World Wide Web" — STOC, 1997.
- Piotr Indyk and Rajeev Motwani, "Approximate Nearest Neighbors: Towards Removing the Curse of Dimensionality" — STOC, 1998.
- Moses S. Charikar, "Similarity Estimation Techniques from Rounding Algorithms" — STOC, 2002.
- Ted Dunning, "The T-Digest: Efficient Estimates of Distributions" — 2013.
