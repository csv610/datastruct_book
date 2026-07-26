#include "multiple_matching.h"
#include <cassert>
#include <print>
#include <string>
#include <vector>

int main() {
    // Aho-Corasick
    {
        std::vector<std::string_view> patterns = {"he", "she", "his", "hers"};
        dsa::aho_corasick ac(patterns);
        auto r = ac.search("ahishers");
        assert(r.size() == 4);
        // "his" at 1, "she" at 3, "he" at 4, "hers" at 4
    }
    {
        std::vector<std::string_view> patterns = {"abc", "bcd", "cde"};
        dsa::aho_corasick ac(patterns);
        auto r = ac.search("abcde");
        assert(r.size() == 3);  // abc@0, bcd@1, cde@2
    }
    {
        std::vector<std::string_view> patterns = {"a", "ab", "abc"};
        dsa::aho_corasick ac(patterns);
        auto r = ac.search("abc");
        assert(r.size() == 3);  // a@0, ab@0, abc@0
    }
    {
        std::vector<std::string_view> patterns = {"xyz"};
        dsa::aho_corasick ac(patterns);
        auto r = ac.search("abc");
        assert(r.empty());
    }

    // Commentz-Walter
    {
        std::vector<std::string_view> patterns = {"he", "she", "his", "hers"};
        dsa::commentz_walter cw(patterns);
        auto r = cw.search("ahishers");
        assert(!r.empty());
    }
    {
        std::vector<std::string_view> patterns = {"abc", "bcd"};
        dsa::commentz_walter cw(patterns);
        auto r = cw.search("abcde");
        assert(r.size() >= 2);
    }

    std::println("All multiple matching tests passed");
    return 0;
}
