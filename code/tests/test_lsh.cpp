#include "lsh.h"
#include <cassert>
#include <cmath>
#include <print>
#include <vector>

int main() {
    // MinHash
    {
        dsa::minhash mh(128);
        mh.add(1); mh.add(2); mh.add(3); mh.add(4);
        auto sig1 = mh.signature();
        assert(sig1.size() == 128);

        dsa::minhash mh2(128);
        mh2.add(2); mh2.add(3); mh2.add(4); mh2.add(5);
        auto sig2 = mh2.signature();

        double j = dsa::minhash::jaccard(sig1, sig2);
        // True Jaccard = |{2,3,4}| / |{1,2,3,4,5}| = 3/5 = 0.6
        assert(j > 0.3 && j < 0.9);  // approximate
    }

    // LSH cosine
    {
        dsa::lsh_cosine lsh({8, 2});
        std::vector<double> a = {1, 0, 0, 0};
        std::vector<double> b = {0, 1, 0, 0};
        std::vector<double> c = {1, 0, 0, 0};
        lsh.insert(0, a);
        lsh.insert(1, b);
        lsh.insert(2, c);

        // Query with a should find 0 and 2 (similar), not 1
        auto candidates = lsh.query(a);
        assert(!candidates.empty());

        double sim_ab = dsa::lsh_cosine::cosine_similarity(a, b);
        double sim_ac = dsa::lsh_cosine::cosine_similarity(a, c);
        assert(std::abs(sim_ab) < 0.01);  // orthogonal
        assert(std::abs(sim_ac - 1.0) < 0.01);  // identical
    }

    // LSH Jaccard
    {
        dsa::lsh_jaccard lsh({64, 8});
        lsh.insert(0, {1, 2, 3, 4, 5});
        lsh.insert(1, {3, 4, 5, 6, 7});
        lsh.insert(2, {10, 20, 30});

        auto candidates = lsh.query({2, 3, 4, 5, 6});
        assert(!candidates.empty());

        double j01 = lsh.estimate_jaccard(0, 1);
        double j02 = lsh.estimate_jaccard(0, 2);
        assert(j01 > j02);  // sets 0,1 overlap more
    }

    std::println("All LSH tests passed");
    return 0;
}
