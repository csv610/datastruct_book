class n_queens_bitmask {
public:
    explicit n_queens_bitmask(int n) : n_(n), count_(0) {}

    int solve_count() {
        count_ = 0;
        backtrack(0, 0, 0, 0);
        return count_;
    }

    std::vector<std::vector<int>> solve_all() {
        solutions_.clear();
        std::vector<int> cols(n_);
        backtrack_all(0, 0, 0, 0, cols);
        return solutions_;
    }

private:
    void backtrack(int row, int cols, int diag1, int diag2) {
        if (row == n_) {
            ++count_;
            return;
        }
        int available = ((1 << n_) - 1) & ~(cols | diag1 | diag2);
        while (available) {
            int bit = available & (-available);
            available &= available - 1;
            backtrack(row + 1, cols | bit,
                      (diag1 | bit) << 1, (diag2 | bit) >> 1);
        }
    }

    void backtrack_all(int row, int cols, int diag1, int diag2,
                       std::vector<int>& assignment) {
        if (row == n_) {
            solutions_.push_back(assignment);
            return;
        }
        int available = ((1 << n_) - 1) & ~(cols | diag1 | diag2);
        while (available) {
            int bit = available & (-available);
            available &= available - 1;
            int col = __builtin_ctz(bit);
            assignment[row] = col;
            backtrack_all(row + 1, cols | bit,
                          (diag1 | bit) << 1, (diag2 | bit) >> 1,
                          assignment);
        }
    }

    int n_;
    int count_;
    std::vector<std::vector<int>> solutions_;
};

// Example usage:
// n_queens_bitmask solver(8);
// std::cout << "8-Queens solutions: " << solver.solve_count() << "\n";
// Output: 8-Queens solutions: 92
