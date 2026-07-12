# Modern C++ Review

## Introduction

This chapter reviews the C++20 features we will use throughout the book. If you have programmed in C++ before, much of this will be familiar — but pay attention to the newer features like move semantics, smart pointers, concepts, and ranges, as they are used extensively in our implementations.

Our goal is not to teach C++ from scratch, but to establish a common vocabulary and set of patterns. Readers needing a more thorough introduction should consult the references at the end of this chapter.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Functions and Parameters

### Pass by Value, Reference, and Const Reference

C++ offers three primary ways to pass arguments:

```cpp
// Pass by value — a copy is made
int square(int x) {
    return x * x;
}

// Pass by reference — no copy, can modify
void increment(int& x) {
    ++x;
}

// Pass by const reference — no copy, read-only
int sum(const std::vector<int>& values) {
    int s = 0;
    for (int v : values) s += v;
    return s;
}
```

**Rule of thumb:** Use pass by value for small types (int, char, pointer). Use const reference for types larger than a pointer. Use reference only when you need to modify the argument.

### Default Arguments

```cpp
int multiply(int a, int b = 1) {
    return a * b;
}
// multiply(5) == 5, multiply(5, 3) == 15
```

### Function Overloading

Multiple functions may share a name as long as their parameter types differ:

```cpp
int max(int a, int b) { return a > b ? a : b; }
double max(double a, double b) { return a > b ? a : b; }
```

### Lambda Expressions

Lambdas are anonymous function objects. They are essential for generic algorithms:

```cpp
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
```

### constexpr and consteval

`constexpr` functions are evaluated at compile time when possible:

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}
// static_assert(factorial(5) == 120);  // compile-time evaluation
```

C++20 introduces `consteval` for functions that must be evaluated at compile time.

## Dynamic Memory and RAII

### The Problem with Raw Pointers

In C++98, dynamic memory was managed with `new` and `delete`:

```cpp
int* p = new int(5);
// ... use p ...
delete p;  // easy to forget
```

Forgetting to `delete` causes memory leaks. Deleting twice causes undefined behavior. Exception safety is manual and error-prone.

### Smart Pointers (C++11)

C++11 introduced smart pointers that automatically free memory when they go out of scope:

```cpp
#include <memory>

std::unique_ptr<int> p = std::make_unique<int>(5);
// No delete needed — freed automatically when p goes out of scope
```

**`std::unique_ptr`** — exclusive ownership. Cannot be copied, only moved.

```cpp
auto p1 = std::make_unique<int>(5);
// auto p2 = p1;  // error: cannot copy unique_ptr
auto p2 = std::move(p1);  // OK: transfer ownership
// p1 is now null
```

**`std::shared_ptr`** — shared ownership via reference counting.

```cpp
auto s1 = std::make_shared<int>(5);
auto s2 = s1;  // OK: both now point to the same int
// Memory freed when both s1 and s2 go out of scope
```

**`std::weak_ptr`** — non-owning observer that can detect when the object is destroyed.

Throughout this book, we use `std::unique_ptr` for owning pointers in linked data structures. We use raw pointers only for non-owning references.

### RAII (Resource Acquisition Is Initialization)

RAII is the most important C++ idiom. Resources (memory, file handles, mutexes) are acquired in a constructor and released in the destructor:

```cpp
class FileHandle {
    FILE* f_;
public:
    FileHandle(const char* name, const char* mode)
        : f_(fopen(name, mode)) {
        if (!f_) throw std::runtime_error("Cannot open file");
    }
    ~FileHandle() { if (f_) fclose(f_); }
    
    // No copy (for simplicity)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};
```

Every data structure in this book follows RAII principles.

## Templates and Concepts

### Function Templates

```cpp
template <typename T>
T max_value(T a, T b) {
    return a > b ? a : b;
}
```

### Class Templates

```cpp
template <typename T>
class Box {
    T value_;
public:
    explicit Box(T value) : value_(std::move(value)) {}
    const T& get() const { return value_; }
    void set(const T& value) { value_ = value; }
};
```

### Template Template Parameters and Variadic Templates

```cpp
template <typename T, template <typename...> class Container = std::vector>
class MyCollection {
    Container<T> items_;
};
```

### Concepts (C++20)

Concepts constrain template parameters with compile-time predicates:

```cpp
template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

template <Comparable T>
T min_value(T a, T b) {
    return a < b ? a : b;
}

// Equivalent abbreviated form:
auto min_value(Comparable auto a, Comparable auto b) {
    return a < b ? a : b;
}
```

Concepts give better error messages and enable overloading based on type properties:

```cpp
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
T sum(T a, T b) { return a + b; }
```

Throughout this book, we use concepts to make template interfaces self-documenting.

## Move Semantics

### Lvalues and Rvalues

An *lvalue* has an identity and persists beyond a single expression. An *rvalue* is temporary.

```cpp
int x = 5;      // x is an lvalue, 5 is an rvalue
int y = x + 2;  // (x + 2) is an rvalue
```

### Move Constructor and Move Assignment

Move semantics transfer resources from a temporary object, avoiding expensive copies:

```cpp
class Buffer {
    size_t size_;
    int* data_;
public:
    // Constructor
    explicit Buffer(size_t size) : size_(size), data_(new int[size]) {}
    
