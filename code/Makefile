CXX      := c++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -Iinclude

TESTS := test_array_list test_deque test_red_black_tree test_sparse_matrix test_suffix_array \
         test_binomial_heap test_fibonacci_heap test_pairing_heap \
         test_splay_tree test_two_three_tree test_suffix_tree test_radix_tree \
         test_sparse_vector test_bit_vector \
         test_linked_list test_binary_tree test_graph test_sorting \
         test_stacks test_queues test_hash_table test_fenwick_segment \
         test_bloom_filter test_dinic \
         test_greedy test_divide_conquer test_dynamic_programming test_backtracking \
         test_probabilistic test_flow_matching \
         test_open_addressing_hash test_skip_list test_expression_tree \
         test_leftist_tree test_b_tree test_treap test_scc test_prim \
         test_segment_tree_lazy \
         test_string_search test_approximate_matching test_string_compression \
         test_bwt test_lsh test_de_bruijn \
         test_exact_matching test_text_decomposition test_multiple_matching \
         test_additional_algorithms test_suffix_array_search \
         test_new_sorting test_dary_heap test_indexed_pq test_new_graph \
         test_gabow_scc test_nfa test_hungarian

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

test_greedy: tests/test_greedy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_divide_conquer: tests/test_divide_conquer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_dynamic_programming: tests/test_dynamic_programming.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_backtracking: tests/test_backtracking.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_probabilistic: tests/test_probabilistic.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_flow_matching: tests/test_flow_matching.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_open_addressing_hash: tests/test_open_addressing_hash.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_skip_list: tests/test_skip_list.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_expression_tree: tests/test_expression_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_leftist_tree: tests/test_leftist_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_b_tree: tests/test_b_tree.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_treap: tests/test_treap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_scc: tests/test_scc.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_prim: tests/test_prim.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_segment_tree_lazy: tests/test_segment_tree_lazy.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_string_search: tests/test_string_search.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_approximate_matching: tests/test_approximate_matching.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_string_compression: tests/test_string_compression.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_bwt: tests/test_bwt.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_lsh: tests/test_lsh.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_de_bruijn: tests/test_de_bruijn.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_exact_matching: tests/test_exact_matching.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_text_decomposition: tests/test_text_decomposition.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_multiple_matching: tests/test_multiple_matching.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_additional_algorithms: tests/test_additional_algorithms.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_suffix_array_search: tests/test_suffix_array_search.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_new_sorting: tests/test_new_sorting.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_dary_heap: tests/test_dary_heap.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_indexed_pq: tests/test_indexed_pq.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_new_graph: tests/test_new_graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_gabow_scc: tests/test_gabow_scc.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_nfa: tests/test_nfa.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_hungarian: tests/test_hungarian.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
