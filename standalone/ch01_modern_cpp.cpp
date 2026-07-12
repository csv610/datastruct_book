// ch01: Modern C++ Review — lambdas, constexpr, smart pointers, move semantics, ranges
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// ---- move semantics: Buffer class ----
class Buffer {
    char* data_;
    size_t size_;
public:
    Buffer(size_t n) : data_(new char[n]()), size_(n) { std::cout << "  construct\n"; }
    ~Buffer() { delete[] data_; std::cout << "  destroy\n"; }

    Buffer(const Buffer& other) : data_(new char[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  copy\n";
    }
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        std::cout << "  copy assign\n";
        return *this;
    }

    Buffer(Buffer&& other) noexcept : data_(std::exchange(other.data_, nullptr)), size_(other.size_) {
        std::cout << "  move\n";
    }
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = other.size_;
        }
        std::cout << "  move assign\n";
        return *this;
    }
};

Buffer make_buffer(size_t n) { return Buffer(n); }

// ---- simple_vector with concepts ----
template <std::regular T>
class simple_vector {
    T* data_;
    size_t size_;
public:
    simple_vector() : data_(nullptr), size_(0) {}
    explicit simple_vector(size_t n) : data_(new T[n]()), size_(n) {}
    ~simple_vector() { delete[] data_; }

    simple_vector(const simple_vector& other) : data_(new T[other.size_]), size_(other.size_) {
        for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
    }
    simple_vector(simple_vector&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)), size_(other.size_) {}

    T& operator[](size_t i) { return data_[i]; }
    size_t size() const { return size_; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
};

constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

int main() {
    // lambda
    auto add = [](int a, int b) { return a + b; };
    std::cout << "lambda add(3,4)=" << add(3, 4) << "\n";

    // constexpr
    std::cout << "constexpr factorial(5)=" << factorial(5) << "\n";

    // smart pointers
    auto p = std::make_unique<int>(42);
    std::cout << "unique_ptr=" << *p << "\n";
    auto s = std::make_shared<std::string>("hello");
    std::cout << "shared_ptr=" << *s << "\n";

    // move semantics
    std::cout << "Buffer move demo:\n";
    Buffer b1(10);
    Buffer b2 = std::move(b1);
    std::cout << "  b2 size = " << sizeof(b2) << " (stack)\n";

    // std::span
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::span<int> sp(vec);
    std::cout << "span:";
    for (auto x : sp) std::cout << " " << x;
    std::cout << "\n";

    // std::string_view
    std::string_view sv = "Hello World";
    std::cout << "string_view: " << sv.substr(0, 5) << "\n";

    // ranges
    auto even = vec | std::views::filter([](int n) { return n % 2 == 0; });
    std::cout << "ranges filter even:";
    for (int n : even) std::cout << " " << n;
    std::cout << "\n";

    // optional
    auto divide = [](int a, int b) -> std::optional<int> {
        if (b == 0) return std::nullopt;
        return a / b;
    };
    auto r = divide(10, 2);
    std::cout << "optional(10/2)=" << (r ? std::to_string(*r) : "nullopt") << "\n";

    // simple_vector
    simple_vector<int> sv2(3);
    sv2[0] = 10; sv2[1] = 20; sv2[2] = 30;
    std::cout << "simple_vector:";
    for (auto x : sv2) std::cout << " " << x;
    std::cout << "\n";

    std::cout << "All ch01 tests passed.\n";
    return 0;
}
