// Test program for array_list
// Compile: g++ -std=c++20 -O2 -Wall -o test_array_list ch04_test_array_list.cpp
// Run:     ./test_array_list

#include "array_list.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <print>

void test_construction() {
    std::print("=== test_construction ===\n");

    dsa::array_list<int> a;
    assert(a.empty());
    assert(a.size() == 0);
    assert(a.capacity() == 4);
    std::print("  default constructor: OK\n");

    dsa::array_list<int> b(10);
    assert(b.size() == 10);
    assert(b.capacity() == 10);
    std::print("  size constructor: OK\n");

    dsa::array_list<int> c = {1, 2, 3, 4, 5};
    assert(c.size() == 5);
    assert(c[0] == 1);
    assert(c[4] == 5);
    std::print("  initializer_list: OK\n");

    dsa::array_list<int> d = c;
    assert(d.size() == c.size());
    assert(d[0] == 1);
    assert(d[4] == 5);
    c[0] = 99;
    assert(d[0] == 1);  // deep copy
    std::print("  copy constructor (deep): OK\n");

    dsa::array_list<int> e = std::move(c);
    assert(e.size() == 5);
    assert(e[0] == 99);
    assert(c.empty());  // moved-from is empty
    std::print("  move constructor: OK\n");
}

void test_push_back() {
    std::print("=== test_push_back ===\n");

    dsa::array_list<int> a;
    for (int i = 0; i < 100; ++i)
        a.push_back(i);

    assert(a.size() == 100);
    for (int i = 0; i < 100; ++i)
        assert(a[i] == i);
    std::print("  100 push_backs, values correct: OK\n");

    // Capacity should have grown
    assert(a.capacity() >= 100);
    std::cout << "  capacity after growth: " << a.capacity() << "\n";
}

void test_pop_back() {
    std::print("=== test_pop_back ===\n");

    dsa::array_list<int> a = {10, 20, 30, 40, 50};
    assert(a.back() == 50);
    a.pop_back();
    assert(a.size() == 4);
    assert(a.back() == 40);
    a.pop_back();
    a.pop_back();
    a.pop_back();
    a.pop_back();
    assert(a.empty());
    std::print("  pop_back until empty: OK\n");

    bool caught = false;
    try { a.pop_back(); }
    catch (const std::underflow_error&) { caught = true; }
    assert(caught);
    std::print("  pop_back on empty throws: OK\n");
}

void test_insert_and_erase() {
    std::print("=== test_insert_and_erase ===\n");

    dsa::array_list<int> a = {1, 2, 4, 5};
    a.insert(2, 3);       // {1, 2, 3, 4, 5}
    assert(a.size() == 5);
    assert(a[2] == 3);
    std::print("  insert at middle: OK\n");

    a.insert(0, 0);       // {0, 1, 2, 3, 4, 5}
    assert(a.size() == 6);
    assert(a[0] == 0);
    std::print("  insert at front: OK\n");

    a.insert(a.size(), 6);  // {0, 1, 2, 3, 4, 5, 6}
    assert(a.size() == 7);
    assert(a.back() == 6);
    std::print("  insert at back: OK\n");

    a.erase(0);           // {1, 2, 3, 4, 5, 6}
    assert(a.size() == 6);
    assert(a[0] == 1);
    std::print("  erase at front: OK\n");

    a.erase(3);           // {1, 2, 3, 5, 6}
    assert(a.size() == 5);
    assert(a[3] == 5);
    std::print("  erase at middle: OK\n");

    bool caught = false;
    try { a.insert(99, 0); }
    catch (const std::out_of_range&) { caught = true; }
    assert(caught);
    std::print("  insert out of range throws: OK\n");
}

void test_front_back() {
    std::print("=== test_front_back ===\n");

    dsa::array_list<int> a = {7, 14, 21};
    assert(a.front() == 7);
    assert(a.back()  == 21);
    std::print("  front/back access: OK\n");

    a.front() = 8;
    a.back()  = 22;
    assert(a[0] == 8);
    assert(a[2] == 22);
    std::print("  front/back modification: OK\n");
}

void test_iterators() {
    std::print("=== test_iterators ===\n");

    dsa::array_list<int> a = {2, 4, 6, 8, 10};

    int sum = 0;
    for (auto it = a.begin(); it != a.end(); ++it)
        sum += *it;
    assert(sum == 30);
    std::print("  iterator sum: OK\n");

    for (auto& x : a)
        x *= 2;
    assert(a[0] == 4);
    assert(a[4] == 20);
    std::print("  range-for modification: OK\n");

    const dsa::array_list<int>& ca = a;
    sum = 0;
    for (auto it = ca.begin(); it != ca.end(); ++it)
        sum += *it;
    assert(sum == 60);
    std::print("  const_iterator sum: OK\n");
}

void test_shrink_to_fit() {
    std::print("=== test_shrink_to_fit ===\n");

    dsa::array_list<int> a;
    for (int i = 0; i < 100; ++i)
        a.push_back(i);
    size_t cap_before = a.capacity();

    a.clear();
    a.shrink_to_fit();
    assert(a.size() == 0);
    assert(a.capacity() < cap_before);
    std::cout << "  capacity reduced from " << cap_before
              << " to " << a.capacity() << "\n";
}

void test_string_type() {
    std::print("=== test_string_type ===\n");

    dsa::array_list<std::string> a;
    a.push_back("hello");
    a.push_back("world");
    a.insert(1, "brave");
    assert(a[0] == "hello");
    assert(a[1] == "brave");
    assert(a[2] == "world");
    std::print("  string list: OK\n");

    // Move
    dsa::array_list<std::string> b = std::move(a);
    assert(b.size() == 3);
    assert(b[1] == "brave");
    assert(a.empty());
    std::print("  move with strings: OK\n");
}

void test_exceptions() {
    std::print("=== test_exceptions ===\n");

    dsa::array_list<int> a = {1, 2, 3};

    bool caught = false;
    try { a.at(5); }
    catch (const std::out_of_range&) { caught = true; }
    assert(caught);
    std::print("  at() out of range: OK\n");

    caught = false;
    try { a[5] = 0; }
    catch (const std::out_of_range&) { caught = true; }
    assert(caught);
    std::print("  operator[] out of range: OK\n");

    caught = false;
    try { a.erase(5); }
    catch (const std::out_of_range&) { caught = true; }
    assert(caught);
    std::print("  erase out of range: OK\n");
}

int main() {
    try {
        test_construction();
        test_push_back();
        test_pop_back();
        test_insert_and_erase();
        test_front_back();
        test_iterators();
        test_shrink_to_fit();
        test_string_type();
        test_exceptions();
        std::print("\nAll tests passed!\n");
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
