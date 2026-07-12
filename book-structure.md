# Data Structures, Algorithms, and Applications in C++
## Third Edition — Book Structure

### Part I: Preliminaries
1. Modern C++ Review
2. Performance Analysis
3. Asymptotic Notation
4. Performance Measurement of Programs

### Part II: Data Structures
5. Linear Lists — Array Representation
6. Linear Lists — Linked Representation
7. Arrays, Matrices, and Sparse Representations
8. Stacks
9. Queues
10. Hashing and Hash Tables
11. Trees and Binary Trees
12. Priority Queues
13. Binary Search Trees and Balanced Search Trees
14. Graphs and Graph Algorithms
15. String Algorithms and Tries
16. Segment Trees, Fenwick Trees, and Union-Find

### Part III: Algorithm-Design Methods
17. The Greedy Method
18. Divide and Conquer
19. Dynamic Programming
20. Backtracking
21. Branch and Bound

### Part IV: Advanced Topics
22. Probabilistic Data Structures and Streaming Algorithms
23. Maximum Flow and Matching
24. Spatial Data Structures
25. Introduction to Parallel Algorithms

### Appendices
A. C++ STL Quick Reference
B. Complexity Classes and NP-Completeness
C. Mathematical Review

---

### Design Principles

**Style (matching Sahni's original):**
- Complete, compilable C++ code for every data structure
- Mathematical rigor: formal definitions, theorems, complexity proofs
- Step-by-step algorithm trace with concrete examples
- Real-world applications for each structure
- "Your Own Data Type" pattern — generic, reusable implementations
- Gradual disclosure: array → linked → optimized
- Extensive exercises at 3 levels (drill → application → research)
- STL connections: after implementing manually, show the STL equivalent

**C++ Standard: C++20**
- RAII throughout (no raw new/delete)
- Smart pointers for linked structures
- Concepts for generic programming
- `std::span`, `std::string_view` for efficient interfaces
- Ranges and views where natural
- Coroutines for lazy generators (advanced sections)
- `constexpr` where applicable

**New to this edition:**
- Complete C++20 modernisation
- String algorithms (tries, KMP, suffix arrays)
- Segment trees, Fenwick trees, union-find with full analysis
- Probabilistic data structures (Bloom filters, HyperLogLog)
- Max flow, min cut, bipartite matching
- Spatial data structures (k-d trees, quadtrees)
- Performance measurement with Google Benchmark
- Online/streaming algorithms
- Every chapter updated with modern applications
