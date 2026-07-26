# Code Examples — Data Structures, Algorithms, and Applications in C++

Complete, compilable implementations of all data structures and algorithms covered in the book.

## Structure

```
code/
├── include/           # Header-only libraries
│   ├── array_list.h
│   ├── bit_vector.h
│   ├── binomial_heap.h
│   ├── deque.h
│   ├── fibonacci_heap.h
│   ├── linked_list.h
│   ├── pairing_heap.h
│   ├── queues.h
│   ├── radix_tree.h
│   ├── sparse_matrix.h
│   ├── sparse_vector.h
│   ├── stacks.h
│   ├── splay_tree.h
│   ├── suffix_tree.h
│   ├── two_three_tree.h
│   └── ...
├── tests/             # Unit tests and usage examples
│   ├── test_array_list.cpp
│   ├── test_deque.cpp
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
| `linked_list.h` | Singly linked list with unique_ptr, iterators, reverse |
| `sparse_matrix.h` | Sparse matrix (CSR/COO), transpose, matrix-vector multiply |
| `sparse_vector.h` | Sparse vector (dot, add, sub, scale) |
| `stacks.h` | Array-based & linked stacks |
| `queues.h` | Circular & linked queues |
| `deque.h` | Double-ended deque (circular buffer) |
| `bit_vector.h` | Bit vector (popcount, find_first/next, bitwise ops) |
| `hash_table.h` | Hash table with separate chaining |
| `open_addressing_hash.h` | Open addressing (linear probing, quadratic probing, double hashing) |
| `skip_list.h` | Skip list (randomized ordered structure) |
| `binary_tree.h` | Binary tree traversals, BST insert/erase/find |
| `max_heap.h` | Max-heap & heap sort |
| `binomial_heap.h` | Binomial heap (mergeable) |
| `fibonacci_heap.h` | Fibonacci heap (amortized O(1) decrease-key) |
| `pairing_heap.h` | Pairing heap (simple, practical) |
| `avl_tree.h` | AVL balanced search tree |
| `red_black_tree.h` | Red-black tree (insert, delete, search) |
| `splay_tree.h` | Splay tree (self-adjusting BST) |
| `two_three_tree.h` | 2-3 tree (B-tree variant) |
| `b_tree.h` | B-tree (generalized balanced search tree) |
| `treap.h` | Treap (BST + heap priority) |
| `leftist_tree.h` | Leftist tree (mergeable min-heap with NPL) |
| `suffix_tree.h` | Suffix tree (naive O(n²) construction) |
| `radix_tree.h` | Radix/Patricia trie (compressed) |
| `expression_tree.h` | Expression tree (build, evaluate, infix/postfix) |
| `huffman.h` | Huffman coding tree (in greedy.h) |
| `graph.h` | Adjacency list graph, BFS, DFS, Dijkstra, Bellman-Ford, Floyd-Warshall, topological sort, cycle detection, Kruskal's MST |
| `scc.h` | Strongly connected components (Kosaraju + Tarjan) |
| `prim.h` | Prim's MST (min-heap) |
| `suffix_array.h` | Suffix array, LCP, substring search |
| `fenwick_segment.h` | Fenwick tree, segment tree, union-find |
| `segment_tree_lazy.h` | Segment tree with lazy propagation (range update + range query) |
| `sorting.h` | Insertion, merge, quick sort, quickselect, counting sort, radix sort, bucket sort |
| `bloom_filter.h` | Bloom filter, Count-Min sketch |
| `dinic.h` | Dinic's max-flow algorithm |
| `flow_matching.h` | Ford-Fulkerson, Edmonds-Karp, Hopcroft-Karp bipartite matching |
| `greedy.h` | Fractional knapsack, Huffman coding |
| `divide_conquer.h` | Closest pair of points, Strassen's matrix multiply |
| `dynamic_programming.h` | 0/1 knapsack, LCS, edit distance, LIS |
| `backtracking.h` | N-Queens, subset sum, TSP (bitmask DP) |
| `probabilistic.h` | HyperLogLog, Count-Min sketch, reservoir sampling |

## Notes

- All code uses C++20 features: concepts, `std::span`, `std::string_view`, ranges, `std::unique_ptr`, move semantics.
- No raw `new`/`delete` — RAII throughout.
- Written for readability and teaching, not maximum performance.
