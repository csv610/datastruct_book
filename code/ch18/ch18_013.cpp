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
