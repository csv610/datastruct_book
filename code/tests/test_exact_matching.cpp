#include "exact_matching.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // Morris-Pratt
    {
        auto r = dsa::morris_pratt_search("ABC ABCDAB ABCDABCDABDE", "ABCDABD");
        assert(r.size() == 1 && r[0] == 15);
    }
    {
        auto r = dsa::morris_pratt_search("aaabaaab", "aaab");
        assert(r.size() == 2);
    }
    {
        auto r = dsa::morris_pratt_search("abcdef", "xyz");
        assert(r.empty());
    }

    // KMP
    {
        auto r = dsa::kmp_search("ABC ABCDAB ABCDABCDABDE", "ABCDABD");
        assert(r.size() == 1 && r[0] == 15);
    }
    {
        auto r = dsa::kmp_search("aaaa", "aa");
        assert(r.size() == 3);
    }
    {
        auto r = dsa::kmp_search("aabaabaabaabaab", "aabaab");
        assert(r.size() == 4);
    }

    // Karp-Rabin
    {
        dsa::karp_rabin kr("abc");
        auto r = kr.search("abcabcabc");
        assert(r.size() == 3 && r[0] == 0 && r[1] == 3 && r[2] == 6);
    }
    {
        dsa::karp_rabin kr("ABCDABD");
        auto r = kr.search("ABC ABCDAB ABCDABCDABDE");
        assert(r.size() == 1 && r[0] == 15);
    }
    {
        dsa::karp_rabin kr("xyz");
        auto r = kr.search("abcdef");
        assert(r.empty());
    }

    // Galil-Seiferas
    {
        auto r = dsa::galil_seiferas_search("ABC ABCDAB ABCDABCDABDE", "ABCDABD");
        assert(r.size() == 1 && r[0] == 15);
    }
    {
        auto r = dsa::galil_seiferas_search("aaaa", "aa");
        assert(r.size() == 3);
    }

    // Boyer-Moore-Horspool
    {
        dsa::boyer_moore_horspool bm("ABCDABD");
        auto r = bm.search("ABC ABCDAB ABCDABCDABDE");
        assert(r.size() == 1 && r[0] == 15);
    }
    {
        dsa::boyer_moore_horspool bm("abc");
        auto r = bm.search("abcabcabc");
        assert(r.size() == 3);
    }
    {
        dsa::boyer_moore_horspool bm("abc");
        auto r = bm.search("abcdef");
        assert(r.size() == 1 && r[0] == 0);
    }

    std::println("All exact matching tests passed");
    return 0;
}
