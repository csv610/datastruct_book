CXX      := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude

TESTS := test_array_list test_deque test_red_black_tree test_sparse_matrix test_suffix_array \
         test_binomial_heap test_fibonacci_heap test_pairing_heap \
         test_splay_tree test_two_three_tree test_suffix_tree test_radix_tree \
         test_sparse_vector test_bit_vector \
         test_linked_list test_binary_tree test_graph test_sorting \
         test_stacks test_queues test_hash_table test_fenwick_segment \
         test_bloom_filter test_dinic

.PHONY: all test clean

all: $(TESTS)

test: $(TESTS)
	@for t in $(TESTS); do echo "Running $$t..."; ./$$t; done

test_array_list: tests/test_ch04_array_list.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_linked_list: tests/test_linked_list.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_deque: tests/test_deque.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_stacks: tests/test_stacks.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_queues: tests/test_queues.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_red_black_tree: tests/test_red_black_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_sparse_matrix: tests/test_sparse_matrix.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_suffix_array: tests/test_suffix_array.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_binomial_heap: tests/test_binomial_heap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_fibonacci_heap: tests/test_fibonacci_heap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_pairing_heap: tests/test_pairing_heap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_splay_tree: tests/test_splay_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_two_three_tree: tests/test_two_three_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_suffix_tree: tests/test_suffix_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_radix_tree: tests/test_radix_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_sparse_vector: tests/test_sparse_vector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_bit_vector: tests/test_bit_vector.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_binary_tree: tests/test_binary_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_graph: tests/test_graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_sorting: tests/test_sorting.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_hash_table: tests/test_hash_table.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_fenwick_segment: tests/test_fenwick_segment.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_bloom_filter: tests/test_bloom_filter.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_dinic: tests/test_dinic.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
