# Dynamic Programming

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The General Method

**Dynamic programming (DP)** solves problems by combining solutions to overlapping subproblems. Unlike divide and conquer (where subproblems are independent), DP handles cases where the same subproblems appear multiple times.

### Key Concepts

- **Optimal substructure:** an optimal solution contains optimal solutions to subproblems
- **Overlapping subproblems:** the same subproblems are solved repeatedly
- **Memoization:** cache subproblem results (top-down)
- **Tabulation:** build the solution iteratively from smallest subproblems (bottom-up)

### DP Design Steps

1. Characterize the structure of an optimal solution
2. Define the value of an optimal solution recursively
3. Compute the value bottom-up
4. Reconstruct the solution

## The 0/1 Knapsack Problem

**Problem:** n items, each with weight wᵢ and value vᵢ. Fill a knapsack of capacity C to maximize value. Each item is taken or left (no fractions).

### Recursive Formulation

Let V(i, c) = maximum value using items 1..i with capacity c.

```
V(i, c) = 
  V(i-1, c)                          if wᵢ > c (cannot take item i)
  max(V(i-1, c), vᵢ + V(i-1, c-wᵢ)) otherwise
V(0, c) = 0
V(i, 0) = 0
```

### Bottom-Up Implementation

```cpp
int knapsack_01(std::span<const int> weights, std::span<const int> values, int capacity) {
    int n = weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= capacity; ++w) {
            if (weights[i - 1] > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = std::max(dp[i - 1][w],
                                    values[i - 1] + dp[i - 1][w - weights[i - 1]]);
            }
        }
    }
    return dp[n][capacity];
}
```

**Complexity:** O(n·C) time and space. Since C can be large, this is pseudo-polynomial.

### Space-Optimized Version

```cpp
int knapsack_01_optimized(std::span<const int> weights,
                           std::span<const int> values, int capacity) {
    std::vector<int> dp(capacity + 1, 0);
    for (size_t i = 0; i < weights.size(); ++i) {
        for (int w = capacity; w >= weights[i]; --w) {
            dp[w] = std::max(dp[w], values[i] + dp[w - weights[i]]);
        }
    }
    return dp[capacity];
}
```

**Space:** O(C). Reason: dp[w] = max(dp_old[w], vᵢ + dp_old[w-wᵢ]). We iterate w backwards so dp[w-wᵢ] is still from the previous iteration.

### Reconstructing the Solution

```cpp
std::vector<size_t> knapsack_items(std::span<const int> weights,
                                    std::span<const int> values, int capacity) {
    int n = weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    // ... fill dp table as above ...

    std::vector<size_t> selected;
    int w = capacity;
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(i - 1);
            w -= weights[i - 1];
        }
    }
    return selected;
}
```

## Matrix Chain Multiplication

**Problem:** Given matrices A₁, A₂, ..., Aₙ with dimensions d₀×d₁, d₁×d₂, ..., dₙ₋₁×dₙ, find the parenthesization that minimizes the total scalar multiplications.

### Recurrence

Let M[i][j] = minimum cost to multiply Aᵢ through Aⱼ.

```
M[i][j] = 0                                   if i = j
          min { M[i][k] + M[k+1][j] + dᵢ₋₁·dₖ·dⱼ }
          i ≤ k < j
```

### Implementation

```cpp
int matrix_chain_order(std::span<const int> dims) {
    int n = dims.size() - 1;  // number of matrices
    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));

    for (int len = 2; len <= n; ++len) {      // chain length
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = m[i][k] + m[k + 1][j]
                         + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < m[i][j]) m[i][j] = cost;
            }
        }
    }
    return m[0][n - 1];
}
```

**Trace for dims = [10, 20, 30, 40, 30] (A:10×20, B:20×30, C:30×40, D:40×30):**

