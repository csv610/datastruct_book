#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cstdlib>

// ----------------------------------------------------------------
// N-Queens — print all solutions
// ----------------------------------------------------------------
class n_queens {
public:
    n_queens(int n) : n_(n), cols_(n) {}

    std::vector<std::vector<int>> solve() {
        solutions_.clear();
        backtrack(0);
        return solutions_;
    }

    static void print_solution(const std::vector<int>& cols) {
        int n = static_cast<int>(cols.size());
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                std::cout << (cols[r] == c ? "Q " : ". ");
            }
            std::cout << "\n";
        }
        std::cout << "\n";
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
            }
        }
    }

    bool is_safe(int row, int col) {
        for (int i = 0; i < row; ++i) {
            if (cols_[i] == col) return false;
            if (std::abs(cols_[i] - col) == row - i) return false;
        }
        return true;
    }

    int n_;
    std::vector<int> cols_;
    std::vector<std::vector<int>> solutions_;
};

// ----------------------------------------------------------------
// Subset Sum — with pruning
// ----------------------------------------------------------------
class subset_sum {
public:
    subset_sum(const std::vector<int>& values, int target)
        : values_(values), target_(target) {
        std::sort(values_.begin(), values_.end());
    }

    std::vector<std::vector<int>> solve() {
        solutions_.clear();
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

        int remaining = std::accumulate(values_.begin() + i, values_.end(), 0);
        if (sum + remaining < target_) return;

        current.push_back(values_[i]);
        backtrack(i + 1, sum + values_[i], current);
        current.pop_back();

        backtrack(i + 1, sum, current);
    }

    std::vector<int> values_;
    int target_;
    std::vector<std::vector<int>> solutions_;
};

// ----------------------------------------------------------------
// Permutation Generation
// ----------------------------------------------------------------
void generate_permutations(std::vector<int>& arr, size_t start) {
    if (start == arr.size()) {
        for (size_t i = 0; i < arr.size(); ++i) {
            std::cout << arr[i] << (i + 1 < arr.size() ? " " : "");
        }
        std::cout << "\n";
        return;
    }
    for (size_t i = start; i < arr.size(); ++i) {
        std::swap(arr[start], arr[i]);
        generate_permutations(arr, start + 1);
        std::swap(arr[start], arr[i]);
    }
}

// ----------------------------------------------------------------
// Sudoku Solver
// ----------------------------------------------------------------
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
                            board[row][col] = 0;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    static bool is_valid(const std::vector<std::vector<int>>& board,
                          int row, int col, int num) {
        for (int i = 0; i < 9; ++i) {
            if (board[row][i] == num) return false;
            if (board[i][col] == num) return false;
        }
        int br = 3 * (row / 3);
        int bc = 3 * (col / 3);
        for (int r = br; r < br + 3; ++r) {
            for (int c = bc; c < bc + 3; ++c) {
                if (board[r][c] == num) return false;
            }
        }
        return true;
    }

    static void print_board(const std::vector<std::vector<int>>& board) {
        for (int r = 0; r < 9; ++r) {
            if (r % 3 == 0 && r != 0)
                std::cout << "------+-------+------\n";
            for (int c = 0; c < 9; ++c) {
                if (c % 3 == 0 && c != 0) std::cout << "| ";
                if (board[r][c] == 0)
                    std::cout << ". ";
                else
                    std::cout << board[r][c] << " ";
            }
            std::cout << "\n";
        }
    }
};

// ----------------------------------------------------------------
// main — demonstrations
// ----------------------------------------------------------------
int main() {
    std::cout << "=== N-Queens (n=4) ===\n";
    n_queens nq(4);
    auto queens_solutions = nq.solve();
    std::cout << "Found " << queens_solutions.size() << " solutions:\n\n";
    for (const auto& sol : queens_solutions) {
        n_queens::print_solution(sol);
    }

    std::cout << "=== Subset Sum ===\n";
    std::vector<int> ss_vals = {3, 1, 4, 1, 5};
    int target = 7;
    subset_sum ss(ss_vals, target);
    auto ss_solutions = ss.solve();
    std::cout << "Values: [3, 1, 4, 1, 5], target=" << target << "\n";
    std::cout << "Solutions (" << ss_solutions.size() << "):\n";
    for (const auto& sol : ss_solutions) {
        std::cout << "  { ";
        for (int x : sol) std::cout << x << " ";
        std::cout << "}\n";
    }

    std::cout << "\n=== Permutations of {1, 2, 3} ===\n";
    std::vector<int> perm = {1, 2, 3};
    generate_permutations(perm, 0);

    std::cout << "\n=== Sudoku Solver ===\n";
    std::vector<std::vector<int>> board = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    std::cout << "Input:\n";
    sudoku_solver::print_board(board);
    sudoku_solver solver;
    if (solver.solve(board)) {
        std::cout << "\nSolved:\n";
        sudoku_solver::print_board(board);
    } else {
        std::cout << "\nNo solution exists.\n";
    }

    return 0;
}
