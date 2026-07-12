# Backtracking and Branch and Bound

## Introduction

Many problems require searching through a large space of candidates. For NP-hard problems, we cannot avoid exponential worst-case time, but we can often prune the search space dramatically.

**Backtracking** and **branch and bound** are systematic search methods that explore a **state space tree** while pruning branches that cannot lead to a solution.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Backtracking

### The General Method

Backtracking incrementally builds candidates and abandons a candidate ("backtracks") as soon as it determines that the candidate cannot lead to a valid solution.

```
procedure backtrack(x):
    if x is a solution:
        record x
        return
    for each choice c in valid_choices(x):
        make_choice(x, c)
        backtrack(x)
        unmake_choice(x, c)  // backtrack step
```

### The n-Queens Problem

Place n queens on an n×n chessboard so that no two queens attack each other.

**State space:** assign queens to rows 0..n-1, each in column col[i].  
**Constraint:** col[i] ≠ col[j] and |col[i] - col[j]| ≠ |i - j| for all i ≠ j.

```cpp
class n_queens {
public:
    n_queens(int n) : n_(n), cols_(n) {}

    std::vector<std::vector<int>> solve() {
        solutions_.clear();
        backtrack(0);
        return solutions_;
    }

private:
    void backtrack(int row) {
        if (row == n_) {
            solutions_.push_back(cols_);
            return;
        }
        for (int col = 0; col < n_; ++col) {
            if (is_safe(row, col)) {
                cols_[row] = col;
                backtrack(row + 1);
                // no explicit unmake needed — cols_[row] will be overwritten
            }
        }
    }

    bool is_safe(int row, int col) {
        for (int i = 0; i < row; ++i) {
            if (cols_[i] == col) return false;  // same column
            if (std::abs(cols_[i] - col) == row - i) return false;  // diagonal
        }
        return true;
    }

    int n_;
    std::vector<int> cols_;
    std::vector<std::vector<int>> solutions_;
};
```

**Trace for n=4:**

```
Row 0: place queen at col 0
Row 1: col 0 → conflict, col 1 → conflict, col 2 → safe
Row 2: col 0 → conflict (diag with row 1), col 1 → conflict, col 2 → conflict,
       col 3 → conflict → backtrack
Row 1: try col 3 → safe
Row 2: col 0 → conflict (col), col 1 → safe
Row 3: no safe column → backtrack
Row 2: try col 2 → conflict, col 3 → conflict → backtrack
Row 1: no more options → backtrack
Row 0: try col 1
... eventually finds: [1, 3, 0, 2] and [2, 0, 3, 1]
```

**Complexity:** O(n!) worst case, but pruning makes it much faster in practice.

### Subset Sum

Find subsets of a set that sum to a target value.

```cpp
class subset_sum {
public:
    subset_sum(std::span<const int> values, int target)
        : values_(values.begin(), values.end()), target_(target) {
        std::sort(values_.begin(), values_.end());  // enable pruning
    }

    std::vector<std::vector<int>> solve() {
        std::vector<int> current;
        backtrack(0, 0, current);
        return solutions_;
    }

private:
    void backtrack(size_t i, int sum, std::vector<int>& current) {
        if (sum == target_) {
            solutions_.push_back(current);
            return;
        }
        if (i >= values_.size() || sum > target_) return;

        // Prune: if sum + remaining values can't reach target
        int remaining = std::accumulate(values_.begin() + i, values_.end(), 0);
        if (sum + remaining < target_) return;

        // Include values_[i]
        current.push_back(values_[i]);
        backtrack(i + 1, sum + values_[i], current);
        current.pop_back();

        // Exclude values_[i]
        backtrack(i + 1, sum, current);
    }

    std::vector<int> values_;
    int target_;
    std::vector<std::vector<int>> solutions_;
};
```

### Subset Generation (All Subsets)

Generate all subsets of {1, 2, ..., n} using backtracking:

```cpp
void generate_subsets(int n) {
    std::vector<int> subset;
    std::function<void(int)> backtrack = [&](int i) {
        if (i > n) {
            // Process subset
            for (int x : subset) std::cout << x << " ";
            std::cout << "\n";
            return;
        }
        subset.push_back(i);     // include i
        backtrack(i + 1);
        subset.pop_back();        // exclude i
        backtrack(i + 1);
    };
    backtrack(1);
}
```

This generates 2ⁿ subsets, reflecting the backtracking pattern of include/exclude decisions.

### Permutation Generation

```cpp
void generate_permutations(std::vector<int>& arr, size_t start) {
    if (start == arr.size()) {
        for (int x : arr) std::cout << x << " ";
        std::cout << "\n";
        return;
    }
    for (size_t i = start; i < arr.size(); ++i) {
        std::swap(arr[start], arr[i]);
        generate_permutations(arr, start + 1);
        std::swap(arr[start], arr[i]);  // backtrack
    }
}
```

## Branch and Bound

Branch and bound is similar to backtracking but uses a **bound** to prune: we compute an optimistic estimate of the best solution achievable from a given node. If the bound is worse than the current best solution, we prune.

### The General Method

