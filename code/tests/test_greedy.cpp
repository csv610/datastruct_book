#include "greedy.h"
#include <cassert>
#include <string>
#include <print>

int main() {
    // Fractional knapsack
    std::vector<dsa::frac_item> items = {{10, 60}, {20, 100}, {30, 120}};
    double result = dsa::fractional_knapsack(50, items);
    assert(std::abs(result - 240.0) < 1e-6);

    // Single item, full fit
    std::vector<dsa::frac_item> items2 = {{10, 60}};
    assert(std::abs(dsa::fractional_knapsack(20, items2) - 60.0) < 1e-6);

    // Partial fit
    std::vector<dsa::frac_item> items3 = {{10, 60}};
    assert(std::abs(dsa::fractional_knapsack(5, items3) - 30.0) < 1e-6);

    // Huffman coding
    std::vector<std::pair<char, int>> freq = {{'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};
    auto codes = dsa::huffman_encode(freq);
    assert(codes.size() == 6);

    // f should have a short code (freq 45)
    for (auto& [ch, code] : codes) {
        if (ch == 'f') assert(code.size() <= 2);
    }

    // All codes should be unique
    for (std::size_t i = 0; i < codes.size(); ++i)
        for (std::size_t j = i + 1; j < codes.size(); ++j)
            assert(codes[i].second != codes[j].second);

    std::print("All greedy tests passed\n");
    return 0;
}
