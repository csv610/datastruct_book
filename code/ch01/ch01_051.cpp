#include <tuple>
#include <map>
#include <string>

// Pairs
auto [x, y] = std::pair{1, 2.0};  // x is int, y is double

// Tuples
auto [a, b, c] = std::tuple{1, "hello", 3.14};

// Structs
struct Point { double x, y; };
auto [px, py] = Point{1.0, 2.0};
