#include "sparse_matrix.h"
#include <cassert>
#include <vector>
#include <print>

int main() {
  std::vector<std::vector<double>> dense = {
      {1, 0, 0, 0},
      {0, 0, 2, 0},
      {0, 3, 0, 0},
      {0, 0, 0, 4}
  };

  dsa::SparseMatrix<double> sm(dense);
  assert(sm.rows() == 4);
  assert(sm.cols() == 4);
  assert(sm.nnz() == 4);
  assert(sm.get(0, 0) == 1.0);
  assert(sm.get(1, 2) == 2.0);
  assert(sm.get(2, 1) == 3.0);
  assert(sm.get(3, 3) == 4.0);
  assert(sm.get(0, 1) == 0.0);

  auto result = sm.to_dense();
  for (std::size_t r = 0; r < 4; ++r)
    for (std::size_t c = 0; c < 4; ++c)
      assert(result[r * 4 + c] == dense[r][c]);

  std::vector<double> x = {1, 2, 3, 4};
  auto y = sm.multiply(x);
  assert(y[0] == 1.0);
  assert(y[1] == 6.0);
  assert(y[2] == 6.0);
  assert(y[3] == 16.0);

  auto t = sm.transpose();
  assert(t.get(0, 0) == 1.0);
  assert(t.get(2, 1) == 2.0);
  assert(t.get(1, 2) == 3.0);
  assert(t.get(3, 3) == 4.0);

  std::print("sparse_matrix: all passed\n");
}
