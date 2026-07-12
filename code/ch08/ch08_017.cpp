#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

class CountMinSketch {
private:
    int d;                        // depth (number of hash functions)
    int w;                        // width (number of counters per row)
    std::vector<std::vector<uint64_t>> table;
    std::vector<uint64_t> seeds;

    // Simple hash combining seed: h_i(x) = (a * hash(x) + b) mod p mod w
    uint64_t hash(int i, const std::string& key) const {
        uint64_t h = 14695981039346656037ULL;  // FNV-1a offset basis
        for (char c : key) {
            h ^= static_cast<uint64_t>(c);
            h *= 1099511628211ULL;             // FNV-1a prime
        }
        h ^= seeds[i];
        h *= 1099511628211ULL;
        return h % w;
    }

public:
    CountMinSketch(int depth, int width)
        : d(depth), w(width),
          table(depth, std::vector<uint64_t>(width, 0)),
          seeds(depth) {
        // Initialize each seed to a distinct odd value
        for (int i = 0; i < d; i++) {
            seeds[i] = static_cast<uint64_t>(i) * 6364136223846793005ULL + 1;
        }
    }

    // Add count c for the given key
    void update(const std::string& key, uint64_t c = 1) {
        for (int i = 0; i < d; i++) {
            uint64_t col = hash(i, key);
            table[i][col] += c;
        }
    }

    // Estimate the frequency of the given key
    uint64_t query(const std::string& key) const {
        uint64_t est = UINT64_MAX;
        for (int i = 0; i < d; i++) {
            uint64_t col = hash(i, key);
            est = std::min(est, table[i][col]);
        }
        return est;
    }

    // Merge two sketches (additive)
    void merge(const CountMinSketch& other) {
        for (int i = 0; i < d; i++)
            for (int j = 0; j < w; j++)
                table[i][j] += other.table[i][j];
    }
};

// Usage example
int main() {
    // epsilon = 0.001, delta = 0.001 => w = 2719, d = 7
    CountMinSketch cms(7, 2719);

    cms.update("hello");
    cms.update("world");
    cms.update("hello");
    cms.update("hello");

    std::cout << "hello: " << cms.query("hello") << "\n";  // 3
    std::cout << "world: " << cms.query("world") << "\n";  // 1
    std::cout << "absent: " << cms.query("missing") << "\n";  // 0
}
