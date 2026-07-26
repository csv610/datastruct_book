# Code Examples — Data Structures, Algorithms, and Applications in C++

Complete, compilable implementations of all data structures and algorithms covered in the book.

## Structure

```
code/
├── include/           # Header-only libraries
│   ├── array_list.h
│   ├── linked_list.h
│   ├── sparse_matrix.h
│   ├── stacks.h
│   ├── queues.h
│   ├── deque.h
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
| `array_list.h` | Dynamic array list |
| `linked_list.h` | Singly & doubly linked lists |
| `sparse_matrix.h` | Sparse matrix (CSR/COO), transpose, matrix-vector multiply |
| `stacks.h` | Array-based & linked stacks |
| `queues.h` | Circular & linked queues |
| `deque.h` | Double-ended deque (circular buffer) |
| `hash_table.h` | Hash table with separate chaining |
| `binary_tree.h` | Binary tree traversals |
| `max_heap.h` | Max-heap & heap sort |
| `avl_tree.h` | AVL balanced search tree |
| `red_black_tree.h` | Red-black tree (insert, delete, search) |
| `graph.h` | Adjacency list graph, BFS, DFS, topological sort |
| `suffix_array.h` | Suffix array, LCP, substring search |
| `fenwick_segment.h` | Fenwick tree, segment tree, union-find |
| `sorting.h` | Insertion, merge, quick, heap, introsort |
| `bloom_filter.h` | Bloom filter, Count-Min sketch |
| `dinic.h` | Dinic's max-flow algorithm |

## Notes

- All code uses C++20 features: concepts, `std::span`, `std::string_view`, ranges, `std::unique_ptr`, move semantics.
- No raw `new`/`delete` — RAII throughout.
- Written for readability and teaching, not maximum performance.
