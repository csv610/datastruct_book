enum quadrant { TL, TR, BL, BR };

void cover_board(std::vector<std::vector<int>>& board,
                 size_t row, size_t col,
                 size_t defect_r, size_t defect_c,
                 size_t size, int& tile) {
    if (size == 1) return;

    size_t half = size / 2;
    int t = tile++;

    // Top-left quadrant
    if (defect_r < row + half && defect_c < col + half) {
        cover_board(board, row, col, defect_r, defect_c, half, tile);
    } else {
        board[row + half - 1][col + half - 1] = t;
        cover_board(board, row, col, row + half - 1, col + half - 1, half, tile);
    }

    // Top-right quadrant
    if (defect_r < row + half && defect_c >= col + half) {
        cover_board(board, row, col + half, defect_r, defect_c, half, tile);
    } else {
        board[row + half - 1][col + half] = t;
        cover_board(board, row, col + half, row + half - 1, col + half, half, tile);
    }

    // Bottom-left quadrant
    if (defect_r >= row + half && defect_c < col + half) {
        cover_board(board, row + half, col, defect_r, defect_c, half, tile);
    } else {
        board[row + half][col + half - 1] = t;
        cover_board(board, row + half, col, row + half, col + half - 1, half, tile);
    }

    // Bottom-right quadrant
    if (defect_r >= row + half && defect_c >= col + half) {
        cover_board(board, row + half, col + half, defect_r, defect_c, half, tile);
    } else {
        board[row + half][col + half] = t;
        cover_board(board, row + half, col + half, row + half, col + half, half, tile);
    }
}
