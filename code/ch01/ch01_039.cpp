// [capture](parameters) -> return_type { body }
auto square = [](int x) { return x * x; };
int result = square(5);  // 25

// With no parameters
auto greet = []() { /* ... */ };

// Trailing return type
auto divide = [](double a, double b) -> double {
    return a / b;
};
