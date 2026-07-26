# Code Examples — Data Structures, Algorithms, and Applications in C++

Complete, compilable implementations of all data structures and algorithms covered in the book.

## Structure

```
code/
├── include/           # Header-only libraries
│   ├── ch04_array_list.h
│   ├── ch05_linked_list.h
│   ├── ch08_stacks.h
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
| `ch08_stacks.h` | Array-based & linked stacks |
| `ch09_queues.h` | Circular & linked queues |
| `ch10_hash_table.h` | Hash table with separate chaining |
| `ch11_binary_tree.h` | Binary tree traversals |
| `ch12_max_heap.h` | Max-heap & heap sort |
| `ch15_avl_tree.h` | AVL balanced search tree |
| `ch16_graph.h` | Adjacency list graph, BFS, DFS, topological sort |
| `ch17_fenwick_segment.h` | Fenwick tree, segment tree, union-find |
| `ch19_bloom_filter.h` | Bloom filter, Count-Min sketch |
| `ch20_dinic.h` | Dinic's max-flow algorithm |
| `ch21_sorting.h` | Insertion, merge, quick, heap, introsort |
| `ch22_string_algorithms.h` | KMP, Rabin-Karp, suffix array, LCP |

## Notes

- All code uses C++20 features: concepts, `std::span`, `std::string_view`, ranges, `std::unique_ptr`, move semantics.
- No raw `new`/`delete` — RAII throughout.
- Written for readability and teaching, not maximum performance.
