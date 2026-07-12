class sudoku_solver {
public:
    bool solve(std::vector<std::vector<int>>& board) {
        int row, col;
        if (!find_empty(board, row, col)) return true;

        for (int num = 1; num <= 9; ++num) {
            if (is_valid(board, row, col, num)) {
                board[row][col] = num;
                if (solve(board)) return true;
                board[row][col] = 0;  // backtrack
            }
        }
        return false;  // trigger backtracking
    }

private:
    bool find_empty(const std::vector<std::vector<int>>& board,
                    int& row, int& col) {
        for (row = 0; row < 9; ++row) {
            for (col = 0; col < 9; ++col) {
                if (board[row][col] == 0) return true;
            }
        }
        return false;
    }

    bool is_valid(const std::vector<std::vector<int>>& board,
                  int row, int col, int num) {
        for (int c = 0; c < 9; ++c) {
            if (board[row][c] == num) return false;
        }
        for (int r = 0; r < 9; ++r) {
            if (board[r][col] == num) return false;
        }
        int box_row = 3 * (row / 3);
        int box_col = 3 * (col / 3);
        for (int r = box_row; r < box_row + 3; ++r) {
            for (int c = box_col; c < box_col + 3; ++c) {
                if (board[r][c] == num) return false;
            }
        }
        return true;
    }
};

// Usage:
// std::vector<std::vector<int>> board = {
//     {5,3,0,0,7,0,0,0,0},
//     {6,0,0,1,9,5,0,0,0},
//     {0,9,8,0,0,0,0,6,0},
//     {8,0,0,0,6,0,0,0,3},
//     {4,0,0,8,0,3,0,0,1},
//     {7,0,0,0,2,0,0,0,6},
//     {0,6,0,0,0,0,2,8,0},
//     {0,0,0,4,1,9,0,0,5},
//     {0,0,0,0,8,0,0,7,9}
// };
// sudoku_solver solver;
// if (solver.solve(board)) { /* print board */ }
