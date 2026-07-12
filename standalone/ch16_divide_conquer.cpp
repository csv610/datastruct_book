// ch16: Divide and Conquer — Merge Sort, Quick Sort, Quickselect, Closest Pair, Defective Chessboard
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <span>
#include <vector>

// ---- Merge Sort ----
void merge_sort(std::span<int> arr) {
    if (arr.size() <= 1) return;

    size_t mid = arr.size() / 2;
    std::vector<int> left(arr.begin(), arr.begin() + mid);
    std::vector<int> right(arr.begin() + mid, arr.end());

    merge_sort(left);
    merge_sort(right);

    size_t i = 0, j = 0, k = 0;
    while (i < left.size() && j < right.size())
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

// ---- Quick Sort (Lomuto partition) ----
size_t partition(std::span<int> arr) {
    int pivot = arr.back();
    size_t i = 0;
    for (size_t j = 0; j < arr.size() - 1; ++j) {
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr.back());
    return i;
}

void quick_sort(std::span<int> arr) {
    if (arr.size() <= 1) return;
    size_t p = partition(arr);
    quick_sort(arr.subspan(0, p));
    quick_sort(arr.subspan(p + 1));
}

// ---- Quickselect (k-th smallest) ----
int quick_select(std::span<int> arr, size_t k) {
    if (arr.size() == 1) return arr[0];
    size_t p = partition(arr);
    if (k == p) return arr[p];
    if (k < p) return quick_select(arr.subspan(0, p), k);
    return quick_select(arr.subspan(p + 1), k - p - 1);
}

// ---- Closest Pair of Points (naive O(n^2)) ----
struct point { double x, y; };

double euclidean(const point& a, const point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

double closest_pair_brute(std::span<const point> points) {
    double best = std::numeric_limits<double>::max();
    for (size_t i = 0; i < points.size(); ++i)
        for (size_t j = i + 1; j < points.size(); ++j)
            best = std::min(best, euclidean(points[i], points[j]));
    return best;
}

// ---- Defective Chessboard (Tromino Tiling) ----
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

int main() {
    std::cout << "=== Merge Sort ===\n\n";
    std::vector<int> ms_arr = {3, 1, 4, 1, 5, 9, 2, 6};
    std::cout << "Before:";
    for (auto x : ms_arr) std::cout << " " << x;
    std::cout << "\n";
    merge_sort(ms_arr);
    std::cout << "After:";
    for (auto x : ms_arr) std::cout << " " << x;
    std::cout << "\n\n";

    std::cout << "=== Quick Sort ===\n\n";
    std::vector<int> qs_arr = {3, 1, 4, 1, 5, 9, 2, 6};
    std::cout << "Before:";
    for (auto x : qs_arr) std::cout << " " << x;
    std::cout << "\n";
    quick_sort(qs_arr);
    std::cout << "After:";
    for (auto x : qs_arr) std::cout << " " << x;
    std::cout << "\n\n";

    std::cout << "=== Quickselect ===\n\n";
    std::vector<int> sel_arr = {3, 1, 4, 1, 5, 9, 2, 6};
    for (size_t k = 0; k < sel_arr.size(); ++k) {
        std::vector<int> copy = sel_arr;
        int val = quick_select(copy, k);
        std::cout << k << "-th smallest: " << val << "\n";
    }
    std::cout << "\n";

    std::cout << "=== Closest Pair (O(n^2)) ===\n\n";
    std::vector<point> pts = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {1.5, 2.5}};
    double d = closest_pair_brute(pts);
    std::cout << "Points: (1,2) (3,4) (5,6) (1.5,2.5)\n";
    std::cout << "Closest distance: " << d << "\n\n";

    std::cout << "=== Defective Chessboard ===\n\n";
    size_t k = 3;
    size_t n = 1 << k;
    std::vector<std::vector<int>> board(n, std::vector<int>(n, -1));
    size_t defect_r = 0, defect_c = 0;
    board[defect_r][defect_c] = 0;
    int tile = 1;
    cover_board(board, 0, 0, defect_r, defect_c, n, tile);

    std::cout << "Board (" << n << "x" << n << "), defect at (0,0):\n";
    for (const auto& row : board) {
        for (int cell : row)
            std::cout << cell << "\t";
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "All ch16 tests passed.\n";
    return 0;
}
