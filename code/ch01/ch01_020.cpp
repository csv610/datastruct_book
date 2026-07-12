#include <ranges>
#include <vector>
#include <iostream>

std::vector<int> v = {1, 2, 3, 4, 5, 6};

// Filter even numbers, square them, take first 2
auto result = v
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; })
    | std::views::take(2);

for (int x : result) std::cout << x << ' ';  // prints: 4 16
