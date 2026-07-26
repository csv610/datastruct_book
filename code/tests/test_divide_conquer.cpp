#include "divide_conquer.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    // Closest pair of points
    {
        std::vector<dsa::point_2d> pts = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
        double d = dsa::closest_pair(pts);
        assert(std::abs(d - 1.4142135) < 0.001);  // sqrt(2) for (2,3) and (3,4)
    }
    {
        std::vector<dsa::point_2d> pts = {{0, 0}, {1, 1}};
        double d = dsa::closest_pair(pts);
        assert(std::abs(d - 1.4142135) < 0.001);  // sqrt(2)
    }

    // Strassen's multiply
    {
        std::vector<std::vector<double>> A = {{1, 2}, {3, 4}};
        std::vector<std::vector<double>> B = {{5, 6}, {7, 8}};
        auto C = dsa::strassen_multiply(A, B);
        assert(static_cast<int>(C[0][0]) == 19);
        assert(static_cast<int>(C[0][1]) == 22);
        assert(static_cast<int>(C[1][0]) == 43);
        assert(static_cast<int>(C[1][1]) == 50);
    }
    {
        // Identity matrix
        std::vector<std::vector<double>> I = {{1, 0}, {0, 1}};
        std::vector<std::vector<double>> A = {{3, 7}, {1, 4}};
        auto C = dsa::strassen_multiply(I, A);
        assert(static_cast<int>(C[0][0]) == 3);
        assert(static_cast<int>(C[0][1]) == 7);
        assert(static_cast<int>(C[1][0]) == 1);
        assert(static_cast<int>(C[1][1]) == 4);
    }

    std::cout << "All divide & conquer tests passed\n";
    return 0;
}
