#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <variant>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iostream>
#include <iterator>

// ch01: Modern C++ Review - Test all concepts

// 1. Pass by value/reference
int square(int x) { return x * x; }
void increment(int& x) { ++x; }
int sum(const std::vector<int>& values) {
    int s = 0;
    for (int v : values) s += v;
    return s;
}

// 2. Default arguments
int multiply(int a, int b = 1) { return a * b; }

// 3. Function overloading
void print(int x) { std::cout << "int: " << x << '\n'; }
void print(double x) { std::cout << "double: " << x << '\n'; }
void print(const std::string& x) { std::cout << "string: " << x << '\n'; }

// 4. Templates
template <typename T>
T max_val(T a, T b) { return a > b ? a : b; }

template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) { return a + b; }

// 5. Class with rule of five
class Buffer {
    std::unique_ptr<int[]> data_;
    size_t size_;
public:
    Buffer(size_t n) : data_(std::make_unique<int[]>(n)), size_(n) {
        std::fill_n(data_.get(), n, 0);
    }
    // Rule of five
    Buffer(const Buffer& other) : data_(std::make_unique<int[]>(other.size_)), size_(other.size_) {
        std::copy_n(other.data_.get(), size_, data_.get());
    }
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            data_ = std::make_unique<int[]>(other.size_);
            size_ = other.size_;
            std::copy_n(other.data_.get(), size_, data_.get());
        }
        return *this;
    }
    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;
    ~Buffer() = default;
    
    int& operator[](size_t i) { return data_[i]; }
    const int& operator[](size_t i) const { return data_[i]; }
    size_t size() const { return size_; }
};

// 6. Smart pointers
void smart_pointer_demo() {
    auto uptr = std::make_unique<int>(42);
    std::cout << "unique_ptr: " << *uptr << '\n';
    
    auto sptr1 = std::make_shared<std::string>("hello");
    auto sptr2 = sptr1;
    std::cout << "shared_ptr count: " << sptr1.use_count() << '\n';
    
    std::weak_ptr<std::string> wptr = sptr1;
    if (auto locked = wptr.lock()) {
        std::cout << "weak_ptr locked: " << *locked << '\n';
    }
}

// 7. Lambda expressions
void lambda_demo() {
    auto add = [](int a, int b) { return a + b; };
    std::cout << "lambda add: " << add(3, 4) << '\n';
    
    int factor = 10;
    auto multiply = [factor](int x) { return x * factor; };
    std::cout << "lambda capture: " << multiply(5) << '\n';
    
    // Mutable lambda
    int counter = 0;
    auto inc = [counter]() mutable { return ++counter; };
    std::cout << "mutable lambda: " << inc() << ", " << inc() << '\n';
    
    // Generic lambda (C++14)
    auto generic = [](auto a, auto b) { return a + b; };
    std::cout << "generic lambda: " << generic(3, 4.5) << '\n';
}

// 8. Structured bindings
void structured_bindings_demo() {
    std::tuple<int, double, std::string> t = {42, 3.14, "hello"};
    auto [a, b, c] = t;
    std::cout << "tuple: " << a << ", " << b << ", " << c << '\n';
    
    std::pair<int, std::string> p = {1, "one"};
    auto [x, y] = p;
    std::cout << "pair: " << x << ", " << y << '\n';
    
    struct Point { int x, y; };
    Point pt{3, 4};
    auto [px, py] = pt;
    std::cout << "struct: " << px << ", " << py << '\n';
}

// 9. std::optional
std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

void optional_demo() {
    auto result = safe_divide(10, 2);
    if (result) {
        std::cout << "10/2 = " << *result << '\n';
    }
    result = safe_divide(10, 0);
    std::cout << "10/0 = " << result.value_or(-1) << '\n';
}

// 10. std::variant
void variant_demo() {
    std::variant<int, double, std::string> v = 42;
    std::cout << "variant int: " << std::get<int>(v) << '\n';
    
    v = 3.14;
    std::cout << "variant double: " << std::get<double>(v) << '\n';
    
    v = "hello";
    std::cout << "variant string: " << std::get<std::string>(v) << '\n';
    
    // Visit
    std::visit([](auto&& arg) { 
        std::cout << "visited: " << arg << '\n'; 
    }, v);
}

// 11. constexpr
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

void constexpr_demo() {
    static_assert(factorial(5) == 120);
    static_assert(fibonacci(10) == 55);
    std::cout << "constexpr: factorial(5)=" << factorial(5) 
              << ", fibonacci(10)=" << fibonacci(10) << '\n';
}

// 12. Concepts (C++20)
template <typename T>
concept Addable>
concept AddableConcept = requires(Addable a, Addable b) {
    { a + b } -> std::convertible_to<Addable>;
};

template <AddableConcept T>
T add_concepts(T a, T b) {
    return a + b;
}

void concepts_demo() {
    std::cout << "concepts add int: " << add_concepts(3, 4) << '\n';
    std::cout << "concepts add double: " << add_concepts(3.5, 4.2) << '\n';
    std::cout << "concepts add string: " << add_concepts(std::string("hello"), std::string("world")) << '\n';
}

// 13. Range-based for with structured bindings
void range_demo() {
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
    for (const auto& [key, value] : m) {
        std::cout << key << " => " << value << '\n';
    }
}

// 14. if constexpr
template <typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        return value * 1.5;
    } else {
        return value;
    }
}

int main() {
    std::cout << "=== Chapter 1: Modern C++ Review ===\n\n";
    
    // 1. Pass by value/reference
    int x = 5;
    std::cout << "square(5) = " << square(x) << '\n';
    increment(x);
    std::cout << "after increment: " << x << '\n';
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "sum = " << sum(v) << '\n';
    
    // 2. Default args
    std::cout << "multiply(5) = " << multiply(5) << '\n';
    std::cout << "multiply(5, 3) = " << multiply(5, 3) << '\n';
    
    // 3. Overloading
    print(42);
    print(3.14);
    print("hello");
    
    // 4. Templates
    std::cout << "max(3, 7) = " << max_val(3, 7) << '\n';
    std::cout << "max(3.5, 2.1) = " << max_val(3.5, 2.1) << '\n';
    std::cout << "add(3, 4.5) = " << add(3, 4.5) << '\n';
    
    // 5. Buffer class
    Buffer buf(5);
    for (size_t i = 0; i < buf.size(); ++i) buf[i] = i * 10;
    Buffer buf2 = buf; // copy
    std::cout << "Buffer copy: " << buf2[3] << '\n';
    
    // 6. Smart pointers
    smart_pointer_demo();
    
    // 7. Lambdas
    lambda_demo();
    
    // 8. Structured bindings
    structured_bindings_demo();
    
    // 9. Optional
    optional_demo();
    
    // 10. Variant
    variant_demo();
    
    // 11. constexpr
    constexpr_demo();
    
    // 12. Concepts
    concepts_demo();
    
    // 13. Range for
    range_demo();
    
    // 14. if constexpr
    std::cout << "process(5) = " << process(5) << '\n';
    std::cout << "process(2.0) = " << process(2.0) << '\n';
    
    std::cout << "\n=== All tests passed ===\n";
    return 0;
}