```
procedure branch_and_bound(root):
    best = infinity
    queue = {root}
    while queue is not empty:
        node = queue.pop()
        if node is a complete solution:
            best = min(best, value(node))
        else if bound(node) < best:
            for each child of node:
                queue.push(child)
    return best
```

### Traveling Salesman Problem (TSP)

Find the shortest Hamiltonian cycle in a complete graph. Branch and bound with the **reduced cost matrix** bound:

```cpp
class tsp_solver {
public:
    tsp_solver(std::vector<std::vector<int>> cost_matrix)
        : cost_(std::move(cost_matrix)), n_(cost_.size()), best_cost_(INT_MAX) {}

    int solve() {
        std::vector<bool> visited(n_, false);
        visited[0] = true;
        branch_and_bound(0, 1, 0, visited);
        return best_cost_;
    }

private:
    void branch_and_bound(int current, int count, int current_cost,
                           std::vector<bool>& visited) {
        if (count == n_) {
            current_cost += cost_[current][0];  // return to start
            best_cost_ = std::min(best_cost_, current_cost);
            return;
        }

        for (int next = 0; next < n_; ++next) {
            if (!visited[next]) {
                int new_cost = current_cost + cost_[current][next];
                // Bound: if already worse than best, prune
                if (new_cost < best_cost_) {
                    visited[next] = true;
                    branch_and_bound(next, count + 1, new_cost, visited);
                    visited[next] = false;
                }
            }
        }
    }

    const std::vector<std::vector<int>>& cost_;
    size_t n_;
    int best_cost_;
};
```

**A tighter bound:** compute the reduced cost matrix. For each row, subtract its minimum. For each column, subtract its minimum. The sum of all subtracted values gives a lower bound for any tour.

### 15-Puzzle

Solve the 15-puzzle using branch and bound with the **Manhattan distance** heuristic:

```cpp
struct puzzle_state {
    std::vector<int> board;  // 16 elements, 0 = empty
    int zero_pos;
    int g;  // cost so far (moves made)
    int h;  // heuristic (Manhattan distance)
    
    int f() const { return g + h; }
    
    bool operator>(const puzzle_state& other) const {
        return f() > other.f();
    }
};

int manhattan_distance(const std::vector<int>& board) {
    int distance = 0;
    for (int i = 0; i < 16; ++i) {
        if (board[i] == 0) continue;
        int target_row = (board[i] - 1) / 4;
        int target_col = (board[i] - 1) % 4;
        int row = i / 4;
        int col = i % 4;
        distance += std::abs(row - target_row) + std::abs(col - target_col);
    }
    return distance;
}

// A* search is branch and bound with a priority queue ordered by f = g + h
int solve_15_puzzle(const std::vector<int>& start) {
    std::priority_queue<puzzle_state, std::vector<puzzle_state>,
                        std::greater<puzzle_state>> pq;
    
    int start_zero = std::find(start.begin(), start.end(), 0) - start.begin();
    pq.push({start, start_zero, 0, manhattan_distance(start)});
    
    std::set<std::vector<int>> visited;
    
    while (!pq.empty()) {
        auto state = pq.top(); pq.pop();
        if (state.h == 0) return state.g;  // solved
        
        if (visited.count(state.board)) continue;
        visited.insert(state.board);
        
        // Generate moves (up, down, left, right)
        int row = state.zero_pos / 4;
        int col = state.zero_pos % 4;
        int dirs[][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        
        for (auto [dr, dc] : dirs) {
            int nr = row + dr, nc = col + dc;
            if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4) continue;
            
            auto new_board = state.board;
            int new_pos = nr * 4 + nc;
            std::swap(new_board[state.zero_pos], new_board[new_pos]);
            
            if (!visited.count(new_board)) {
                pq.push({new_board, new_pos, state.g + 1,
                         manhattan_distance(new_board)});
            }
        }
    }
    return -1;  // unsolvable
}
```

This is **A* search** — branch and bound with a priority queue and an admissible heuristic (one that never overestimates the true cost).

### Backtracking vs. Branch and Bound

| | Backtracking | Branch and Bound |
|---|---|---|
| **Goal** | Find all solutions or any solution | Find optimal solution |
| **Pruning** | Constraints (feasibility) | Bounds (optimality) |
| **Search order** | Typically DFS | Typically best-first |
| **Memory** | O(depth) recursion stack | O(branching factor × depth) for priority queue |
| **Typical use** | N-queens, Sudoku, subset sum | TSP, 15-puzzle, integer programming |

## Application: Sudoku Solver

