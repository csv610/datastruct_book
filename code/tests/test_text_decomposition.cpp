#include "text_decomposition.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // Maximum suffix
    {
        auto [pos, len] = dsa::maximum_suffix("mississippi");
        assert(len == 9);  // "ssissippi" is the max suffix
    }
    {
        auto [pos, len] = dsa::maximum_suffix("banana");
        assert(len == 4);  // "nana" is the max suffix
    }
    {
        auto [pos, len] = dsa::maximum_suffix("abc");
        assert(len == 1);
    }

    // Lyndon factorization
    {
        auto factors = dsa::lyndon_factorization("mississippi");
        // "mississippi" = "m" + "ississ" + "i" + "pp" + "i"
        // Duval: "mississippi" → "m" "ississ" "i" "pp" "i"
        assert(!factors.empty());
        // Verify: concatenated factors should equal original
        std::string rebuilt;
        for (auto& f : factors)
            rebuilt += std::string("mississippi").substr(f.start, f.length);
        assert(rebuilt == "mississippi");
    }
    {
        auto factors = dsa::lyndon_factorization("aaaa");
        // "aaaa" = "a" + "a" + "a" + "a"
        assert(factors.size() == 4);
    }
    {
        auto factors = dsa::lyndon_factorization("abacaba");
        assert(!factors.empty());
        std::string rebuilt;
        for (auto& f : factors)
            rebuilt += std::string("abacaba").substr(f.start, f.length);
        assert(rebuilt == "abacaba");
    }

    // Lyndon word check
    {
        assert(dsa::is_lyndon_word("a"));
        assert(dsa::is_lyndon_word("ab"));
        assert(dsa::is_lyndon_word("aab"));
        assert(!dsa::is_lyndon_word("ba"));
        assert(!dsa::is_lyndon_word("aa"));
    }

    // Maximum suffix ascending/descending
    {
        auto p = dsa::maximum_suffix_ascending("banana");
        assert(p < 6);
    }
    {
        auto p = dsa::maximum_suffix_descending("banana");
        assert(p < 6);
    }

    std::println("All text decomposition tests passed");
    return 0;
}
