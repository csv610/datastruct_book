# Performance Analysis

## Introduction

Why do we need to analyze algorithms? Consider two programs that solve the same problem. One completes in milliseconds; the other takes hours. The difference is not in the hardware or the programming language — it is in the algorithm.

Performance analysis is the theoretical study of an algorithm's time and space requirements as a function of input size. Performance measurement (Chapter 3) is the empirical study of actual running time. Both are essential.

In this chapter we develop the mathematical tools needed to compare algorithms independently of the machine, compiler, or programming language.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## What Is an Algorithm?

An **algorithm** is a finite sequence of unambiguous instructions that, given an input, produces an output and terminates in finite time.

Properties of an algorithm:
1. **Input** — zero or more externally supplied values
2. **Output** — at least one value produced
3. **Definiteness** — each instruction is precise and unambiguous
4. **Finiteness** — the algorithm terminates after a finite number of steps
5. **Effectiveness** — each instruction is basic enough to be performed exactly

A **program** is an algorithm expressed in a programming language. A program may not terminate (e.g., an operating system), but an algorithm always must.

## Measuring Algorithm Efficiency

### Time Complexity

The time taken by a program depends on:
- The input size
- The machine and compiler
- The quality of the generated code
- The algorithm itself

We need a way to talk about efficiency that abstracts away the machine. We count **operations** — comparisons, arithmetic operations, data movements — as a function of **input size**, denoted *n*.

### Space Complexity

Space complexity measures the memory required:
- **Instruction space** — the compiled code
- **Data space** — constants, variables, dynamically allocated memory
- **Stack space** — function call overhead, local variables in recursive functions

## Asymptotic Notation

Asymptotic notation describes the growth rate of functions. We care about behavior as *n* → ∞.

### Big O Notation (O)

**Definition:** f(n) = O(g(n)) if there exist positive constants c and n₀ such that f(n) ≤ c·g(n) for all n ≥ n₀.

Big O gives an **upper bound** on the growth rate.

*Example:* f(n) = 3n² + 2n + 1 is O(n²) because 3n² + 2n + 1 ≤ 4n² for all n ≥ 3.

Common Big O classes, from slowest to fastest growth:

| Notation | Name | Example |
|----------|------|---------|
| O(1) | Constant | Array access |
| O(log n) | Logarithmic | Binary search |
| O(n) | Linear | Finding maximum |
| O(n log n) | Linearithmic | Merge sort |
| O(n²) | Quadratic | Bubble sort |
| O(n³) | Cubic | Matrix multiplication |
| O(2ⁿ) | Exponential | Subset generation |
| O(n!) | Factorial | Permutations |

### Big Omega Notation (Ω)

**Definition:** f(n) = Ω(g(n)) if there exist positive constants c and n₀ such that f(n) ≥ c·g(n) for all n ≥ n₀.

Big Omega gives a **lower bound**.

*Example:* f(n) = 3n² + 2n + 1 is Ω(n²) because 3n² + 2n + 1 ≥ 3n² for all n ≥ 0.

### Big Theta Notation (Θ)

**Definition:** f(n) = Θ(g(n)) if f(n) = O(g(n)) and f(n) = Ω(g(n)).

Big Theta gives a **tight bound** — the function grows at the same rate as g(n).

*Example:* f(n) = 3n² + 2n + 1 is Θ(n²).

### Little o and Little ω

**Little o:** f(n) = o(g(n)) if for every positive constant c, there exists n₀ such that f(n) < c·g(n) for all n ≥ n₀. (Strict upper bound — f grows strictly slower than g.)

**Little ω:** f(n) = ω(g(n)) if for every positive constant c, there exists n₀ such that f(n) > c·g(n) for all n ≥ n₀. (Strict lower bound.)

## Practical Complexity

### Growth Rates Compared

Consider algorithms with different time complexities on a machine executing 10⁸ operations per second:

| n | O(log n) | O(n) | O(n log n) | O(n²) | O(2ⁿ) |
|---|----------|------|------------|-------|-------|
| 10 | 0.00 µs | 0.10 µs | 0.33 µs | 1.00 µs | 10.2 µs |
| 100 | 0.01 µs | 1.00 µs | 6.64 µs | 100 µs | 4×10¹⁶ years |
| 1000 | 0.01 µs | 10.0 µs | 99.7 µs | 10.0 ms | — |
| 10⁶ | 0.02 µs | 10.0 ms | 199 ms | 2.78 hours | — |
| 10⁹ | 0.03 µs | 10.0 s | 299 s | 317 years | — |

### Which Complexity Matters?

An O(n²) algorithm that takes 1 second for n = 1000 will take 100 seconds for n = 10000. An O(n log n) algorithm that takes 1 second for n = 1000 will take about 10 seconds for n = 10000 — a linear degradation, not quadratic.