    // Destructor
    ~Buffer() { delete[] data_; }
    
    // Copy constructor (expensive)
    Buffer(const Buffer& other) : size_(other.size_), data_(new int[other.size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
    
    // Move constructor (cheap)
    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }
    
    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }
};
```

### Rule of Five

If you define any of: destructor, copy constructor, copy assignment, move constructor, or move assignment, you should typically define all five.

In modern C++ with smart pointers, the Rule of Five is rarely needed — the Rule of Zero applies: let the smart pointers manage resources automatically.

## std::vector and std::span

### std::vector

`std::vector` is the most important container in modern C++. It is a dynamic array:

```cpp
std::vector<int> v;           // empty
std::vector<int> v(10);       // 10 elements, default-initialized
std::vector<int> v = {1,2,3}; // initializer list

v.push_back(4);               // add to end: O(1) amortized
v.pop_back();                 // remove from end: O(1)
v[0] = 10;                    // O(1) index
v.size();                     // number of elements
v.empty();                    // true if size == 0
```

### std::span (C++20)

`std::span` is a non-owning view over a contiguous sequence:

```cpp
void process(std::span<int> data) {
    for (int& x : data) x *= 2;
}

std::vector<int> v = {1, 2, 3};
process(v);              // span from vector
int arr[] = {4, 5, 6};
process(arr);            // span from C array
```

`std::span` is ideal for function parameters that need to read or write a contiguous sequence without being tied to a specific container type.

### std::string_view (C++17)

`std::string_view` is a non-owning view over a character sequence:

```cpp
auto len(std::string_view s) -> size_t {
    return s.size();
}
std::string a = "hello";
len(a);         // from std::string
len("world");   // from string literal — no allocation!
```

## Ranges and Views (C++20)

C++20 introduces the Ranges library for composable operations on sequences:

```cpp
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
```

Ranges are used throughout the book where they improve clarity.

## Error Handling

### Exceptions

C++ uses exceptions for error handling:

```cpp
#include <stdexcept>

int divide(int a, int b) {
    if (b == 0) throw std::invalid_argument("Division by zero");
    return a / b;
}
```

Standard exception classes: `std::logic_error`, `std::invalid_argument`, `std::domain_error`, `std::length_error`, `std::out_of_range`, `std::runtime_error`, `std::range_error`, `std::overflow_error`.

### noexcept

Functions that cannot throw should be marked `noexcept`:

```cpp
void swap(int& a, int& b) noexcept {
    int t = a; a = b; b = t;
}
```

Move constructors and move assignments should be `noexcept` to enable optimal container performance.

### std::optional (C++17)

For functions that may or may not produce a result:

```cpp
#include <optional>

std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}
```

## Iterators and Ranges

### Iterator Categories

- **Input iterators** — read once, forward (istream_iterator)
- **Output iterators** — write once, forward (ostream_iterator)
- **Forward iterators** — read/write, forward (forward_list)
- **Bidirectional iterators** — forward and backward (list, set, map)
- **Random access iterators** — O(1) index (vector, deque, array)

### Range-Based for Loop

```cpp
std::vector<int> v = {1, 2, 3};
for (int x : v) { /* read */ }
for (int& x : v) { /* modify */ }
for (const auto& x : v) { /* read, avoid copy */ }
```

### Iterator Adapters

```cpp
std::vector<int> v = {1, 2, 3};
// Back inserter
std::fill_n(std::back_inserter(v), 3, 0);  // appends 0, 0, 0

// Stream iterator
std::copy(v.begin(), v.end(),
          std::ostream_iterator<int>(std::cout, " "));
```

## Type Deduction: auto and decltype

```cpp
auto x = 5;          // int
auto y = 3.14;       // double
auto z = &x;         // int*

auto& ref = x;       // int&
const auto& cref = x; // const int&

auto&& universal = x;  // forwarding reference
```

`decltype` deduces the exact type of an expression:

```cpp
int x = 5;
decltype(x) y = 10;  // int
decltype((x)) z = x; // int&
```

## Structured Bindings (C++17)

```cpp
std::pair<int, std::string> p = {1, "hello"};
auto [id, name] = p;  // id = 1, name = "hello"

