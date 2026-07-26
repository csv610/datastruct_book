#pragma once
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <limits>
#include <utility>
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
    std::sort(strip.begin(), strip.end(),
              [](const auto& a, const auto& b) { return a.y < b.y; });
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
    std::sort(points.begin(), points.end(),
              [](const auto& a, const auto& b) { return a.x < b.x; });
    return closest_pair_rec(points, 0, points.size());
}

// ---- Strassen's Matrix Multiply ----
inline void mat_add(const std::vector<std::vector<double>>& A,
                    const std::vector<std::vector<double>>& B,
                    std::vector<std::vector<double>>& C, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
}

inline void mat_sub(const std::vector<std::vector<double>>& A,
                    const std::vector<std::vector<double>>& B,
                    std::vector<std::vector<double>>& C, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = 0; j < n; ++j)
            C[i][j] = A[i][j] - B[i][j];
}

inline void strassen_rec(const std::vector<std::vector<double>>& A,
                         const std::vector<std::vector<double>>& B,
                         std::vector<std::vector<double>>& C, std::size_t n) {
    if (n <= 64) {
        C.assign(n, std::vector<double>(n, 0.0));
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                for (std::size_t k = 0; k < n; ++k)
                    C[i][j] += A[i][k] * B[k][j];
        return;
    }

    std::size_t h = n / 2;
    auto make = [&]() { return std::vector<std::vector<double>>(h, std::vector<double>(h)); };

    std::vector<std::vector<double>> A11(h, std::vector<double>(h)), A12(h, std::vector<double>(h)),
        A21(h, std::vector<double>(h)), A22(h, std::vector<double>(h));
    std::vector<std::vector<double>> B11(h, std::vector<double>(h)), B12(h, std::vector<double>(h)),
        B21(h, std::vector<double>(h)), B22(h, std::vector<double>(h));

    for (std::size_t i = 0; i < h; ++i)
        for (std::size_t j = 0; j < h; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + h];
            A21[i][j] = A[i + h][j];
            A22[i][j] = A[i + h][j + h];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + h];
            B21[i][j] = B[i + h][j];
            B22[i][j] = B[i + h][j + h];
        }

    auto T1 = make(), T2 = make();
    std::vector<std::vector<double>> M1(h), M2(h), M3(h), M4(h), M5(h), M6(h), M7(h);

    auto sum = make(), diff = make();

    // M1 = (A11 + A22)(B11 + B22)
    mat_add(A11, A22, sum, h); mat_add(B11, B22, diff, h);
    strassen_rec(sum, diff, M1, h);
    // M2 = (A21 + A22)B11
    mat_add(A21, A22, sum, h);
    strassen_rec(sum, B11, M2, h);
    // M3 = A11(B12 - B22)
    mat_sub(B12, B22, diff, h);
    strassen_rec(A11, diff, M3, h);
    // M4 = A22(B21 - B11)
    mat_sub(B21, B11, diff, h);
    strassen_rec(A22, diff, M4, h);
    // M5 = (A11 + A12)B22
    mat_add(A11, A12, sum, h);
    strassen_rec(sum, B22, M5, h);
    // M6 = (A21 - A11)(B11 + B12)
    mat_sub(A21, A11, diff, h); mat_add(B11, B12, sum, h);
    strassen_rec(diff, sum, M6, h);
    // M7 = (A12 - A22)(B21 + B22)
    mat_sub(A12, A22, diff, h); mat_add(B21, B22, sum, h);
    strassen_rec(diff, sum, M7, h);

    // C11 = M1 + M4 - M5 + M7
    // C12 = M3 + M5
    // C21 = M2 + M4
    // C22 = M1 - M2 + M3 + M6
    C.assign(n, std::vector<double>(n));
    for (std::size_t i = 0; i < h; ++i)
        for (std::size_t j = 0; j < h; ++j) {
            C[i][j]         = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + h]     = M3[i][j] + M5[i][j];
            C[i + h][j]     = M2[i][j] + M4[i][j];
            C[i + h][j + h] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
}

inline std::vector<std::vector<double>> strassen_multiply(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B) {
    std::size_t n = A.size();
    std::vector<std::vector<double>> C;
    strassen_rec(A, B, C, n);
    return C;
}

}  // namespace dsa
