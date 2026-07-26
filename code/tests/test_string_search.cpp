#include "string_search.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // Z-algorithm
    {
        auto z = dsa::z_function("aabxaabxcaab");
        assert(z[0] == 12);
        assert(z[1] == 1);
        assert(z[4] == 4);
        assert(z[8] == 0);
    }
    {
        auto matches = dsa::z_search("aaabaaab", "aaab");
        assert(matches.size() == 2 && matches[0] == 0 && matches[1] == 4);
    }
    {
        auto matches = dsa::z_search("abcabcabc", "abc");
        assert(matches.size() == 3);
    }

    // Boyer-Moore
    {
        dsa::boyer_moore bm("ABCDABD");
        auto matches = bm.search("ABC ABCDAB ABCDABCDABDE");
        assert(matches.size() == 1 && matches[0] == 15);
    }
    {
        dsa::boyer_moore bm("aab");
        auto matches = bm.search("aaabaaab");
        assert(matches.size() == 2);
    }
    {
        dsa::boyer_moore bm("abc");
        auto matches = bm.search("abcdef");
        assert(matches.size() == 1 && matches[0] == 0);
    }

    // LCP
    {
        assert(dsa::lcp("hello", "help") == 3);
        assert(dsa::lcp("abc", "abc") == 3);
        assert(dsa::lcp("abc", "xyz") == 0);
    }

    // Longest palindrome (Manacher)
    {
        assert(dsa::longest_palindrome("babad") == "bab");
        assert(dsa::longest_palindrome("cbbd") == "bb");
        assert(dsa::longest_palindrome("a") == "a");
        assert(dsa::longest_palindrome("racecar") == "racecar");
        assert(dsa::longest_palindrome("abacababad") == "abacaba");
    }

    std::println("All string search tests passed");
    return 0;
}
