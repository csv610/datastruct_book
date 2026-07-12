CXX      := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2

TESTS := test_ch04_array_list test_ch05_linked_list

.PHONY: all test clean

all: $(TESTS)

test: $(TESTS)
	@for t in $(TESTS); do echo "Running $$t..."; ./$$t; done

test_ch04_array_list: test_ch04_array_list.cpp ch04_array_list.h
	$(CXX) $(CXXFLAGS) -o $@ test_ch04_array_list.cpp

test_ch05_linked_list: test_ch05_linked_list.cpp ch05_linked_list.h
	$(CXX) $(CXXFLAGS) -o $@ test_ch05_linked_list.cpp

clean:
	rm -f $(TESTS)
