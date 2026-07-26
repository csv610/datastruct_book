#include "sparse_vector.h"
#include <cassert>
#include <vector>
#include <print>

int main() {
  dsa::SparseVector<double> v(5);
  v.set(0, 1.0);
  v.set(2, 3.0);
  v.set(4, 5.0);
  assert(v.nnz() == 3);
  assert(v.size() == 5);
  assert(v.get(0) == 1.0);
  assert(v.get(1) == 0.0);
  assert(v.get(2) == 3.0);
  assert(v.get(3) == 0.0);
  assert(v.get(4) == 5.0);

  auto dense = v.to_dense();
  std::vector<double> expected = {1, 0, 3, 0, 5};
  assert(dense == expected);

  dsa::SparseVector<double> w(5);
  w.set(0, 2.0);
  w.set(2, 1.0);
  w.set(3, 4.0);

  assert(v.dot(w) == 5.0);

  auto sum = v + w;
  assert(sum.get(0) == 3.0);
  assert(sum.get(2) == 4.0);
  assert(sum.get(3) == 4.0);
  assert(sum.get(4) == 5.0);

  auto diff = v - w;
  assert(diff.get(0) == -1.0);
  assert(diff.get(2) == 2.0);
  assert(diff.get(4) == 5.0);

  auto scaled = v.scale(2.0);
  assert(scaled.get(0) == 2.0);
  assert(scaled.get(2) == 6.0);
  assert(scaled.get(4) == 10.0);

  assert(v.norm_squared() == 35.0);

  std::print("sparse_vector: all passed\n");
}