```
m[0][1] = 10×20×30 = 6000        (AB)
m[1][2] = 20×30×40 = 24000       (BC)
m[2][3] = 30×40×30 = 36000       (CD)

m[0][2] = min(
  m[0][0] + m[1][2] + 10×20×40 = 0 + 24000 + 8000 = 32000,
  m[0][1] + m[2][2] + 10×30×40 = 6000 + 0 + 12000 = 18000 → min
)

m[1][3] = min(
  m[1][1] + m[2][3] + 20×30×30 = 0 + 36000 + 18000 = 54000,
  m[1][2] + m[3][3] + 20×40×30 = 24000 + 0 + 24000 = 48000 → min
)

m[0][3] = min(
  k=0: m[0][0] + m[1][3] + 10×20×30 = 0 + 48000 + 6000 = 54000,
  k=1: m[0][1] + m[2][3] + 10×30×30 = 6000 + 36000 + 9000 = 51000 → min
  k=2: m[0][2] + m[3][3] + 10×40×30 = 18000 + 0 + 12000 = 30000 → ACTUAL MIN
)

Optimal: (A × (B × (C × D))) = 30000 multiplications
```

**Complexity:** O(n³) time, O(n²) space.

## All-Pairs Shortest Paths (Floyd-Warshall)

From Chapter 12. The DP formulation:

Let D⁽ᵏ⁾[i][j] = shortest path from i to j using intermediate vertices only from {0, 1, ..., k-1}.

```
D⁽ᵏ⁾[i][j] = min(D⁽ᵏ⁻¹⁾[i][j], D⁽ᵏ⁻¹⁾[i][k-1] + D⁽ᵏ⁻¹⁾[k-1][j])
```

## Longest Common Subsequence (LCS)

**Problem:** Given two strings X and Y, find the longest sequence that appears as a subsequence in both.

### Recurrence

```
LCS[i][j] = longest common subsequence of X[0..i-1] and Y[0..j-1]

LCS[i][j] = 0                                        if i=0 or j=0
            LCS[i-1][j-1] + 1                        if X[i-1] = Y[j-1]
            max(LCS[i-1][j], LCS[i][j-1])            otherwise
```

### Implementation

```cpp
int lcs_length(std::string_view a, std::string_view b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

std::string lcs(std::string_view a, std::string_view b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    // ... fill dp table ...

    // Backtrack
    std::string result;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result.push_back(a[i - 1]);
            --i; --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}
```

**Example:** X = "ABCBDAB", Y = "BDCABA" → LCS = "BCBA" or "BDAB" (length 4).

## Edit Distance (Levenshtein Distance)

**Problem:** Minimum number of insertions, deletions, and substitutions to convert string A into string B.

### Recurrence

```
ED[i][j] = edit distance between A[0..i-1] and B[0..j-1]

ED[i][j] = i                                       if j = 0
           j                                       if i = 0
           ED[i-1][j-1]                            if A[i-1] = B[j-1]
           min(ED[i-1][j] + 1,     // delete
               ED[i][j-1] + 1,     // insert
               ED[i-1][j-1] + 1)   // substitute
```

### Implementation

```cpp
int edit_distance(std::string_view a, std::string_view b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({dp[i - 1][j] + 1,      // delete
                                     dp[i][j - 1] + 1,      // insert
                                     dp[i - 1][j - 1] + 1}); // substitute
            }
        }
    }
    return dp[m][n];
}
```

**Application:** Spell checking, DNA sequence alignment, diff tools.

## Coin Change (Unbounded)

**Problem:** Given coin denominations (unlimited supply), find the minimum number of coins to make a target amount.

### Recurrence

```
dp[c] = min coins needed to make amount c
dp[c] = min over all coins ≤ c of (dp[c - coin] + 1)
dp[0] = 0
```

