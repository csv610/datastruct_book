// Test program for linked_list
// Compile: g++ -std=c++20 -O2 -Wall -o test_linked_list test_ch05_linked_list.cpp
// Run:     ./test_linked_list

#include "linked_list.h"
#include <cassert>
#include <iostream>
#include <string>
#include <print>

void test_construction() {
    std::print("=== test_construction ===\n");

    dsa::linked_list<int> a;
    assert(a.empty());
    assert(a.size() == 0);
    std::print("  default constructor: OK\n");

    dsa::linked_list<int> b = {10, 20, 30};
    assert(b.size() == 3);
    assert(b.front() == 10);
    assert(b.back()  == 30);
    std::print("  initializer_list: OK\n");

    dsa::linked_list<int> c = b;
    assert(c.size() == 3);
    b.push_front(5);
    assert(c.front() == 10);  // deep copy
    std::print("  copy constructor (deep): OK\n");

    dsa::linked_list<int> d = std::move(c);
    assert(d.size() == 3);
    assert(c.empty());
    std::print("  move constructor: OK\n");
}

void test_push_front_back() {
    std::print("=== test_push_front_back ===\n");

    dsa::linked_list<int> a;
    a.push_front(30);
    a.push_front(20);
    a.push_front(10);
    assert(a.front() == 10);
    assert(a.back()  == 30);
    assert(a.size()  == 3);
    std::print("  push_front sequence: OK\n");

    a.push_back(40);
    a.push_back(50);
    assert(a.back()  == 50);
    assert(a.size()  == 5);
    std::print("  push_back sequence: OK\n");

    // Verify order: 10, 20, 30, 40, 50
    int expected = 10;
    for (auto it = a.begin(); it != a.end(); ++it) {
        assert(*it == expected);
        expected += 10;
    }
    std::print("  sequence order preserved: OK\n");
}

void test_pop_front() {
    std::print("=== test_pop_front ===\n");

    dsa::linked_list<int> a = {1, 2, 3, 4, 5};
    a.pop_front();
    assert(a.front() == 2);
    assert(a.size() == 4);
    a.pop_front();
    assert(a.front() == 3);
    a.pop_front();
    a.pop_front();
    a.pop_front();
    assert(a.empty());
    std::print("  pop_front until empty: OK\n");
}

void test_clear() {
    std::print("=== test_clear ===\n");

    dsa::linked_list<int> a = {1, 2, 3, 4, 5};
    assert(!a.empty());
    a.clear();
    assert(a.empty());
    assert(a.size() == 0);
    a.push_back(99);
    assert(a.front() == 99);
    std::print("  clear then reuse: OK\n");
}

void test_reverse() {
    std::print("=== test_reverse ===\n");

    dsa::linked_list<int> a = {10, 20, 30, 40, 50};
    a.reverse();
    int expected = 50;
    for (auto it = a.begin(); it != a.end(); ++it) {
        assert(*it == expected);
        expected -= 10;
    }
    assert(a.front() == 50);
    assert(a.back()  == 10);
    std::print("  reverse: OK\n");

    dsa::linked_list<int> b;
    b.reverse();  // empty reverse
    assert(b.empty());
    std::print("  reverse (empty): OK\n");

    dsa::linked_list<int> c = {42};
    c.reverse();
    assert(c.front() == 42);
    std::print("  reverse (single element): OK\n");
}

void test_iterators() {
    std::print("=== test_iterators ===\n");

    dsa::linked_list<int> a = {2, 4, 6, 8, 10};

    int sum = 0;
    for (auto it = a.begin(); it != a.end(); ++it)
        sum += *it;
    assert(sum == 30);
    std::print("  iterator sum: OK\n");

    for (auto& x : a)
        x /= 2;
    assert(a.front() == 1);
    assert(a.back()  == 5);
    std::print("  range-for modification: OK\n");
}

void test_string_type() {
    std::print("=== test_string_type ===\n");

    dsa::linked_list<std::string> a;
    a.push_back("data");
    a.push_front("structures");
    a.push_back("algorithms");

    auto it = a.begin();
    assert(*it == "structures"); ++it;
    assert(*it == "data");       ++it;
    assert(*it == "algorithms");
    std::print("  string list: OK\n");
}

void test_exceptions() {
    std::print("=== test_exceptions ===\n");

    dsa::linked_list<int> a;

    bool caught = false;
    try { a.front(); }
    catch (const std::underflow_error&) { caught = true; }
    assert(caught);
    std::print("  front() on empty throws: OK\n");

    caught = false;
    try { a.pop_front(); }
    catch (const std::underflow_error&) { caught = true; }
    assert(caught);
    std::print("  pop_front() on empty throws: OK\n");
}

int main() {
    try {
        test_construction();
        test_push_front_back();
        test_pop_front();
        test_clear();
        test_reverse();
        test_iterators();
        test_string_type();
        test_exceptions();
        std::print("\nAll linked_list tests passed!\n");
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