**Key insight:** The asymptotic growth rate dominates all constant factors for sufficiently large n.

## Analyzing Iterative Algorithms

### Simple Loop

```cpp
// Find the maximum element
int find_max(const std::vector<int>& a) {
    int max = a[0];                          // 1
    for (size_t i = 1; i < a.size(); ++i) {  // n-1 iterations
        if (a[i] > max) max = a[i];          // 2 ops per iteration
    }
    return max;
}
```

Operations: 1 + 2(n-1) = 2n - 1 = O(n)

### Nested Loops

```cpp
// All pairs
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
        process(a[i], a[j]);   // O(1) operation
    }
}
```

Inner loop executes n times for each of n outer iterations: O(n²).

### Dependent Nested Loops

```cpp
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
        process(a[i], a[j]);
    }
}
```

Total iterations: 0 + 1 + 2 + ... + (n-1) = n(n-1)/2 = O(n²).

### Logarithmic Loops

```cpp
for (int i = 1; i < n; i *= 2) {
    process(i);
}
```

i takes values 1, 2, 4, 8, ..., 2^k where 2^k < n. So k = ⌊log₂ n⌋ iterations: O(log n).

## Analyzing Recursive Algorithms

### Recurrence Relations

Recursive algorithms lead to recurrence relations. The recurrence describes the running time in terms of the running time on smaller inputs.

**Example: Factorial**

```cpp
int factorial(int n) {
    if (n <= 1) return 1;          // O(1)
    return n * factorial(n - 1);   // T(n-1) + O(1)
}
```

Recurrence: T(n) = T(n-1) + c, T(1) = d.

Solution: T(n) = c(n-1) + d = O(n).

**Example: Binary Search**

```cpp
int binary_search(const std::vector<int>& a, int target, int low, int high) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (a[mid] == target) return mid;
    if (a[mid] > target) return binary_search(a, target, low, mid - 1);
    return binary_search(a, target, mid + 1, high);
}
```

Recurrence: T(n) = T(n/2) + c, T(1) = d.

Solution: T(n) = c·log₂ n + d = O(log n).

### Solving Recurrences

**Method 1: Substitution**

Guess the form of the solution and prove by induction.

For T(n) = 2T(n/2) + n, T(1) = 1:
- Guess T(n) = O(n log n)
- Assume T(k) ≤ ck log k for k < n
- Then T(n) ≤ 2c(n/2)log(n/2) + n = cn log(n/2) + n = cn log n - cn log 2 + n = cn log n - cn + n
- Choose c ≥ 1: T(n) ≤ cn log n ✓

**Method 2: Recursion Tree**

Draw the tree of recursive calls, sum the cost at each level.

For T(n) = 2T(n/2) + n:
```
          n                 ← level 0, 1 node
        /   \
      n/2   n/2             ← level 1, 2 nodes
     /  \   /  \
    n/4 n/4 n/4 n/4         ← level 2, 4 nodes
   ...  ...  ...  ...
```
Level i has 2ⁱ nodes, each costing n/2ⁱ. Total per level: n.
Height: log₂ n levels. Total: n·log₂ n.

**Method 3: Master Theorem**

For recurrences of the form T(n) = aT(n/b) + f(n), where a ≥ 1, b > 1:

| Case | Condition | Solution |
|------|-----------|----------|
| 1 | f(n) = O(n^(log_b a - ε)) for some ε > 0 | T(n) = Θ(n^(log_b a)) |
| 2 | f(n) = Θ(n^(log_b a) · log^k n) | T(n) = Θ(n^(log_b a) · log^(k+1) n) |
| 3 | f(n) = Ω(n^(log_b a + ε)) and af(n/b) ≤ cf(n) for some c < 1 | T(n) = Θ(f(n)) |

*Examples:*
- T(n) = 2T(n/2) + n: a=2, b=2, log_b a = 1, f(n)=n = Θ(n^1) → Case 2 → T(n) = Θ(n log n)
- T(n) = T(n/2) + 1: a=1, b=2, log_b a = 0, f(n)=1 = Θ(n^0) → Case 2 → T(n) = Θ(log n)
- T(n) = 9T(n/3) + n: a=9, b=3, log_b a = 2, f(n)=n = O(n^(2-ε)) → Case 1 → T(n) = Θ(n²)

## Amortized Analysis

Sometimes a single operation is expensive, but the average cost over a sequence of operations is low. **Amortized analysis** bounds the average cost per operation.

### Aggregate Method

We compute the total cost of n operations and divide by n.

**Example: Dynamic array (std::vector push_back)**

