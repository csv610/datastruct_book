#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <climits>
#include <random>
#include <map>
#include <set>
#include <functional>

// ----------------------------------------------------------------
// Bloom Filter
// ----------------------------------------------------------------
class bloom_filter {
public:
    bloom_filter(size_t expected_elements, double false_positive_rate = 0.01) {
        m_ = static_cast<size_t>(-static_cast<double>(expected_elements)
              * std::log(false_positive_rate)
              / (std::log(2.0) * std::log(2.0)));
        if (m_ < 1) m_ = 1;
        k_ = std::max(1, static_cast<int>(std::round(
             (static_cast<double>(m_) / expected_elements) * std::log(2.0))));
        bits_.resize(m_, false);
    }

    void insert(const std::string& key) {
        auto [h1, h2] = hash(key);
        for (int i = 0; i < k_; ++i) {
            size_t pos = (h1 + static_cast<size_t>(i) * h2) % m_;
            bits_[pos] = true;
        }
    }

    bool contains(const std::string& key) const {
        auto [h1, h2] = hash(key);
        for (int i = 0; i < k_; ++i) {
            size_t pos = (h1 + static_cast<size_t>(i) * h2) % m_;
            if (!bits_[pos]) return false;
        }
        return true;
    }

    size_t bit_count() const { return m_; }
    int hash_count() const { return k_; }

private:
    static std::pair<size_t, size_t> hash(const std::string& key) {
        std::hash<std::string> h;
        size_t hval = h(key);
        return {hval, (hval >> 16) | (hval << 16)};
    }

    size_t m_;
    int k_;
    std::vector<bool> bits_;
};

// ----------------------------------------------------------------
// Count-Min Sketch
// ----------------------------------------------------------------
class count_min_sketch {
public:
    count_min_sketch(double epsilon, double delta) {
        width_ = static_cast<size_t>(std::ceil(std::exp(1.0) / epsilon));
        depth_ = static_cast<size_t>(std::ceil(std::log(1.0 / delta)));
        counts_.resize(depth_, std::vector<size_t>(width_, 0));
    }

    void add(const std::string& key, size_t count = 1) {
        for (size_t i = 0; i < depth_; ++i) {
            size_t pos = hash_i(key, i) % width_;
            counts_[i][pos] += count;
        }
    }

    size_t estimate(const std::string& key) const {
        size_t min_val = SIZE_MAX;
        for (size_t i = 0; i < depth_; ++i) {
            size_t pos = hash_i(key, i) % width_;
            min_val = std::min(min_val, counts_[i][pos]);
        }
        return min_val;
    }

    size_t width() const { return width_; }
    size_t depth() const { return depth_; }

private:
    static size_t hash_i(const std::string& key, size_t i) {
        std::hash<std::string> h;
        return h(key) + i * 0x9e3779b97f4a7c15ULL;
    }

    size_t width_, depth_;
    std::vector<std::vector<size_t>> counts_;
};

// ----------------------------------------------------------------
// Reservoir Sampling
// ----------------------------------------------------------------
std::vector<int> reservoir_sample(const std::vector<int>& stream, size_t k) {
    std::vector<int> reservoir(stream.begin(), stream.begin() + std::min(k, stream.size()));
    std::mt19937 rng(std::random_device{}());
    for (size_t i = k; i < stream.size(); ++i) {
        size_t j = std::uniform_int_distribution<size_t>(0, i)(rng);
        if (j < k) reservoir[j] = stream[i];
    }
    return reservoir;
}

// ----------------------------------------------------------------
// Consistent Hash Ring (with virtual nodes)
// ----------------------------------------------------------------
class consistent_hash_ring {
public:
    void add_server(const std::string& name, int virtual_nodes = 100) {
        std::hash<std::string> h;
        for (int i = 0; i < virtual_nodes; ++i) {
            auto hv = h(name + ":" + std::to_string(i));
            ring_[hv] = name;
        }
    }

    void remove_server(const std::string& name) {
        std::erase_if(ring_, [&](const auto& p) {
            return p.second == name;
        });
    }

    std::string get_server(const std::string& key) const {
        if (ring_.empty()) return "";
        std::hash<std::string> h;
        auto hv = h(key);
        auto it = ring_.lower_bound(hv);
        if (it == ring_.end()) it = ring_.begin();
        return it->second;
    }

    size_t server_count() const {
        std::set<std::string> unique;
        for (const auto& [_, name] : ring_) {
            unique.insert(name);
        }
        return unique.size();
    }

private:
    std::map<uint64_t, std::string> ring_;
};

// ----------------------------------------------------------------
// main — demonstrations
// ----------------------------------------------------------------
int main() {
    std::cout << "=== Bloom Filter ===\n";
    bloom_filter bf(100, 0.01);
    std::cout << "Expected elements: 100, target FPR: 0.01\n";
    std::cout << "Bit array size: " << bf.bit_count() << " bits\n";
    std::cout << "Hash functions (k): " << bf.hash_count() << "\n";

    bf.insert("apple");
    bf.insert("banana");
    bf.insert("cherry");
    std::cout << "Inserted: apple, banana, cherry\n";
    std::cout << "Contains apple? " << (bf.contains("apple") ? "yes" : "no") << "\n";
    std::cout << "Contains grape? " << (bf.contains("grape") ? "yes" : "no") << "\n";

    std::cout << "\n=== Count-Min Sketch ===\n";
    count_min_sketch cms(0.01, 0.01);
    std::cout << "Width: " << cms.width() << ", Depth: " << cms.depth() << "\n";
    cms.add("alice", 42);
    cms.add("bob", 17);
    cms.add("alice", 8);
    std::cout << "Added: alice×50, bob×17\n";
    std::cout << "Estimate for alice: " << cms.estimate("alice") << "\n";
    std::cout << "Estimate for bob: " << cms.estimate("bob") << "\n";
    std::cout << "Estimate for charlie (unseen): " << cms.estimate("charlie") << "\n";

    std::cout << "\n=== Reservoir Sampling ===\n";
    std::vector<int> stream(1000);
    for (int i = 0; i < 1000; ++i) stream[i] = i;
    auto sample = reservoir_sample(stream, 10);
    std::cout << "Stream size: 1000, sample size: 10\n";
    std::cout << "Sample: ";
    for (int x : sample) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "\n=== Consistent Hash Ring ===\n";
    consistent_hash_ring ring;
    ring.add_server("server-a", 50);
    ring.add_server("server-b", 50);
    ring.add_server("server-c", 50);
    std::cout << "Servers: server-a, server-b, server-c (50 vnodes each)\n";

    std::vector<std::string> keys = {"key1", "key2", "key3", "key4", "key5"};
    for (const auto& k : keys) {
        std::cout << "  " << k << " -> " << ring.get_server(k) << "\n";
    }

    std::cout << "After removing server-b:\n";
    ring.remove_server("server-b");
    for (const auto& k : keys) {
        std::cout << "  " << k << " -> " << ring.get_server(k) << "\n";
    }

    return 0;
}
