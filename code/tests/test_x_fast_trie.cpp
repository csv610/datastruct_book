#include "x_fast_trie.h"
#include <cassert>
#include <print>

int main() {
    dsa::x_fast_trie<256> trie;
    assert(trie.empty());

    trie.insert(5);
    trie.insert(10);
    trie.insert(3);
    trie.insert(20);
    trie.insert(15);
    assert(trie.size() == 5);

    assert(trie.contains(5));
    assert(trie.contains(20));
    assert(!trie.contains(7));

    assert(trie.successor(5) == 10);
    assert(trie.successor(20) == -1);
    assert(trie.predecessor(10) == 5);
    assert(trie.predecessor(3) == -1);

    trie.erase(10);
    assert(!trie.contains(10));
    assert(trie.successor(5) == 15);

    // Insert more to test depth levels
    for (int i = 30; i < 200; i += 7) {
        trie.insert(i);
    }
    assert(trie.size() > 5);
    assert(trie.successor(199) == -1);
    assert(trie.predecessor(30) == 20);

    std::print("All x_fast_trie tests passed\n");
    return 0;
}