When the array is full, we double its capacity and copy all elements. A push_back that triggers a resize costs O(k) where k is the current size. But most push_back operations cost O(1).

Total cost for n insertions into an initially empty array that doubles at capacity:
- Capacities: 1, 2, 4, 8, ..., 2^(⌈log₂ n⌉)
- Copy cost: 1 + 2 + 4 + ... + 2^(⌈log₂ n⌉) < 2n
- n insertions cost at most 3n (n inserts at unit cost + 2n for copies)
- Amortized cost per insertion: O(1)

### Accounting Method

We charge more for cheap operations and use the surplus to pay for expensive ones.

For the dynamic array: charge 3 for each push_back. Each regular insertion costs 1; the remaining 2 are saved. When a resize copies k elements, the saved credits from the k elements pay for it.

### Potential Method

Define a potential function Φ(state) that measures how close the data structure is to needing an expensive operation. The amortized cost is actual cost + ΔΦ.

For the dynamic array: let Φ = 2·size - capacity when size ≥ capacity/2, and 0 otherwise.

## Space Complexity

### Fixed vs. Variable Space

- **Fixed space:** constants, program code — independent of input size
- **Variable space:** depends on input size — arrays, dynamic allocations, recursion stack

Total space = fixed + variable.

### Analyzing Space

```cpp
// O(1) extra space
int sum(const std::vector<int>& a) {
    int total = 0;
    for (int x : a) total += x;
    return total;
}

// O(n) extra space
std::vector<int> duplicate(const std::vector<int>& a) {
    std::vector<int> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = a[i];
    return result;
}

// O(n) stack space (recursive)
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);  // n stack frames
}
```

## Best, Average, and Worst Case

- **Best case** — minimum time over all inputs of size n
- **Worst case** — maximum time over all inputs of size n
- **Average case** — expected time over all inputs of size n (requires a probability distribution)

*Example: Linear search in an array of size n*
- Best case: element is first → O(1)
- Worst case: element is last or absent → O(n)
- Average case (assuming uniform distribution, element present): (n+1)/2 → O(n)

We primarily study worst-case complexity in this book. It provides a guarantee, not a hope.

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Confusing worst-case with average-case | Over-engineering for rare inputs | State which case you're analyzing |
| Ignoring constant factors | O(n²) algorithm beats O(n log n) for n ≤ 1000 | Measure, don't just analyze |
| Misapplying the Master Theorem | Wrong asymptotic bound | Verify recurrence matches one of the three cases exactly |
| Forgetting the recursion stack in space analysis | Underestimating memory by O(n) | Count stack frames for recursive algorithms |
| Assuming `std::vector::push_back` is always O(1) | Surprising latency spikes on reallocation | Use `reserve()` when size is known |
| Confusing Big-O (upper bound) with Big-Θ (tight bound) | Imprecise claims about algorithm behavior | Use Θ when the bound is tight, O for upper bound only |
| Analyzing worst-case when amortized matters | Pessimistic design decisions | Use amortized analysis for sequences of operations |

## Summary

1. **Asymptotic notation** (O, Ω, Θ, o, ω) describes growth rates independently of machine constants.
2. **Iterative algorithms** are analyzed by summing loop iterations.
3. **Recursive algorithms** lead to recurrences, solved by substitution, recursion trees, or the Master Theorem.
4. **Amortized analysis** measures the average cost of operations over a sequence.
5. **Space complexity** includes fixed and variable components, plus the recursion stack.
6. **Worst-case analysis** provides performance guarantees.

## Exercises

### Drill

1. **Index vs table scan.** A database can search an index in O(log n) reads or scan the whole table in O(n) reads. For a table of 10^6 rows, the index should win. But the index = 800 ms and the scan = 12 ms. What does Big-O notation ignore that explains this? (Hint: think about sequential vs random I/O.)

   *(In production, databases like PostgreSQL use cost-based query planning that weighs disk access patterns, not just operation counts.)*

2. **Blocking vs non-blocking.** A database operation called KEYS has O(n) cost and blocks all other operations while running. Another operation called SCAN has O(1) per call and O(n) total, but does not block others. A colleague says "they're both O(n), they're the same." Why is this reasoning wrong?

