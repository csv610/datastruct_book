#include "probabilistic.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    // Reservoir sampling
    {
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto result = dsa::reservoir_sample(3, data);
        assert(result.size() == 3);
    }
    {
        std::vector<int> data = {1, 2};
        auto result = dsa::reservoir_sample(5, data);
        assert(result.size() == 2);
    }
    {
        std::vector<int> data = {42};
        auto result = dsa::reservoir_sample(1, data);
        assert(result.size() == 1 && result[0] == 42);
    }

    // HyperLogLog
    {
        dsa::hyperloglog hll(12);
        for (uint64_t i = 0; i < 10000; ++i) {
            uint64_t h = i;
            h ^= h >> 33;
            h *= 0xff51afd7ed558ccdULL;
            h ^= h >> 33;
            h *= 0xc4ceb9fe1a85ec53ULL;
            h ^= h >> 33;
            hll.add(h);
        }
        double est = hll.estimate();
        assert(est > 3000 && est < 30000);  // should be near 10000
    }

    // Count-Min Sketch
    {
        dsa::count_min_sketch cms(256, 5);
        cms.add("apple");
        cms.add("apple");
        cms.add("banana");
        assert(cms.estimate("apple") >= 2);
        assert(cms.estimate("banana") >= 1);
        assert(cms.estimate("cherry") >= 0);  // may be 0 or small FP
    }

    std::cout << "All probabilistic tests passed\n";
    return 0;
}
