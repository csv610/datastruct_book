#include "nfa.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // Literal matching
    {
        dsa::regex_nfa re("abc");
        assert(re.matches("abc"));
        assert(!re.matches("ab"));
        assert(!re.matches("abcd"));
    }

    // Dot (any character)
    {
        dsa::regex_nfa re("a.c");
        assert(re.matches("abc"));
        assert(re.matches("axc"));
        assert(!re.matches("ac"));
    }

    // Kleene star
    {
        dsa::regex_nfa re("ab*c");
        assert(re.matches("ac"));
        assert(re.matches("abc"));
        assert(re.matches("abbbc"));
        assert(!re.matches("ab"));
    }

    // Plus (one or more)
    {
        dsa::regex_nfa re("ab+c");
        assert(!re.matches("ac"));
        assert(re.matches("abc"));
        assert(re.matches("abbbbc"));
    }

    // Question mark (zero or one)
    {
        dsa::regex_nfa re("ab?c");
        assert(re.matches("ac"));
        assert(re.matches("abc"));
        assert(!re.matches("abbc"));
    }

    // Alternation
    {
        dsa::regex_nfa re("a|b");
        assert(re.matches("a"));
        assert(re.matches("b"));
        assert(!re.matches("c"));
    }

    // Combined
    {
        dsa::regex_nfa re("(ab)*c");
        assert(re.matches("c"));
        assert(re.matches("abc"));
        assert(re.matches("ababc"));
        assert(!re.matches("ab"));
    }

    // Empty pattern
    {
        dsa::regex_nfa re("a*");
        assert(re.matches(""));
        assert(re.matches("a"));
        assert(re.matches("aaaa"));
    }

    std::println("All NFA tests passed");
    return 0;
}
