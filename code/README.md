# Code Examples — Data Structures, Algorithms, and Applications in C++ (3rd Ed.)

This directory contains complete, compilable implementations of all data structures and algorithms covered in the book.

## Requirements

- **C++20** compiler (GCC 11+, Clang 14+, Apple Clang 15+, MSVC 2022 17+)
- **Make** (GNU Make or equivalent)

## Building

```bash
make          # Build all examples
make test     # Build and run all tests
make clean    # Remove build artifacts
```

To build a single test:

```bash
make test_ch04_array_list && ./test_ch04_array_list
make test_ch05_linked_list && ./test_ch05_linked_list
```

## Structure

Each header file (`ch*.h`) implements one or more data structures or algorithms:

| File | Covers |
|------|--------|
| `ch04_array_list.h` | Array-based list (dynamic array) |
| `ch05_linked_list.h` | Singly/doubly linked lists |
| `ch08_stacks.h` | Array-based and linked stacks |
| `ch09_queues.h` | Circular queue, linked queue |
| `ch10_hash_table.h` | Hash table with separate chaining |
| `ch11_binary_tree.h` | Binary tree traversals |
| `ch12_max_heap.h` | Max-heap and heap sort |
| `ch15_avl_tree.h` | AVL balanced search tree |
| `ch16_graph.h` | Adjacency list graph, BFS, DFS, topological sort |
| `ch17_fenwick_segment.h` | Fenwick tree, segment tree, union-find |
| `ch19_bloom_filter.h` | Bloom filter, Count-Min Sketch |
| `ch20_dinic.h` | Dinic max-flow algorithm |
| `ch21_sorting.h` | Insertion, merge, quick, heap, introsort |
| `ch22_string_algorithms.h` | KMP, Rabin-Karp, suffix array, LCP |

Test files (`test_ch*.cpp`) contain usage examples and unit tests.

## Notes

- All code uses C++20 features: concepts, `std::span`, `std::string_view`, ranges, `std::unique_ptr`, move semantics.
- No raw `new`/`delete` is used anywhere — RAII throughout.
- The code is written for readability and teaching, not maximum performance.
