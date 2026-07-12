# Preface

The study of data structures and algorithms is fundamental to computer science and engineering. A mastery of these areas is essential for developing computer programs that utilize machine resources in an effective manner. Consequently, every computer science and engineering curriculum includes one or more courses devoted to these subjects.

*Data Structures, Algorithms, and Applications in C++*, Third Edition, has been developed for use in programs that cover this material in a unified course as well as in programs that spread the study over two or more courses.

## What's New in This Edition

The first edition (2000) helped define how data structures are taught to a generation of computer scientists. The second edition (2004) added STL integration and improved coverage. Twenty-five years later, C++ has evolved substantially — from C++11 through C++14, C++17, and C++20 — and so has the landscape of algorithm engineering.

This Third Edition is a thorough modernization:

**Modern C++ (C++20).** Every line of code has been rewritten. The previous editions used C++98 with raw `new` and `delete`, manual memory management, and no move semantics. This edition uses:
- **RAII** throughout — resource acquisition is initialization
- **Smart pointers** (`std::unique_ptr`, `std::shared_ptr`) for all linked structures
- **Concepts** for type-safe generic programming
- **`std::span` and `std::string_view`** for efficient, non-owning views
- **Move semantics** and perfect forwarding for efficient resource transfer
- **Ranges and views** (C++20) for composable data transformations

No raw `new`/`delete` appears anywhere in this book.

**Expanded coverage.** Five new chapters:
- **Strings and Tries** (Chapter 13): tries (R-way, ternary, compressed), Knuth-Morris-Pratt, Rabin-Karp, suffix arrays, LCP arrays
- **Segment Trees, Fenwick Trees, and Union-Find** (Chapter 14): three essential range-query and connectivity structures
- **Probabilistic Data Structures** (Chapter 19): Bloom filters, Count-Min Sketch, HyperLogLog, reservoir sampling
- **Maximum Flow and Matching** (Chapter 20): Ford-Fulkerson, Dinic, min-cut, bipartite matching
- **Spatial Data Structures and Parallel Algorithms** (Chapters 21–22): k-d trees, quadtrees, SIMD, task parallelism, GPU fundamentals

**Streamlined coverage.** Backtracking and branch and bound (web-only chapters in the 2nd edition) are now consolidated into a single chapter. Skip lists are covered within the hashing discussion. The C++ review chapter has been completely rewritten for C++20. LZW compression is an application example rather than a core section. Simulated pointers and available-space lists have been removed — modern C++ provides better alternatives. Tournament trees and threaded binary trees, which occupied significant space in the 2nd edition, have been removed — they have little relevance in modern practice.

## Organization

The book is divided into four parts:

**Part I: Preliminaries** (Chapters 1–4). Reviews the C++20 features used throughout the book, introduces performance analysis and asymptotic notation, and covers performance measurement methodology. These chapters ensure all readers share a common foundation.

**Part II: Data Structures** (Chapters 5–16). Presents the core data structures in a progression from simple to complex. Each chapter develops one or more C++ implementations, analyzes complexity, applies the structure to real problems, and closes with the STL equivalent. The topics span linear lists (array and linked representations), arrays and matrices, stacks and queues, hashing, trees, priority queues, search trees, graphs, strings, segment trees, and union-find.

**Part III: Algorithm-Design Methods** (Chapters 17–20). Covers the four major design paradigms: greedy, divide and conquer, dynamic programming, and backtracking with branch and bound. Each chapter presents the general method, proves its correctness conditions, and works through multiple applications.

**Part IV: Advanced Topics** (Chapters 21–22). Covers probabilistic data structures for streaming data, maximum flow and matching, spatial data structures for geometric problems, and an introduction to parallel algorithms.

**Appendices** provide a C++ STL quick reference, a survey of complexity classes and NP-completeness, and a mathematical review.

## Features

Every chapter follows a consistent structure:

- **Bird's-eye view** — a motivating overview of the chapter's content
- **Complete C++ code** — every data structure and algorithm is presented as a compilable class or function
- **Step-by-step traces** — algorithms are walked through on concrete data
- **Complexity analysis** — precise asymptotic bounds with proofs where appropriate
- **Applications** — one or more real-world problems solved using the chapter's material
- **STL connections** — after developing a hand-crafted implementation, we show the equivalent standard library components
- **Summary** — key points recapped
- **Exercises** — organized into three levels: *Drill* (basic reinforcement), *Application* (implement, modify, benchmark), and *Research* (read classic papers, implement advanced variants, prove theorems)
- **References and further reading** — classic papers and authoritative textbooks

## How to Use This Book

The book supports multiple course structures. Two common approaches:

**One-semester data structures course** (14 weeks): Cover Chapters 1–4 (review, 1 week), Chapters 5–7 (linear lists, 2 weeks), Chapters 8–10 (stacks, queues, hashing, 2 weeks), Chapters 11–12 (trees, search trees, 2 weeks), Chapter 14–15 (graphs, greedy, 2 weeks), Chapter 16–17 (divide and conquer, DP, 2 weeks), Chapter 18 (backtracking, 1 week), with selected advanced topics from Chapters 19–20 as time permits.

**Two-semester sequence**: The first semester covers Chapters 1–14 (preliminaries, all data structures). The second semester covers Chapters 15–22 (algorithm-design methods and advanced topics).

Instructors should assign programming projects that require students to implement data structures from the code listings and apply them to the application problems. The exercises at the application and research levels make suitable project assignments.

## Code and Resources

All code from this book is available at the companion website. The code is organized by chapter and compiles with:

- GCC 12+ with `-std=c++20 -O2 -Wall -Wextra`
- Clang 16+ with `-std=c++20 -O2 -Wall -Wextra`
- MSVC 2022 17.6+ with `/std:c++20 /O2 /W4`

The companion website also provides:
- Complete test harnesses for every implementation
- Benchmarking scripts using Google Benchmark
- Sample input data for application problems
- Slides for instructors
- Solutions to selected exercises

## Acknowledgments

Sartaj Sahni's original books shaped the education of countless computer scientists. The first edition of *Data Structures, Algorithms, and Applications in C++* was a landmark text that brought together rigorous theory, complete code, and real applications in a way no book had before. This third edition strives to honor that legacy while bringing the material fully into the twenty-first century.

I am grateful to the many readers, instructors, and students who have provided feedback on earlier editions. Their observations have guided every decision in this revision.

---

*The only way to learn data structures is to implement them. The only way to learn algorithms is to trace them. This book gives you the code, the examples, and the exercises. The rest is up to you.*
