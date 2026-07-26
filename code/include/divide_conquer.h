#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <mdspan>
#include <vector>

namespace dsa {

// ---- Closest Pair of Points (Divide & Conquer) ----
struct point_2d {
    double x, y;
};

inline double dist(const point_2d& a, const point_2d& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline double closest_brute(std::vector<point_2d>& pts, std::size_t l, std::size_t r) {
    double best = std::numeric_limits<double>::max();
    for (std::size_t i = l; i < r; ++i)
        for (std::size_t j = i + 1; j < r; ++j)
            best = std::min(best, dist(pts[i], pts[j]));
    return best;
}

inline double closest_strip(std::vector<point_2d>& strip, double d) {
    std::ranges::sort(strip, {}, &point_2d::y);
    double best = d;
    for (std::size_t i = 0; i < strip.size(); ++i)
        for (std::size_t j = i + 1;
             j < strip.size() && (strip[j].y - strip[i].y) < best; ++j)
            best = std::min(best, dist(strip[i], strip[j]));
    return best;
}

inline double closest_pair_rec(std::vector<point_2d>& pts, std::size_t l, std::size_t r) {
    if (r - l <= 3) return closest_brute(pts, l, r);

    auto mid = l + (r - l) / 2;
    double mid_x = pts[mid].x;
    double dl = closest_pair_rec(pts, l, mid);
    double dr = closest_pair_rec(pts, mid, r);
    double d = std::min(dl, dr);

    std::vector<point_2d> strip;
    for (std::size_t i = l; i < r; ++i)
        if (std::abs(pts[i].x - mid_x) < d)
            strip.push_back(pts[i]);

    return closest_strip(strip, d);
}

inline double closest_pair(std::vector<point_2d> points) {
    if (points.size() < 2) return std::numeric_limits<double>::max();
    std::ranges::sort(points, {}, &point_2d::x);
    return closest_pair_rec(points, 0, points.size());
}

// ---- Strassen's Matrix Multiply (using std::mdspan) ----
using mat_view = std::mdspan<double, std::extents<std::size_t, std::dynamic_extent, std::dynamic_extent>>;

inline void mat_add(mat_view A, mat_view B, mat_view C, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            C[i, j] = A[i, j] + B[i, j];
}

inline void mat_sub(mat_view A, mat_view B, mat_view C, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            C[i, j] = A[i, j] - B[i, j];
}

inline void strassen_rec(mat_view A, mat_view B, mat_view C, std::size_t n) {
    if (n <= 64) {
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j) {
                C[i, j] = 0.0;
                for (std::size_t k = 0; k < n; ++k)
                    C[i, j] += A[i, k] * B[k, j];
            }
        return;
    }

    std::size_t h = n / 2;

    // Allocate submatrices
    std::vector<double> buf_A11(h * h), buf_A12(h * h), buf_A21(h * h), buf_A22(h * h);
    std::vector<double> buf_B11(h * h), buf_B12(h * h), buf_B21(h * h), buf_B22(h * h);
    std::vector<double> buf_sum(h * h), buf_diff(h * h);
    std::vector<double> buf_M1(h * h), buf_M2(h * h), buf_M3(h * h),
                        buf_M4(h * h), buf_M5(h * h), buf_M6(h * h), buf_M7(h * h);

    mat_view A11(buf_A11.data(), h, h), A12(buf_A12.data(), h, h),
             A21(buf_A21.data(), h, h), A22(buf_A22.data(), h, h);
    mat_view B11(buf_B11.data(), h, h), B12(buf_B12.data(), h, h),
             B21(buf_B21.data(), h, h), B22(buf_B22.data(), h, h);
    mat_view sum(buf_sum.data(), h, h), diff(buf_diff.data(), h, h);
    mat_view M1(buf_M1.data(), h, h), M2(buf_M2.data(), h, h), M3(buf_M3.data(), h, h),
             M4(buf_M4.data(), h, h), M5(buf_M5.data(), h, h), M6(buf_M6.data(), h, h),
             M7(buf_M7.data(), h, h);

    for (std::size_t i = 0; i < h; ++i)
        for (std::size_t j = 0; j < h; ++j) {
            A11[i, j] = A[i, j];
            A12[i, j] = A[i, j + h];
            A21[i, j] = A[i + h, j];
            A22[i, j] = A[i + h, j + h];
            B11[i, j] = B[i, j];
            B12[i, j] = B[i, j + h];
            B21[i, j] = B[i + h, j];
            B22[i, j] = B[i + h, j + h];
        }

    mat_add(A11, A22, sum, h); mat_add(B11, B22, diff, h);
    strassen_rec(sum, diff, M1, h);
    mat_add(A21, A22, sum, h);
    strassen_rec(sum, B11, M2, h);
    mat_sub(B12, B22, diff, h);
    strassen_rec(A11, diff, M3, h);
    mat_sub(B21, B11, diff, h);
    strassen_rec(A22, diff, M4, h);
    mat_add(A11, A12, sum, h);
    strassen_rec(sum, B22, M5, h);
    mat_sub(A21, A11, diff, h); mat_add(B11, B12, sum, h);
    strassen_rec(diff, sum, M6, h);
    mat_sub(A12, A22, diff, h); mat_add(B21, B22, sum, h);
    strassen_rec(diff, sum, M7, h);

    for (std::size_t i = 0; i < h; ++i)
        for (std::size_t j = 0; j < h; ++j) {
            C[i, j]         = M1[i, j] + M4[i, j] - M5[i, j] + M7[i, j];
            C[i, j + h]     = M3[i, j] + M5[i, j];
            C[i + h, j]     = M2[i, j] + M4[i, j];
            C[i + h, j + h] = M1[i, j] - M2[i, j] + M3[i, j] + M6[i, j];
        }
}

inline std::vector<std::vector<double>> strassen_multiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    std::size_t n = A.size();

    std::vector<double> buf_A(n * n), buf_B(n * n), buf_C(n * n);
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j) {
            buf_A[i * n + j] = A[i][j];
            buf_B[i * n + j] = B[i][j];
        }

    mat_view Av(buf_A.data(), n, n), Bv(buf_B.data(), n, n), Cv(buf_C.data(), n, n);
    strassen_rec(Av, Bv, Cv, n);

    std::vector<std::vector<double>> C(n, std::vector<double>(n));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            C[i][j] = Cv[i, j];
    return C;
}

}  // namespace dsa
