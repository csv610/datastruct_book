# Contents

## Preface

---

## Part I — Preliminaries

### Chapter 1 Modern C++ Review
1.1 Introduction
1.2 Functions and Parameters
&emsp;1.2.1 Pass by Value, Reference, and Const Reference
&emsp;1.2.2 Default Arguments
&emsp;1.2.3 Function Overloading
&emsp;1.2.4 Lambda Expressions
&emsp;1.2.5 constexpr and consteval
1.3 Dynamic Memory and RAII
&emsp;1.3.1 The Problem with Raw Pointers
&emsp;1.3.2 Smart Pointers (C++11)
&emsp;1.3.3 RAII (Resource Acquisition Is Initialization)
1.4 Templates and Concepts
&emsp;1.4.1 Function Templates
&emsp;1.4.2 Class Templates
&emsp;1.4.3 Template Template Parameters and Variadic Templates
&emsp;1.4.4 Concepts (C++20)
1.5 Move Semantics
&emsp;1.5.1 Lvalues and Rvalues
&emsp;1.5.2 Move Constructor and Move Assignment
&emsp;1.5.3 Rule of Five
1.6 std::vector and std::span
&emsp;1.6.1 std::vector
&emsp;1.6.2 std::span (C++20)
&emsp;1.6.3 std::string_view (C++17)
1.7 Ranges and Views (C++20)
1.8 Error Handling
&emsp;1.8.1 Exceptions
&emsp;1.8.2 noexcept
&emsp;1.8.3 std::optional (C++17)
1.9 Iterators and Ranges
&emsp;1.9.1 Iterator Categories
&emsp;1.9.2 Range-Based for Loop
&emsp;1.9.3 Iterator Adapters
1.10 Type Deduction: auto and decltype
1.11 Structured Bindings (C++17)
1.12 Coroutines (C++20)
1.13 Common Patterns in This Book
1.14 Compiler and Build

### Chapter 2 Performance Analysis
2.1 Introduction
2.2 What Is an Algorithm?
2.3 Measuring Algorithm Efficiency
2.4 Asymptotic Notation
&emsp;2.4.1 Big-Oh Notation (O)
&emsp;2.4.2 Omega Notation (Ω)
&emsp;2.4.3 Theta Notation (Θ)
&emsp;2.4.4 Little-Oh and Little-Omega
2.5 Practical Complexities
2.6 Analyzing Iterative Algorithms
&emsp;2.6.1 Simple Loops
&emsp;2.6.2 Nested Loops
&emsp;2.6.3 Sequential and Conditional Statements
2.7 Analyzing Recursive Algorithms
&emsp;2.7.1 The Substitution Method
&emsp;2.7.2 The Recursion-Tree Method
&emsp;2.7.3 The Master Theorem
2.8 Amortized Analysis
&emsp;2.8.1 Aggregate Analysis
&emsp;2.8.2 The Accounting Method
&emsp;2.8.3 The Potential Method
2.9 Space Complexity
&emsp;2.9.1 Fixed and Variable Components
&emsp;2.9.2 Recursion Stack Depth
2.10 Best, Average, and Worst Case

### Chapter 3 Performance Measurement of Programs
3.1 Introduction
3.2 Why Measure?
3.3 A First Measurement: std::chrono
3.4 Google Benchmark Library
&emsp;3.4.1 Basic Benchmark
&emsp;3.4.2 Parameterized Benchmarks
&emsp;3.4.3 Preventing Optimization
3.5 Factors Affecting Performance
&emsp;3.5.1 Hardware: CPU, Memory, Cache
&emsp;3.5.2 Compiler Optimizations
&emsp;3.5.3 Input Characteristics
3.6 Case Study: Sorting Algorithms
3.7 Profiling
3.8 Measuring Memory Usage
3.9 Practical Measurement Methodology
3.10 Common Pitfalls

---

## Part II — Data Structures

### Chapter 4 Linear Lists
4.1 Array-Based Lists
&emsp;4.1.1 The Linear List Abstract Data Type
&emsp;4.1.2 Array Representation
&emsp;4.1.3 Dynamic Array (Resizable Array)
&emsp;4.1.4 Complexity Analysis
&emsp;4.1.5 Application: Polynomial Arithmetic
&emsp;4.1.6 Application: Image Editor Layer Stack
&emsp;4.1.7 STL Connection: std::vector
4.2 Linked Lists
&emsp;4.2.1 Singly Linked List (Chain)
&emsp;4.2.2 Complexity Analysis
&emsp;4.2.3 Doubly Linked List
&emsp;4.2.4 Circular Linked Lists
&emsp;4.2.5 Application: Polynomial Using Linked List
&emsp;4.2.6 Application: Undo/Redo System
&emsp;4.2.7 Memory Overhead
&emsp;4.2.8 STL Connection: std::forward_list and std::list

