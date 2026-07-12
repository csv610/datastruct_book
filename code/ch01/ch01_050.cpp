consteval int square(int n) {
    return n * n;
}

// int x = 5;
// int y = square(x);  // ERROR: x is not constexpr
constexpr int y = square(5);  // OK: compile-time
