constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// Compile-time evaluation
static_assert(factorial(5) == 120);

// Runtime evaluation (n is not constexpr)
int x = 10;
int y = factorial(x);  // computed at runtime