std::map<std::string, int> m = {{"a", 1}, {"b", 2}};
for (const auto& [key, value] : m) {
    // ...
}
```

## Coroutines (C++20)

Coroutines enable suspend/resume functions. We use them for lazy generators:

```cpp
#include <generator>
#include <ranges>

std::generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        int next = a + b;
        a = b;
        b = next;
    }
}

// Usage:
// for (int f : fibonacci(10)) { ... }
```

(Full coroutine coverage is beyond this chapter; we introduce them where needed in later chapters.)

## Common Patterns in This Book

Throughout the book, we follow these conventions:

1. **Template parameters** named `T` for the element type.
2. **Concepts** used to document template requirements.
3. **`noexcept`** on move operations and swaps.
4. **`explicit`** on single-argument constructors.
5. **`const`** correctness throughout.
6. **`std::unique_ptr`** for owning pointers in linked structures.
7. **`std::size_t`** for indices and sizes.
8. **Iterators** following STL conventions (begin/end pairs).

### Example: A Generic Container Skeleton

```cpp
#include <concepts>
#include <memory>
#include <cstddef>

template <typename T>
class simple_vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    simple_vector() = default;
    
    explicit simple_vector(size_type n)
        : size_(n), capacity_(n), data_(std::make_unique<T[]>(n)) {}
    
    // Rule of Zero — unique_ptr handles everything
    
    T& operator[](size_type i) { return data_[i]; }
    const T& operator[](size_type i) const { return data_[i]; }
    
    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    
private:
    size_type size_ = 0;
    size_type capacity_ = 0;
    std::unique_ptr<T[]> data_;
};
```

## Compiler and Build

All code in this book compiles with:

- GCC 12+ with `-std=c++20`
- Clang 16+ with `-std=c++20`
- MSVC 2022 17.6+ with `/std:c++20`

Recommended build command:

```bash
g++ -std=c++20 -O2 -Wall -Wextra -o program program.cpp
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Using `std::move` on `const` objects | Move degrades to copy — object remains unmodified | Only `std::move` non-`const` objects |
| Returning references to local variables | Dangling reference, undefined behavior | Return by value, or ensure lifetime extends past caller |
| Forgetting `noexcept` on move operations | `std::vector` copies instead of moves during reallocation | Always mark move constructor/assignment `noexcept` |
| Storing `std::string_view` to temporary strings | View points to freed memory | Only use `string_view` when the source outlives the view |
| Circular `shared_ptr` references | Memory leak — ref count never reaches zero | Use `weak_ptr` to break cycles |
| Passing by `const T&` for small types (int, char, pointer) | Unnecessary indirection, inhibits optimization | Pass small trivially-copyable types by value |
| Using `auto` with braced initializers | `auto x{1}` is `std::initializer_list<int>`, not `int` | Use `auto x = 1` or be explicit |

## Summary

1. Use **RAII** to manage all resources. Prefer smart pointers over raw `new`/`delete`.
2. Use **move semantics** to avoid unnecessary copies.
3. Use **concepts** to constrain templates and produce clearer error messages.
4. Use `std::span` and `std::string_view` for non-owning views of contiguous data.
5. Follow the **Rule of Zero** — let automatic resource management do its job.
6. Mark move operations as `noexcept` to enable optimal container performance.

## Exercises

### Drill

1. **TCP connection pool.** A networked service maintains a pool of open connections to a database. Each connection is owned by the pool via `unique_ptr`. Why is `unique_ptr` the right choice here? What happens if you use `shared_ptr` instead and the pool destructor runs while a worker thread still holds a copy? Write a small demo.

   *(In production, libraries like Redis use connection pools exactly this way — `unique_ptr` ensures each connection has exactly one owner at a time.)*