```cpp
int coin_change(std::span<const int> coins, int amount) {
    std::vector<int> dp(amount + 1, INT_MAX / 2);
    dp[0] = 0;

    for (int c = 1; c <= amount; ++c) {
        for (int coin : coins) {
            if (coin <= c) {
                dp[c] = std::min(dp[c], dp[c - coin] + 1);
            }
        }
    }
    return dp[amount] == INT_MAX / 2 ? -1 : dp[amount];
}
```

## DP vs. Greedy vs. Divide and Conquer

| Criterion | Greedy | Divide & Conquer | Dynamic Programming |
|-----------|--------|------------------|---------------------|
| Subproblems | One | Independent | Overlapping |
| Decision | Locally optimal | Combine solutions | Table of all options |
| Correctness | Greedy choice property | Induction | Optimal substructure |
| Example | Dijkstra, MST | Merge sort, binary search | Knapsack, LCS, Floyd-Warshall |

## Common Bugs and Pitfalls

- **Infinite loops in recurrence** — a recurrence that depends on itself without a base case leads to infinite recursion. Always verify the base case terminates and that subproblems strictly decrease the state space.
- **Off-by-one in table dimensions** — DP tables indexed by string length or array position must handle the empty-prefix case at index 0. Forgetting this causes out-of-bounds access or incorrect base values.
- **Integer overflow in DP values** — filling a knapsack DP with `INT_MAX` and then adding 1 overflows to negative. Use `INT_MAX / 2` as "infinity" and check before adding.
- **Assuming DP is always faster** — for small problem sizes, recursion or brute force can be faster due to DP's overhead from table construction. Profile before optimizing.
- **Forgetting to reconstruct** — the DP table gives the optimal *value*, not the solution itself. Always store the decision in a parallel table if reconstruction is needed (e.g., which items to pack).
- **2D tables for problems that need only the previous row** — many DP problems (01 knapsack, edit distance) only need the previous row. Allocating O(n²) space when O(n) suffices is wasteful and limits scalability.

## Summary

1. **Dynamic programming** solves problems with overlapping subproblems and optimal substructure.
2. **Top-down (memoization)** is often easier to code; **bottom-up (tabulation)** is often more efficient.
3. **Knapsack, matrix chain, LCS, edit distance, and Floyd-Warshall** are canonical DP problems.
4. Industrial applications include **Google's PageRank** (DP on graphs), **spell checkers** (edit distance), **DNA sequence alignment** (Smith-Waterman), **speech recognition** (Viterbi), **robotics path planning**, and **supply chain optimization**.
5. The **key skill** is formulating the recurrence — recognizing the state and the transition.

## Exercises

### Drill

1. For the 0/1 knapsack with items (weight, value) = [(2, 3), (3, 4), (4, 5), (5, 6)] and capacity C = 8, compute the DP table bottom-up. What is the optimal value? Which items are selected?

2. Compute the LCS of "ABCBDAB" and "BDCABA". Show the DP table and trace back to find the LCS.

3. For the matrix chain product with dimensions A₁(10×20), A₂(20×30), A₃(30×40), A₄(40×30), compute the optimal parenthesization using DP. Show the cost table and the split table.

4. Compute the edit distance between "kitten" and "sitting". Show the DP table and trace the optimal alignment.

5. For the coin change problem with coins [1, 2, 5] and amount = 11, compute the DP table. What is the minimum number of coins?

6. State whether each problem has optimal substructure:
   a) Finding the longest path in a graph
   b) Finding the shortest path in a graph (Dijkstra)
   c) Finding the maximum independent set
   d) Finding the longest increasing subsequence

### Application

7. **Supply chain shipping optimization (0/1 knapsack).** A warehouse has trucks with 50-ton capacity. There are 200 pallets with weights (100–1000 kg) and priority values. Implement the bottom-up DP knapsack to select pallets maximizing value. For n = 200 and C = 50,000, optimize memory using the 1D rolling array technique. Benchmark against the 2D version. How much memory does the 1D version save?

