CXX      := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude

TESTS := ch01_test ch02_test ch03_test ch04_test ch05_test ch06_test ch07_test \
         test_deque test_red_black_tree test_sparse_matrix test_suffix_array

.PHONY: all test clean

all: $(TESTS)

test: $(TESTS)
	@for t in $(TESTS); do echo "Running $$t..."; ./$$t; done

ch01_test: tests/ch01_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ch02_test: tests/ch02_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ch03_test: tests/ch03_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ch04_test: tests/ch04_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ch05_test: tests/ch05_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ch06_test: tests/ch06_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

ch07_test: tests/ch07_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_deque: tests/test_deque.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_red_black_tree: tests/test_red_black_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_sparse_matrix: tests/test_sparse_matrix.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_suffix_array: tests/test_suffix_array.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
