#include "merkle_tree.h"
#include <cassert>
#include <print>

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};

    dsa::merkle_tree<int> mt(data);
    assert(!mt.empty());
    assert(mt.leaf_count() == 8);

    std::string root = mt.root_hash();
    assert(!root.empty());

    // Verify leaf hashes
    for (int i = 0; i < 8; ++i) {
        assert(!mt.leaf_hash(i).empty());
        assert(mt.verify(i, mt.leaf_hash(i)));
        assert(!mt.verify(i, "wrong_hash"));
    }

    // Proof verification
    auto pf = mt.proof(0);
    assert(!pf.empty());
    assert(dsa::merkle_tree<int>::verify_proof(mt.leaf_hash(0), pf, root));

    // Wrong hash should fail
    assert(!dsa::merkle_tree<int>::verify_proof("wrong", pf, root));

    // Single element
    dsa::merkle_tree<int> mt1({42});
    assert(mt1.leaf_count() == 1);
    assert(mt1.root_hash() == mt1.leaf_hash(0));

    // Odd number of elements
    dsa::merkle_tree<int> mt3({1, 2, 3});
    assert(mt3.leaf_count() == 3);

    // Empty
    dsa::merkle_tree<int> mt0;
    assert(mt0.empty());
    assert(mt0.root_hash() == "");

    std::print("All merkle_tree tests passed\n");
    return 0;
}
