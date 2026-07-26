#include "suffix_array_search.h"
#include <cassert>
#include <print>
#include <string>
#include <vector>

// Simple O(n^2) SA construction for tests (no sentinel)
static std::vector<std::size_t> build_sa(std::string_view s) {
    std::size_t n = s.size();
    std::vector<std::size_t> sa(n);
    for (std::size_t i = 0; i < n; ++i) sa[i] = i;
    std::sort(sa.begin(), sa.end(), [&s](std::size_t a, std::size_t b) {
        return s.compare(a, std::string_view::npos, s, b, std::string_view::npos) < 0;
    });
    return sa;
}

int main() {
    // Kasai LCP
    {
        std::string text = "banana";
        auto sa = build_sa(text);
        auto lcp = dsa::kasai_lcp(text, sa);
        assert(lcp.size() == text.size());
        assert(lcp[0] == 0);
        for (std::size_t i = 1; i < sa.size(); ++i) {
            std::size_t j1 = sa[i - 1], j2 = sa[i];
            std::size_t expected = 0;
            while (j1 + expected < text.size() && j2 + expected < text.size() &&
                   text[j1 + expected] == text[j2 + expected])
                ++expected;
            assert(lcp[i] == expected);
        }
    }
    {
        std::string text = "abracadabra";
        auto sa = build_sa(text);
        auto lcp = dsa::kasai_lcp(text, sa);
        assert(lcp.size() == text.size());
        assert(lcp[0] == 0);
    }

    // Suffix array search
    {
        std::string text = "abracadabra";
        auto sa = build_sa(text);
        auto r = dsa::suffix_array_search(text, sa, "abra");
        assert(r.size() == 2);
        assert(r[0] == 0 && r[1] == 7);
    }
    {
        std::string text = "banana";
        auto sa = build_sa(text);
        auto r = dsa::suffix_array_search(text, sa, "ana");
        assert(r.size() == 2);
    }
    {
        std::string text = "abcabcabc";
        auto sa = build_sa(text);
        auto r = dsa::suffix_array_search(text, sa, "abc");
        assert(r.size() == 3);
    }

    // Longest Previous Factor
    {
        std::string text = "mississippi";
        auto sa = build_sa(text);
        auto lcp = dsa::kasai_lcp(text, sa);
        auto lpf = dsa::longest_previous_factor(text, sa, lcp);
        assert(lpf.size() == text.size());
        assert(lpf[0] == 0);
    }

    // Phi array
    {
        std::string text = "banana";
        auto sa = build_sa(text);
        auto phi = dsa::phi_array(sa);
        assert(phi.size() == sa.size());
    }

    std::println("All suffix array search tests passed");
    return 0;
}