### Chapter 5 Arrays, Matrices, and Sparse Representations
5.1 Introduction
5.2 Multi-Dimensional Arrays
&emsp;5.2.1 Row- and Column-Major Mapping
&emsp;5.2.2 Array of Arrays vs. Flat Array
5.3 Special Matrices
&emsp;5.3.1 Diagonal Matrices
&emsp;5.3.2 Tridiagonal Matrices
&emsp;5.3.3 Triangular Matrices
&emsp;5.3.4 Symmetric Matrices
5.4 Sparse Matrices
&emsp;5.4.1 Coordinate (COO) Format
&emsp;5.4.2 Compressed Sparse Row (CSR)
&emsp;5.4.3 Compressed Sparse Column (CSC)
&emsp;5.4.4 Matrix-Vector Multiplication
5.5 Application: PageRank
5.6 Application: Image Processing
5.7 STL Connection: std::mdspan (C++23)

### Chapter 6 Stacks
6.1 The Stack ADT
&emsp;6.1.1 Operations
&emsp;6.1.2 Applications
6.2 Array-Based Stack
6.3 Linked Stack
6.4 Applications
&emsp;6.4.1 Balanced Parentheses
&emsp;6.4.2 Postfix Evaluation
&emsp;6.4.3 Tower of Hanoi
6.5 STL Connection: std::stack

### Chapter 7 Queues
7.1 The Queue ADT
&emsp;7.1.1 Operations
&emsp;7.1.2 Applications
7.2 Array-Based Queue (Circular Buffer)
7.3 Linked Queue
7.4 Applications
&emsp;7.4.1 Wire Routing
&emsp;7.4.2 Task Scheduling
7.5 Deque (Double-Ended Queue)
7.6 STL Connection: std::queue