3. **Hash table growing pains.** An engineer replaces a balanced tree (O(log n) lookup) with a hash table (O(1) average lookup), expecting faster code. Throughput drops. What hidden cost of the hash table does Big-O notation hide? Think about rehashing, memory allocation, and cache misses.

   *(In production, Google's `absl::flat_hash_map` and similar libraries optimize the growth strategy to minimize this overhead.)*

4. **Compound index.** A database index on `(city, last_seen)` should answer "find all rows for Seattle in the last hour" in O(log n) time. The actual query takes much longer. If "Seattle" matches 10,000 rows and "last hour" spans only a few of them, why does the index still scan many pages?

5. **Stack overflow.** A serverless function uses a recursive algorithm whose depth equals the input size. Code review says "each call is O(1), so total is O(n)." Yet the function crashes on large inputs. What aspect of complexity analysis was overlooked?

### Application

6. **Write buffer amortization.** An in-memory write buffer accepts inserts (each O(log m) where m is buffer size). When the buffer reaches capacity, it's flushed to disk (cost O(m)). Show that the amortized cost per insert is O(log n) despite the occasional O(m) flush. Implement a simulation that counts operations over 10^6 inserts.

   *(In production, LevelDB and RocksDB use this exact "memtable" pattern.)*

7. **Vector growth cost.** A dynamic array (like `std::vector`) doubles its capacity each time it runs out of space. Analyze the total number of element copies over n insertions. Show that the average (amortized) cost is O(1) per insertion despite occasional O(n) reallocations. Implement a simulation that counts copies for n = 10^6 and verify the bound. What happens with growth factor 1.5 vs 2?

   *(In production, Chromium's rendering engine carefully manages vector growth to minimize page load jank.)*

8. **Merging sorted segments.** A background process merges m sorted segments containing n total items. The standard merge has recurrence T(n) = 2T(n/2) + O(n). Solve this using the Master Theorem. Implement a simulator and verify the total operations. How does the complexity change if the merge step is O(n²)?

   *(In production, Kafka's log compaction uses this pattern to merge sorted message segments.)*

9. **Shortest path in a road network.** Dijkstra's algorithm with a binary heap has complexity O((V+E) log V). For a road network with V = 10^6 intersections and E = 2.5 × 10^6 roads, estimate the heap operations. Implement Dijkstra with a binary heap and with a simpler array-based approach for a dense subgraph. Compare actual operation counts.

   *(In production, Google Maps uses optimized variants of Dijkstra with heuristics like A* and contraction hierarchies.)*

10. **Batched allocation.** A server allocates resources in batches — when the pool is empty, it creates 10 new ones at once instead of 1 at a time. Using amortized analysis (the accounting method), assign a "token cost" to each allocation. Show that the average cost per resource is O(1) despite occasional bursts. Implement a simulation.

    *(In production, NGINX and other web servers use batched connection allocation to smooth out latency spikes.)*

### Research

11. **Cache complexity.** Standard Big-O analysis ignores the memory hierarchy. Analyze a blocked (tiled) matrix multiplication: count how many cache misses occur for n × n matrices with a cache of size Z. Show the complexity is O(n³ / √Z) cache misses. Implement and measure L1/L2/L3 misses using `perf stat` for n = 512, 1024, 2048.

12. **Real-time guarantees.** A real-time control system (e.g., a robot) cannot tolerate occasional O(n) reallocation spikes from `std::vector::push_back`. Design a data structure that guarantees O(1) per operation at the cost of higher average memory. Implement it and measure worst-case vs `std::vector`. Compare P99 and P99.9 latency.

13. **External sorting.** When data exceeds available RAM, sorting must happen on disk. An external sort creates sorted runs of size M (memory limit) and then merges them. Analyze the I/O complexity: how many disk blocks are read and written? Show that increasing M (more memory) reduces I/O significantly. Implement a simulation with various memory budgets.

14. **Consensus message complexity.** A distributed consensus algorithm requires multiple rounds of messages between nodes. In the naive version, every node talks to every other node — O(n²) messages. An optimized version elects a leader, reducing this to O(n) messages. Analyze the message cost for committing one log entry in a 5-node cluster and a 50-node cluster. Implement a message-counting simulation.

## References and Further Reading

- Alfred V. Aho, John E. Hopcroft, and Jeffrey D. Ullman, *The Design and Analysis of Computer Algorithms*. Addison-Wesley, 1974.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. MIT Press, 2022.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Addison-Wesley, 2011.
- Donald E. Knuth, *The Art of Computer Programming*, Volumes 1–4A. Addison-Wesley.
- Daniel H. Greene and Donald E. Knuth, *Mathematics for the Analysis of Algorithms*, 3rd Edition. Birkhäuser, 1990.
- Michael Sipser, *Introduction to the Theory of Computation*, 3rd Edition. Cengage, 2012.
- Steven S. Skiena, *The Algorithm Design Manual*, 2nd Edition. Springer, 2008.
- Jon Kleinberg and Éva Tardos, *Algorithm Design*. Addison-Wesley, 2005.
- Sanjoy Dasgupta, Christos H. Papadimitriou, and Umesh Vazirani, *Algorithms*. McGraw-Hill, 2006.
