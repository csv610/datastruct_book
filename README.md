# Data Structures, Algorithms, and Applications in C++

**Third Edition — A Modern, C++20 Approach**

A comprehensive textbook covering fundamental and advanced data structures, algorithm-design paradigms, and modern C++ programming practices. This repository contains the full book content (chapters, TeX sources) and complete, compilable C++ code examples for every data structure and algorithm presented.

---

## Repository Structure

```
DataStruct/
├── chapters/               # Book chapters in Markdown
├── code/                   # Complete C++ implementations and tests
│   ├── *.h                 # Header-only data structure implementations
│   ├── ch*/                # Per-chapter code examples
│   ├── test_*.cpp          # Unit tests and usage examples
│   ├── standalone/         # Self-contained chapter programs
│   └── build/              # CMake build artifacts
├── tex/                    # LaTeX source files
│   ├── book.tex            # Master document
│   ├── chapters/           # Per-chapter .tex files
│   └── book.pdf            # Compiled PDF
├── books/                  # Reference PDFs
├── figures/                # Diagrams and illustrations
├── standalone/             # Standalone C++ programs per chapter
├── book-structure.md       # Detailed book structure
├── content.md              # Full table of contents
└── modern_cpp_data_structures_blueprint.md  # Author blueprint
```

## Book Contents

### Part I — Preliminaries
| Ch | Title | Topics |
|----|-------|--------|
| 1 | Modern C++ Review | RAII, smart pointers, concepts, move semantics, ranges, coroutines |
| 2 | Performance Analysis | Asymptotic notation, recursive analysis, amortized analysis, space complexity |
| 3 | Performance Measurement | `std::chrono`, Google Benchmark, profiling, memory measurement |

### Part II — Data Structures
| Ch | Title | Topics |
|----|-------|--------|
| 4 | Linear Lists | Array-based and linked lists, polynomial arithmetic |
| 5 | Arrays, Matrices & Sparse | Row/column-major, CSR/CSC, PageRank |
| 6 | Stacks | Array/linked stacks, balanced parentheses, postfix evaluation |
| 7 | Queues | Circular buffer, linked queue, deque |
| 8 | Hashing & Hash Tables | Chaining, open addressing, skip lists, Robin Hood, cuckoo, Swiss table |
| 9 | Trees & Binary Trees | Traversals, Huffman coding, expression trees |
| 10 | Priority Queues | Heaps, heap sort, leftist trees, event-driven simulation |
| 11 | Search Trees | BST, AVL, red-black, B-tree, treap |
| 12 | Graphs | Adjacency list/matrix, BFS, DFS, Dijkstra, Bellman-Ford, Floyd-Warshall, SCC |
| 13 | Strings & Tries | Tries, KMP, Rabin-Karp, suffix arrays, LCP |
| 14 | Segment/Fenwick/Union-Find | Range queries, BIT, DSU with inverse Ackermann |

### Part III — Algorithm-Design Methods
| Ch | Title | Topics |
|----|-------|--------|
| 15 | Greedy Method | Fractional knapsack, Huffman, Dijkstra, MST |
| 16 | Divide and Conquer | Merge sort, quick sort, closest pair, Strassen's |
| 17 | Dynamic Programming | 0/1 knapsack, LCS, edit distance, Floyd-Warshall |
| 18 | Backtracking & Branch and Bound | n-Queens, subset sum, TSP, 15-puzzle |
| 19 | Probabilistic Structures | Bloom filters, Count-Min sketch, HyperLogLog, reservoir sampling |
| 20 | Maximum Flow & Matching | Ford-Fulkerson, Edmonds-Karp, Dinic's, bipartite matching |

### Appendices
- **A** — C++ STL Quick Reference
- **B** — Complexity Classes and NP-Completeness
- **C** — Mathematical Review

## Building the Code

### Prerequisites

- **C++20** compiler (GCC 11+, Clang 14+, Apple Clang 15+, or MSVC 2022 17+)
- **CMake** 3.16+ (recommended) or **GNU Make**
- **Google Benchmark** (optional, for Chapter 3 benchmarks)

### Using CMake

```bash
cd code
mkdir build && cd build
cmake ..
cmake --build .
ctest
```

### Using Make

```bash
cd code
make          # Build all examples
make test     # Build and run all tests
make clean    # Remove build artifacts
```

### Running Individual Tests

```bash
cd code
make test_ch04_array_list && ./test_ch04_array_list
make test_ch05_linked_list && ./test_ch05_linked_list
```

## Code Highlights

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

## Design Principles

- **RAII throughout** — no raw `new`/`delete`; smart pointers for all dynamic allocation
- **C++20 features** — concepts, `std::span`, `std::string_view`, ranges, `constexpr`, coroutines
- **Mathematical rigor** — formal definitions, theorems, complexity proofs
- **STL connections** — every manual implementation is compared with its STL equivalent
- **Gradual disclosure** — array → linked → optimized implementations
- **Three-level exercises** — drill, application, and research problems
- **Real-world applications** — each structure includes practical use cases

## Generating the PDF

```bash
cd tex
pdflatex book.tex
makeindex book
pdflatex book.tex
pdflatex book.tex
```

## License

This repository contains the source material for *Data Structures, Algorithms, and Applications in C++ (3rd Edition)*. All rights reserved by the author.

## Contributing

If you find errors in the code or text, please open an issue with:
- The chapter and section number
- A description of the error
- A suggested fix (if applicable)

---

*Built with modern C++20 — because data structures deserve modern tools.*