2. **Logging service.** A high-throughput logging system buffers log messages as `std::string` objects in a `std::vector`. Why does marking the string's move constructor `noexcept` matter? Estimate how many extra copies happen during vector reallocation if moves fall back to copies.

   *(In production, Google's Protobuf library relies on noexcept moves to maintain throughput in their RPC layer.)*

3. **Cache key safety.** A web cache stores items in a hash map keyed by URL string. An intern changes the key type to `std::string_view` to "avoid copies." What goes wrong when the owning URL string is removed from the cache?

4. **Function return bug.** A compiler optimization pass has a function that finds an instruction and returns a reference to it:
   ```cpp
   const Instruction& findInst(const std::vector<Instruction>& list, int id) {
       for (const auto& inst : list)
           if (inst.id() == id) return inst;
       // Oops — what do we return here?
   }
   ```
   Fix this function. What should the return type be? What happens in a real compiler when this bug exists?

5. **Circular references.** Two objects hold `shared_ptr` to each other. They are never destroyed, even after all external references are gone. Why? Demonstrate with code and fix using `weak_ptr`.

### Application

6. **Query pipeline.** A database engine processes queries as a pipeline: scan reads rows, filter checks conditions, aggregate computes sums. Each stage owns the next via `unique_ptr`. Implement a class hierarchy with `ScanNode`, `FilterNode`, and `AggregateNode`. Each has a `next()` that returns the next row (use `std::optional`). Demonstrate correct destruction order when the pipeline is torn down.

   *(In production, DuckDB uses this exact pattern — `unique_ptr` chains of operator nodes.)*

7. **Socket wrapper.** Write an RAII wrapper around a POSIX socket file descriptor. The class should:
   - Close the fd on destruction
   - Support move (transfer ownership)
   - Be non-copyable
   - Provide `send(span<const char>)` and `recv()` methods
   Demonstrate moving a socket from an acceptor to a connection handler.

   *(In production, NGINX and similar servers use this pattern for connection lifetime management.)*

8. **Log parser.** A profiler writes timestamped function call records to a binary log. Write a parser that reads a `vector<char>` from disk and uses `std::span` and `std::string_view` to extract fields without any heap allocation. Compare the allocation count with a version that uses `std::string` for each field.

   *(In production, Chrome's V8 profiler and many trading systems use zero-copy parsing to avoid GC pressure.)*

9. **Constrained serialization.** Write a `serialize` function using C++20 concepts that works for any type. For trivially-copyable types, write raw bytes. For other types, call a `serialize(ostream&)` member function. Demonstrate that passing a non-trivially-copyable type without the member function produces a clear compile error.

10. **Ring buffer.** A network protocol processes messages from a stream. Implement a fixed-capacity `RingBuffer<T>` with RAII. Support `push`, `pop`, `front`, `back`, `size`, `empty`. When full, overwrite the oldest element (sliding window). Compare performance with `std::deque<T>`.

    *(In production, PostgreSQL uses a ring buffer for its wire protocol message framing.)*

### Research

11. **Custom allocator.** Implement a `PoolAllocator<T>` that allocates from a fixed arena. Use it with `std::vector` and measure allocation overhead vs the default allocator for 10^6 push_back operations. When does an arena allocator improve performance?

    *(In production, Google's TCMalloc and other allocators use thread-local caches to reduce contention.)*

12. **Perfect forwarding.** Study how `std::forward` works. Implement a `make_box` factory that constructs any `Box<T>` with perfect forwarding. Show it works for both lvalue and rvalue arguments. Then extend to `make_unique_for_overwrite` (C++20).

13. **Small object optimization.** Implement a `SmallVector<T, N>` that stores up to N elements inline before heap-allocating. Use `std::aligned_storage` and placement new/delete. Benchmark push_back against `std::vector<T>` for various T sizes. Show the allocation-free regime.

14. **Coroutine generator.** Implement a `generator<T>` using C++20 coroutines that yields parsed tokens from a text input. Compare readability and generated assembly with an iterator-based version. Use Compiler Explorer to see whether the coroutine frame is heap-allocated.

## References and Further Reading

- Bjarne Stroustrup, *The C++ Programming Language*, 4th Edition. Addison-Wesley, 2013.
- Bjarne Stroustrup, "C++11 — the new ISO C++ standard." *The C++ Source*, 2011.
- Nicolai M. Josuttis, *C++17 — The Complete Guide*. Leanpub, 2019.
- Nicolai M. Josuttis, *C++20 — The Complete Guide*. Leanpub, 2022.
- Rainer Grimm, *C++20: Get the Details*. Leanpub, 2021.
- Herb Sutter, "Exceptional C++: 47 Engineering Puzzles, Programming Problems, and Solutions." Addison-Wesley, 2000.
- Herb Sutter and Andrei Alexandrescu, "C++ Coding Standards: 101 Rules, Guidelines, and Best Practices." Addison-Wesley, 2005.
- Scott Meyers, *Effective Modern C++*. O'Reilly, 2014.
- Scott Meyers, "Universal References in C++11." — Overload Journal, 2012.
- David Vandevoorde, Nicolai M. Josuttis, and Douglas Gregor, *C++ Templates: The Complete Guide*, 2nd Edition. Addison-Wesley, 2017.
- Andrei Alexandrescu, *Modern C++ Design*. Addison-Wesley, 2001.
- cppreference.com — The definitive online C++ reference.
- ISO C++ Standard Committee, "Working Draft, Standard for Programming Language C++" (N4901, 2021).