8. **DNA sequence alignment (Smith-Waterman).** Implement the Smith-Waterman local alignment algorithm (a variant of edit distance) for DNA sequences. Align two bacterial gene sequences (download from NCBI). Compute the alignment score and visualize the matching regions. Compare with BLAST's heuristic approach.

9. **Viterbi for speech recognition.** Implement the Viterbi algorithm to decode the most likely phoneme sequence from audio features. Given an HMM with 5 states (phonemes), observation probabilities, and transition probabilities, decode the sequence of observations [O₁, O₂, O₃, O₄, O₅]. Show the DP trellis and the most likely state path.

10. **Text justification in Google Docs.** Implement text justification using DP: given words with lengths and a line width of 80 characters, break lines to minimize the sum of (slack)³ where slack = width - used_width. Compare the DP result with greedy word-wrapping. Show that greedy produces worse spacing on a sample paragraph.

11. **Robotic path planning (grid DP).** A warehouse robot (similar to Amazon Robotics drives) must navigate from (0,0) to (m,n) on a grid, moving only right and down, avoiding obstacles. Each cell has a traversal cost (floor wear, battery). Find the minimum-cost path using DP. Test on a 100×100 grid with 20% random obstacles.

12. **OPT binary search tree for compiler symbol tables.** Compilers use optimal BSTs for symbol table lookup. Given keys [A, B, C, D, E] with access frequencies [0.05, 0.15, 0.40, 0.25, 0.15], compute the OBST using DP. Show the cost table and the resulting tree. Compare expected search cost with a skewed BST (sorted insertion) and a balanced tree.

13. **Longest increasing subsequence for stock trading.** Find the longest sequence of days where a stock price increases (longest bull run). Implement O(n²) DP and O(n log n) patience sorting. Test on 20 years of S&P 500 daily close prices (download from Yahoo Finance). How long is the longest increasing subsequence? How much faster is the O(n log n) version for 5000 data points?

14. **Held-Karp TSP for delivery route planning.** Implement the Held-Karp DP algorithm for the traveling salesman problem. Test on 15 randomly generated cities in 2D. Show the DP table (size = n·2ⁿ). For n = 20, how much memory does the DP table require? At what n does the algorithm become infeasible for a typical laptop? Compare with a nearest-neighbor greedy heuristic — how far from optimal is the greedy solution?

### Research

15. **(DP + approximation for knapsack)** The subset sum DP can approximate the knapsack problem with arbitrarily small error. Implement the FPTAS (fully polynomial-time approximation scheme) for knapsack by scaling values. Show that for ε = 0.1, the solution is within 10% of optimal while running in polynomial time.

16. **(DP on trees)** Solve the maximum independent set in a tree using DP. A social network wants to select influencers such that no two connected influencers are selected. Implement tree DP and test on random trees with 1000 nodes. How much faster is tree DP than a general graph algorithm (which would be NP-hard)?

17. **(Floyd-Warshall for network routing)** Implement Floyd-Warshall for all-pairs shortest paths in a network of 1000 routers. Use an adjacency matrix with edge weights representing latency. Compare with running Dijkstra from each vertex. Which is faster for dense graphs? For sparse graphs?

18. **(DP for resource allocation)** Implement the DP for allocating a budget of \$1M across 10 projects, each with a nonlinear return function (diminishing returns). Show the DP table and the optimal allocation. Compare with a greedy allocation by ROI.

## References and Further Reading

- Richard Bellman, *Dynamic Programming*. Princeton University Press, 1957.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapters 14 (DP), 15 (DP).
- Dan Gusfield, *Algorithms on Strings, Trees, and Sequences*. Cambridge, 1997.
- Michael Held and Richard M. Karp, "A Dynamic Programming Approach to Sequencing Problems" — JSIAM, 1962.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Chapter 5 (strings), Chapter 6 (DP).
- Donald E. Knuth, "Optimum Binary Search Trees" — Acta Informatica, 1971.
