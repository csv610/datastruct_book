cmake_minimum_required(VERSION 3.16)
project(DataStructBookCode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++23 /O2")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    find_program(CLANGTIDY_PATH NAMES clang-tidy clang-tidy-14 clang-tidy-13 clang-tidy-12 clang-tidy-11)
    if(CLANGTIDY_PATH)
        message(STATUS "Found clang-tidy: ${CLANGTIDY_PATH}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY_PATH};--config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;-quiet")
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${CLANGTIDY_PATH}")
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

set(TESTS
    test_array_list
    test_deque
    test_red_black_tree
    test_sparse_matrix
    test_suffix_array
    test_binomial_heap
    test_fibonacci_heap
    test_pairing_heap
    test_splay_tree
    test_two_three_tree
    test_suffix_tree
    test_radix_tree
    test_sparse_vector
    test_bit_vector
    test_linked_list
    test_binary_tree
    test_graph
    test_sorting
    test_stacks
    test_queues
    test_hash_table
    test_fenwick_segment
    test_bloom_filter
    test_dinic
    test_greedy
    test_divide_conquer
    test_dynamic_programming
    test_backtracking
    test_probabilistic
    test_flow_matching
    test_open_addressing_hash
    test_skip_list
    test_expression_tree
    test_leftist_tree
    test_b_tree
    test_treap
    test_scc
    test_prim
    test_segment_tree_lazy
    test_string_search
    test_approximate_matching
    test_string_compression
    test_bwt
    test_lsh
    test_de_bruijn
    test_exact_matching
    test_text_decomposition
    test_multiple_matching
    test_additional_algorithms
    test_suffix_array_search
    test_new_sorting
    test_dary_heap
    test_indexed_pq
    test_new_graph
    test_gabow_scc
    test_nfa
    test_hungarian
    test_graph_advanced
    test_min_cost_flow
    test_persistent_segment_tree
    test_cuckoo_filter
    test_heavy_hitters
    test_wavelet_tree
    test_union_find
    test_interval_tree
    test_kd_tree
    test_skew_heap
    test_min_max_heap
    test_orthogonal_range_tree
    test_bplus_tree
    test_two_three_four_tree
    test_ab_tree
    test_veb_tree
    test_doubly_linked_list
    test_scapegoat_tree
    test_aa_tree
    test_llrb_tree
    test_r_tree
    test_quadtree
    test_x_fast_trie
    test_interval_heap
    test_merkle_tree
)

foreach(T ${TESTS})
    if(T STREQUAL "test_array_list")
        add_executable(${T} tests/test_ch04_array_list.cpp)
    else()
        add_executable(${T} tests/${T}.cpp)
    endif()
    add_test(NAME ${T} COMMAND ${T})
endforeach()
