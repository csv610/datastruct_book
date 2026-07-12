auto is_positive = [](int x) { return x > 0; };
auto sum = [](int a, int b) { return a + b; };

// With capture
int offset = 10;
auto shifted = [offset](int x) { return x + offset; };

// Mutable lambda (captured variables can be modified)
auto counter = [count = 0]() mutable { return ++count; };

// Generic lambda (C++14)
auto add = [](auto a, auto b) { return a + b; };

// Lambda as a comparator
std::vector<int> v = {3, 1, 4, 1, 5};
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