### Chapter 8 Hashing and Hash Tables
8.1 The Dictionary ADT
8.2 Hash Tables: Basic Idea
8.3 Hash Functions
&emsp;8.3.1 Division Method
&emsp;8.3.2 Multiplication Method
&emsp;8.3.3 Universal Hashing
&emsp;8.3.4 Hash Functions for Strings
8.4 Separate Chaining
&emsp;8.4.1 Implementation
&emsp;8.4.2 Rehashing
8.5 Open Addressing
&emsp;8.5.1 Linear Probing
&emsp;8.5.2 Quadratic Probing
&emsp;8.5.3 Double Hashing
8.6 Skip Lists
&emsp;8.6.1 Structure
&emsp;8.6.2 Node Structure
&emsp;8.6.3 Search
&emsp;8.6.4 Insertion
&emsp;8.6.5 Choosing the Level
&emsp;8.6.6 Complexity
&emsp;8.6.7 Skip Lists vs. Hash Tables vs. Balanced Trees
&emsp;8.6.8 STL Connection
8.7 Modern Hash Table Designs
&emsp;8.7.1 Robin Hood Hashing
&emsp;8.7.2 Cuckoo Hashing
&emsp;8.7.3 Swiss Table (Google's Abseil)
8.8 Complexity Analysis
8.9 Comparison of Dictionaries
8.10 Application: LZW Compression
8.11 STL Connection: std::unordered_map

### Chapter 9 Trees and Binary Trees
9.1 Introduction
9.2 Terminology
&emsp;9.2.1 Root, Parent, Child, Leaf
&emsp;9.2.2 Height, Depth, Level
9.3 Binary Trees
&emsp;9.3.1 Properties of Binary Trees
&emsp;9.3.2 Full, Complete, and Perfect Binary Trees
&emsp;9.3.3 Tree Traversals: Preorder, Inorder, Postorder, Level-Order
9.4 Array Representation of Binary Trees
9.5 Applications of Binary Trees
&emsp;9.5.1 Expression Trees
&emsp;9.5.2 Huffman Coding Trees
&emsp;9.5.3 Binary Search Trees (Preview)
9.6 Union-Find (Disjoint Set Union)
&emsp;9.6.1 Quick-Find
&emsp;9.6.2 Quick-Union
&emsp;9.6.3 Union by Rank and Path Compression
&emsp;9.6.4 Complexity Analysis
9.7 Reconstructing Binary Trees from Traversals

### Chapter 10 Priority Queues
10.1 The Priority Queue ADT
10.2 Heaps
&emsp;10.2.1 Definition and Properties
&emsp;10.2.2 Insertion (Sift-Up)
&emsp;10.2.3 Deletion (Sift-Down)
10.3 Heap Initialization (Heapify)
10.4 Heap Sort
&emsp;10.4.1 Algorithm
&emsp;10.4.2 Complexity and Comparison with Other Sorts
10.5 Min-Heaps
10.6 Leftist Trees (Mergeable Heaps)
&emsp;10.6.1 Height-Biased Leftist Trees
&emsp;10.6.2 Operations: Merge, Insert, Delete
10.7 Application: Event-Driven Simulation
10.8 Application: Median Maintenance
10.9 STL Connection: std::priority_queue

### Chapter 11 Binary Search Trees and Balanced Search Trees
11.1 Binary Search Trees
&emsp;11.1.1 Definition and Properties
&emsp;11.1.2 Searching
&emsp;11.1.3 Insertion
&emsp;11.1.4 Deletion
&emsp;11.1.5 Complexity of BST Operations
11.2 AVL Trees
&emsp;11.2.1 Definition and Balance Factor
&emsp;11.2.2 Rotations: LL, RR, LR, RL
&emsp;11.2.3 Insertion with Rebalancing
&emsp;11.2.4 Deletion with Rebalancing
&emsp;11.2.5 Complexity Analysis
11.3 Red-Black Trees
&emsp;11.3.1 Definition and Properties
&emsp;11.3.2 Insertion (Top-Down and Bottom-Up)
&emsp;11.3.3 Deletion
&emsp;11.3.4 Comparison with AVL Trees
11.4 B-Trees
&emsp;11.4.1 Definition (Order m)
&emsp;11.4.2 Searching
&emsp;11.4.3 Insertion and Splitting
&emsp;11.4.4 Deletion and Merging
&emsp;11.4.5 B+ Trees (Sketch)
11.5 Treaps (Randomized BST)
&emsp;11.5.1 Definition: BST + Heap
&emsp;11.5.2 Insert, Delete, Search
&emsp;11.5.3 Expected Complexity
11.6 Comparison of Search Tree Implementations
11.7 STL Connection: std::map and std::set

### Chapter 12 Graphs and Graph Algorithms
12.1 The Graph ADT
&emsp;12.1.1 Directed vs. Undirected
&emsp;12.1.2 Weighted vs. Unweighted
12.2 Graph Classes
&emsp;12.2.1 Adjacency Matrix
&emsp;12.2.2 Adjacency List
12.3 Graph Traversal
&emsp;12.3.1 Depth-First Search (DFS)
&emsp;12.3.2 Breadth-First Search (BFS)
&emsp;12.3.3 Applications: Connected Components, Cycle Detection
12.4 Topological Sort
12.5 Minimum Spanning Tree
&emsp;12.5.1 Kruskal's Algorithm
&emsp;12.5.2 Prim's Algorithm
12.6 Shortest Paths
&emsp;12.6.1 Dijkstra's Algorithm
&emsp;12.6.2 Bellman-Ford Algorithm
&emsp;12.6.3 Floyd-Warshall Algorithm
12.7 Strongly Connected Components
&emsp;12.7.1 Kosaraju's Algorithm
&emsp;12.7.2 Tarjan's Algorithm
12.8 Application: Social Network Analysis
12.9 Complexity Summary

### Chapter 13 String Algorithms and Tries
13.1 Tries
&emsp;13.1.1 Standard Trie
&emsp;13.1.2 R-Way Trie Implementation
&emsp;13.1.3 Ternary Search Trie
&emsp;13.1.4 Compressed Trie (Radix Tree / Patricia Trie)
13.2 String Matching
&emsp;13.2.1 Naive Algorithm
&emsp;13.2.2 Knuth-Morris-Pratt (KMP)
&emsp;13.2.3 Rabin-Karp (Rolling Hash)
&emsp;13.2.4 Boyer-Moore (Sketch)
13.3 Suffix Arrays and LCP Arrays
&emsp;13.3.1 Definition
&emsp;13.3.2 Naive Construction (O(n² log n))
&emsp;13.3.3 Doubling Algorithm (O(n log n))
&emsp;13.3.4 LCP Array Construction (Kasai's Algorithm)
&emsp;13.3.5 Applications: Pattern Matching, Longest Repeated Substring
13.4 Application: Longest Repeated Substring
13.5 Application: Genome Pattern Search

### Chapter 14 Segment Trees, Fenwick Trees, and Union-Find
14.1 Segment Tree
&emsp;14.1.1 Structure
&emsp;14.1.2 Building
&emsp;14.1.3 Range Query
&emsp;14.1.4 Point Update
&emsp;14.1.5 Lazy Propagation (Range Updates)
14.2 Fenwick Tree (Binary Indexed Tree)
&emsp;14.2.1 Structure and Idea
&emsp;14.2.2 Prefix Sum and Point Update
&emsp;14.2.3 Range Sum
&emsp;14.2.4 Lower Bound Search
&emsp;14.2.5 2D Fenwick Tree
14.3 Union-Find (Disjoint Set Union)
&emsp;14.3.1 Quick-Find and Quick-Union
&emsp;14.3.2 Union by Rank
&emsp;14.3.3 Path Compression
&emsp;14.3.4 Inverse Ackermann Complexity
&emsp;14.3.5 Application: Percolation
14.4 Comparison of Range-Query Structures

### Chapter 15 The Greedy Method
15.1 The General Method
&emsp;15.1.1 Greedy Algorithm Template
&emsp;15.1.2 When Greedy Works
15.2 Container Loading
15.3 The Fractional Knapsack Problem
15.4 Task Scheduling
15.5 Single-Source Shortest Paths (Dijkstra)
15.6 Minimum Spanning Tree
15.7 Huffman Coding
15.8 Greedy Does Not Always Work
&emsp;15.8.1 The 0/1 Knapsack Counterexample
&emsp;15.8.2 Coin Change Counterexample

### Chapter 16 Divide and Conquer
16.1 The General Method
16.2 Defective Chessboard
16.3 Binary Search
16.4 Finding Min and Max
16.5 Sorting Algorithms
&emsp;16.5.1 Merge Sort
&emsp;16.5.2 Quick Sort
&emsp;16.5.3 Comparison
16.6 Selection (Finding the k-th Smallest)
&emsp;16.6.1 Quickselect (Randomized)
&emsp;16.6.2 Median of Medians (Deterministic)
16.7 Closest Pair of Points
16.8 Strassen's Matrix Multiplication

### Chapter 17 Dynamic Programming
17.1 The General Method
&emsp;17.1.1 Optimal Substructure
&emsp;17.1.2 Overlapping Subproblems
&emsp;17.1.3 Top-Down vs. Bottom-Up
17.2 The 0/1 Knapsack Problem
17.3 Matrix Chain Multiplication
17.4 All-Pairs Shortest Paths (Floyd-Warshall)
17.5 Longest Common Subsequence (LCS)
17.6 Edit Distance (Levenshtein Distance)
17.7 Coin Change (Unbounded)
17.8 DP vs. Greedy vs. Divide and Conquer

### Chapter 18 Backtracking and Branch and Bound
18.1 Introduction
18.2 Backtracking
&emsp;18.2.1 The General Method
&emsp;18.2.2 The n-Queens Problem
&emsp;18.2.3 Subset Sum
&emsp;18.2.4 Subset Generation
&emsp;18.2.5 Permutation Generation
18.3 Branch and Bound
&emsp;18.3.1 The General Method
&emsp;18.3.2 Traveling Salesman Problem (TSP)
&emsp;18.3.3 The 15-Puzzle
&emsp;18.3.4 Backtracking vs. Branch and Bound
18.4 Application: Sudoku Solver

### Chapter 19 Probabilistic Data Structures
19.1 Introduction
19.2 Bloom Filters
&emsp;19.2.1 Operations
&emsp;19.2.2 False Positive Analysis
&emsp;19.2.3 Optimal Number of Hash Functions
&emsp;19.2.4 Application: Spell-Checker
19.3 Count-Min Sketch
&emsp;19.3.1 Structure
&emsp;19.3.2 Point Query
&emsp;19.3.3 Heavy Hitters
19.4 HyperLogLog
&emsp;19.4.1 The Idea: Probabilistic Counting
&emsp;19.4.2 Structure and Operations
&emsp;19.4.3 Error Analysis
&emsp;19.4.4 Application: Distinct IP Addresses
19.5 Reservoir Sampling

### Chapter 20 Maximum Flow and Matching
20.1 The Maximum Flow Problem
&emsp;20.1.1 Flow Networks
&emsp;20.1.2 The Max-Flow Min-Cut Theorem
20.2 Ford-Fulkerson Algorithm
&emsp;20.2.1 Residual Networks
&emsp;20.2.2 Augmenting Paths
&emsp;20.2.3 Complexity and Limitations
&emsp;20.2.4 Edmonds-Karp (BFS)
20.3 Dinic's Algorithm
&emsp;20.3.1 Level Graph
&emsp;20.3.2 Blocking Flow
&emsp;20.3.3 Complexity Analysis
20.4 Minimum Cut
20.5 Bipartite Matching
20.6 Application: Project Selection

---

## Appendices

### Appendix A C++ STL Quick Reference
A.1 Containers
A.2 Algorithms (in \<algorithm\>)
A.3 Numeric Algorithms (in \<numeric\>)
A.4 Iterators
A.5 Utility Components

### Appendix B Complexity Classes and NP-Completeness
B.1 Decision Problems and Languages
B.2 The Class P
B.3 The Class NP
B.4 NP-Completeness and Cook-Levin Theorem
B.5 Common NP-Complete Problems
B.6 Coping with NP-Completeness

### Appendix C Mathematical Review
C.1 Summation Formulas
C.2 Logarithms
C.3 Combinatorics
C.4 Important Inequalities
C.5 Useful Constants

---

## References
