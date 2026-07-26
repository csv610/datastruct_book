CXX      := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude

TESTS := ch01_test ch02_test ch03_test ch04_test ch05_test ch06_test ch07_test \
         test_ch07_deque test_ch11_red_black_tree test_ch05_sparse_matrix test_ch13_suffix_array

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

test_ch07_deque: tests/test_ch07_deque.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_ch11_red_black_tree: tests/test_ch11_red_black_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_ch05_sparse_matrix: tests/test_ch05_sparse_matrix.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_ch13_suffix_array: tests/test_ch13_suffix_array.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
