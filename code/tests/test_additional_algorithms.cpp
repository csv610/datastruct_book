#include "additional_algorithms.h"
#include <cassert>
#include <print>
#include <string>
#include <vector>

int main() {
    // Landau-Vishkin (k-mismatches)
    {
        auto r = dsa::landau_vishkin("abcabc", "abc", 0);
        assert(r.size() == 2 && r[0] == 0 && r[1] == 3);
    }
    {
        auto r = dsa::landau_vishkin("abcdef", "axc", 1);
        assert(r.size() == 1 && r[0] == 0);
    }
    {
        auto r = dsa::landau_vishkin("aaaa", "aa", 0);
        assert(r.size() == 3);
    }

    // Hirschberg LCS length
    {
        assert(dsa::hirschberg_lcs_length("ABCBDAB", "BDCABA") == 4);
        assert(dsa::hirschberg_lcs_length("abc", "abc") == 3);
        assert(dsa::hirschberg_lcs_length("abc", "xyz") == 0);
        assert(dsa::hirschberg_lcs_length("", "abc") == 0);
    }

    // Hirschberg LCS string
    {
        auto lcs = dsa::hirschberg_lcs("ABCBDAB", "BDCABA");
        assert(lcs.size() == 4);
    }
    {
        auto lcs = dsa::hirschberg_lcs("abc", "abc");
        assert(lcs == "abc");
    }
    {
        auto lcs = dsa::hirschberg_lcs("abc", "xyz");
        assert(lcs.empty());
    }

    // Shortest Common Superstring (greedy)
    {
        auto scs = dsa::shortest_common_superstring({"abc", "bcd", "cde"});
        assert(scs.size() <= 7);  // "abcde" = 5, but greedy might give longer
        assert(scs.size() >= 5);
    }
    {
        auto scs = dsa::shortest_common_superstring({"ab", "ba"});
        assert(scs.size() <= 3);
    }

    // LCS string
    {
        auto lcs = dsa::lcs_string("ABCBDAB", "BDCABA");
        assert(lcs.size() == 4);
    }
    {
        auto lcs = dsa::lcs_string("abc", "abc");
        assert(lcs == "abc");
    }
    {
        auto lcs = dsa::lcs_string("abc", "def");
        assert(lcs.empty());
    }

    std::println("All additional algorithm tests passed");
    return 0;
}
