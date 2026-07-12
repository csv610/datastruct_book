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
                // no explicit unmake needed -- cols_[row] will be overwritten
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
