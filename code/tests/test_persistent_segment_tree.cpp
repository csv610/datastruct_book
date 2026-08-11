#include "persistent_segment_tree.h"
#include <cassert>
#include <print>

int main() {
  dsa::persistent_segment_tree<int> tree(4, {1, 2, 3, 4});
  assert(tree.query(0, 0, 4) == 10);
  const auto v1 = tree.update(0, 1, 20);
  const auto v2 = tree.update(v1, 3, 40);
  assert(tree.query(0, 0, 2) == 3);
  assert(tree.query(v1, 0, 2) == 21);
  assert(tree.query(v2, 2, 4) == 43);
  std::print("persistent segment tree: all passed\n");
}
