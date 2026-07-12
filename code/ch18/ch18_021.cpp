// Optimization: Precompute which digits are used in each
// row, column, and box using bitmasks.
// MRV heuristic: always fill the cell with fewest choices.
class sudoku_optimized {
public:
    bool solve(std::vector<std::vector<int>>& board) {
        int row_mask[9] = {}, col_mask[9] = {}, box_mask[9] = {};
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[r][c] != 0) {
                    int bit = 1 << board[r][c];
                    row_mask[r] |= bit;
                    col_mask[c] |= bit;
                    box_mask[box_id(r, c)] |= bit;
                }
            }
        }
        return solve(board, row_mask, col_mask, box_mask);
    }

private:
    int box_id(int r, int c) { return 3 * (r / 3) + c / 3; }

    bool solve(std::vector<std::vector<int>>& board,
               int row_mask[], int col_mask[], int box_mask[]) {
        int best_r = -1, best_c = -1, min_choices = 10;
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[r][c] == 0) {
                    int used = row_mask[r] | col_mask[c]
                             | box_mask[box_id(r, c)];
                    int choices = 9 - __builtin_popcount(used);
                    if (choices < min_choices) {
                        min_choices = choices;
                        best_r = r;
                        best_c = c;
                        if (choices == 0) return false;
                        if (choices == 1) break;
                    }
                }
            }
            if (best_r >= 0 && min_choices <= 1) break;
        }
        if (best_r < 0) return true;

        int b = box_id(best_r, best_c);
        int used = row_mask[best_r] | col_mask[best_c]
                 | box_mask[b];
        for (int num = 1; num <= 9; ++num) {
            int bit = 1 << num;
            if (used & bit) continue;
            board[best_r][best_c] = num;
            row_mask[best_r] |= bit;
            col_mask[best_c] |= bit;
            box_mask[b] |= bit;
            if (solve(board, row_mask, col_mask, box_mask))
                return true;
            board[best_r][best_c] = 0;
            row_mask[best_r] &= ~bit;
            col_mask[best_c] &= ~bit;
            box_mask[b] &= ~bit;
        }
        return false;
    }
};
