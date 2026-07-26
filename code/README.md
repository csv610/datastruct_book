# Code Examples — Data Structures, Algorithms, and Applications in C++

Complete, compilable implementations of all data structures and algorithms covered in the book.

## Structure

```
code/
├── include/           # Header-only libraries
│   ├── ch04_array_list.h
│   ├── ch05_linked_list.h
│   ├── ch05_sparse_matrix.h
│   ├── ch06_stacks.h
│   ├── ch07_queues.h
│   ├── ch07_deque.h
│   └── ...
├── tests/             # Unit tests and usage examples
│   ├── ch01_test.cpp
│   ├── ch02_test.cpp
│   └── ...
├── CMakeLists.txt
├── Makefile
└── README.md
```

## Requirements

- **C++20** compiler (GCC 11+, Clang 14+, Apple Clang 15+, MSVC 2022 17+)
- **CMake** 3.16+ (recommended) or **GNU Make**

## Building

### Using Make

```bash
cd code
make          # Build all tests
make test     # Build and run all tests
make clean    # Remove build artifacts
```

### Using CMake

```bash
cd code
mkdir build && cd build
cmake ..
cmake --build .
ctest
```

## Headers

| Header | Data Structures / Algorithms |
|--------|------------------------------|
| `ch04_array_list.h` | Dynamic array list |
| `ch05_linked_list.h` | Singly & doubly linked lists |
| `ch05_sparse_matrix.h` | Sparse matrix (CSR/COO), transpose, matrix-vector multiply |
| `ch06_stacks.h` | Array-based & linked stacks |
| `ch07_queues.h` | Circular & linked queues |
| `ch07_deque.h` | Double-ended deque (circular buffer) |
| `ch08_hash_table.h` | Hash table with separate chaining |
| `ch09_binary_tree.h` | Binary tree traversals |
| `ch10_max_heap.h` | Max-heap & heap sort |
| `ch11_avl_tree.h` | AVL balanced search tree |
| `ch11_red_black_tree.h` | Red-black tree (insert, delete, search) |
| `ch12_graph.h` | Adjacency list graph, BFS, DFS, topological sort |
| `ch13_suffix_array.h` | Suffix array, LCP, substring search |
| `ch14_fenwick_segment.h` | Fenwick tree, segment tree, union-find |
| `ch16_sorting.h` | Insertion, merge, quick, heap, introsort |
| `ch19_bloom_filter.h` | Bloom filter, Count-Min sketch |
| `ch20_dinic.h` | Dinic's max-flow algorithm |

## Notes

- All code uses C++20 features: concepts, `std::span`, `std::string_view`, ranges, `std::unique_ptr`, move semantics.
- No raw `new`/`delete` — RAII throughout.
- Written for readability and teaching, not maximum performance.
