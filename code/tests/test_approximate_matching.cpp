#include "approximate_matching.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // Hamming distance
    {
        assert(dsa::hamming_distance("karolin", "kathrin") == 3);
        assert(dsa::hamming_distance("abc", "abc") == 0);
        assert(dsa::hamming_distance("abc", "xyz") == 3);
    }

    // Levenshtein
    {
        assert(dsa::levenshtein("kitten", "sitting") == 3);
        assert(dsa::levenshtein("abc", "abc") == 0);
        assert(dsa::levenshtein("", "abc") == 3);
    }

    // Myers' bit-parallel (Hamming, within k mismatches)
    {
        auto r = dsa::myers_approximate("abcabc", "abc", 0);
        assert(r.size() == 2);
        assert(r[0] == 0 && r[1] == 3);
    }
    {
        auto r = dsa::myers_approximate("abcdef", "axc", 1);
        assert(r.size() == 1 && r[0] == 0);
    }
    {
        auto r = dsa::myers_approximate("aaaa", "aa", 0);
        assert(r.size() == 3);
    }

    // Shift-Or approximate
    {
        auto r = dsa::shift_or_approximate("abcabc", "abc", 0);
        assert(r.size() == 2);
    }
    {
        auto r = dsa::shift_or_approximate("abcdef", "axc", 1);
        assert(r.size() == 1 && r[0] == 0);
    }

    // Approximate search (edit distance based)
    {
        auto r = dsa::approximate_search("abcd", "axc", 2);
        assert(!r.empty());
        assert(r[0].first == 0);
    }
    {
        auto r = dsa::approximate_search("hello", "xyz", 2);
        assert(r.empty());
    }

    std::println("All approximate matching tests passed");
    return 0;
}
