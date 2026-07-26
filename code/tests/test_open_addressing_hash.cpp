#include "open_addressing_hash.h"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    // Open addressing (linear probing)
    {
        dsa::open_addressing_hash<std::string, int> h(8);
        assert(h.empty());
        assert(h.insert("apple", 1));
        assert(h.insert("banana", 2));
        assert(h.insert("cherry", 3));
        assert(h.size() == 3);
        assert(h.contains("apple"));
        assert(!h.contains("grape"));

        auto* v = h.find("banana");
        assert(v && *v == 2);

        *h.find("banana") = 99;
        assert(*h.find("banana") == 99);

        assert(h.erase("apple"));
        assert(!h.contains("apple"));
        assert(h.size() == 2);

        // Duplicate insert updates
        h.insert("cherry", 42);
        assert(*h.find("cherry") == 42);
        assert(h.size() == 2);
    }

    // Quadratic probing
    {
        dsa::quadratic_probing_hash<int, std::string> h(16);
        h.insert(10, "ten");
        h.insert(20, "twenty");
        h.insert(30, "thirty");
        assert(h.size() == 3);
        auto* v = h.find(20);
        assert(v && *v == "twenty");
        assert(!h.find(99));
    }

    // Double hashing
    {
        dsa::double_hashing_hash<std::string, int> h(16);
        h.insert("one", 1);
        h.insert("two", 2);
        h.insert("three", 3);
        assert(h.size() == 3);
        assert(*h.find("one") == 1);
        assert(!h.find("four"));
    }

    // Stress test: insert many
    {
        dsa::open_addressing_hash<int, int> h(16);
        for (int i = 0; i < 1000; ++i)
            h.insert(i, i * 10);
        assert(h.size() == 1000);
        for (int i = 0; i < 1000; ++i) {
            auto* v = h.find(i);
            assert(v && *v == i * 10);
        }
        for (int i = 0; i < 500; ++i)
            h.erase(i);
        assert(h.size() == 500);
    }

    std::cout << "All open addressing hash tests passed\n";
    return 0;
}