```cpp
class sudoku_solver {
public:
    bool solve(std::vector<std::vector<int>>& board) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board[row][col] == 0) {
                    for (int num = 1; num <= 9; ++num) {
                        if (is_valid(board, row, col, num)) {
                            board[row][col] = num;
                            if (solve(board)) return true;
                            board[row][col] = 0;  // backtrack
                        }
                    }
                    return false;  // no valid number for this cell
                }
            }
        }
        return true;  // all cells filled
    }

    bool is_valid(const std::vector<std::vector<int>>& board,
                  int row, int col, int num) {
        for (int i = 0; i < 9; ++i) {
            if (board[row][i] == num) return false;
            if (board[i][col] == num) return false;
            int box_row = 3 * (row / 3) + i / 3;
            int box_col = 3 * (col / 3) + i % 3;
            if (board[box_row][box_col] == num) return false;
        }
        return true;
    }
};
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Forgetting to restore state after backtracking | State corrupted, solution space polluted | Always undo the assignment when returning from recursion |
| No pruning on unsolvable branches | Exponential time exploring dead ends | Add feasibility check before recursing |
| Infinite recursion if base case is not reached | Stack overflow, crash | Ensure every recursive call makes progress toward a base case |
| Wrong bounding function in branch-and-bound | Prunes optimal solutions, suboptimal result | Bounding function must be admissible (≤ true value for maximization) |
| Using float equality for bound comparison | Wrong bound evaluation | Use tolerance `abs(a - b) < ε` or integer arithmetic |
| Not sorting items before search (e.g., subset sum) | Much slower search, more backtracking | Sort items to enable early pruning |

## Summary

1. **Backtracking** systematically explores the state space tree, pruning branches that violate constraints.
2. **n-Queens, subset sum, and Sudoku** are canonical backtracking problems.
3. **Branch and bound** extends backtracking by pruning based on bounds, searching for optimal solutions.
4. **A* search** is branch and bound with a priority queue and an admissible heuristic.
5. Both methods solve NP-hard problems in practice by pruning most of the exponential search space.

## Exercises

### Drill

1. Draw the state-space tree for 4-Queens. How many nodes are visited by the backtracking algorithm? How many nodes would be visited by brute force (all 4⁴ placements)?

2. For subset sum with values [3, 1, 4, 1, 5] and target = 7, trace the backtracking algorithm. Which subsets are found? Which branches are pruned?

3. For the TSP instance with cost matrix:
   ```
   [∞, 10, 15, 20]
   [10, ∞, 35, 25]
   [15, 35, ∞, 30]
   [20, 25, 30, ∞]
   ```
   Compute the reduced cost matrix bound for the root node. What is the lower bound?

4. What is the size of the state-space tree for:
   a) n-Queens
   b) Subset sum with n elements
   c) TSP with n cities
   d) Graph coloring on a graph with v vertices and k colors

5. When does backtracking degenerate to brute-force search? Give an example problem and instance where no pruning occurs.

### Application

6. Solve the **knight's tour** problem using backtracking on an 8×8 board. Use Warnsdorff's heuristic for the order of moves to speed up the search.

7. Implement **graph coloring** using backtracking. Given a graph and k colors, assign colors to vertices so that no two adjacent vertices share a color. Find the minimum k (chromatic number) for small graphs.

8. Write a **crossword puzzle solver** using backtracking. Given a partially filled crossword grid and a dictionary, fill in all the words.

9. Solve TSP for n up to 15 using branch and bound with the reduced cost matrix bound. Report the solution and the number of nodes visited. Compare with exhaustive search (n!).

10. Implement a **magic square** generator using backtracking. Generate all 3×3 magic squares (where rows, columns, and diagonals sum to the same value).

11. Implement branch and bound for the **0/1 knapsack** problem. Use the fractional knapsack as the upper bound. Show that the bound prunes at least some branches for most instances.

12. Implement the **Hamiltonian cycle** problem using backtracking. Find all Hamiltonian cycles in a small graph (≤ 8 vertices).

13. Write a **Sudoku solver** that can solve any 9×9 puzzle. Count the number of backtracks needed for hard puzzles (e.g., those with 17 clues).

### Research

14. **(SAT solvers)** Read about SAT solvers and the **DPLL algorithm** — a sophisticated backtracking algorithm for Boolean satisfiability. Implement a simple SAT solver and test it on small CNF formulas.

15. **(Constraint satisfaction)** Research the AC-3 algorithm for arc consistency in constraint satisfaction problems. Incorporate it into the n-Queens solver as a preprocessing step. Does it reduce the search space?

16. **(Branch and bound for integer programming)** Read about LP relaxation and branch and bound for integer linear programming. Implement a simple ILP solver for small instances.

17. **(Parallel backtracking)** Implement a parallel n-Queens solver using `std::thread` or C++20 `std::jthread`. Measure the speedup on a multi-core machine. What is the granularity of the parallel work?

## References and Further Reading

- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapter 34 (NP-completeness), Chapter 35 (approximation).
- Steven S. Skiena, *The Algorithm Design Manual*, 2nd Edition. Springer, 2008.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Section 6.4 (backtracking).
- Judea Pearl, *Heuristics: Intelligent Search Strategies for Computer Problem Solving*. Addison-Wesley, 1984.
- Martin Gardner, "Mathematical Games" — Scientific American (columns on n-Queens, magic squares, etc.).
- Donald E. Knuth, "Dancing Links" — Millennial Perspectives in Computer Science, 2000 (exact cover and algorithm X).
- Armin Biere et al. (eds.), *Handbook of Satisfiability*, 2nd Edition. IOS Press, 2021.
