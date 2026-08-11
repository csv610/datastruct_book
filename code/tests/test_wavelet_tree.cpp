#include "wavelet_tree.h"
#include <cassert>
#include <print>

int main() {
  dsa::wavelet_tree tree("abracadabra");
  assert(tree.size() == 11);
  assert(tree.access(3) == 'a');
  assert(tree.rank('a', 11) == 5);
  assert(tree.select('a', 0) == 0);
  assert(tree.select('a', 4) == 10);
  assert(tree.range_count(0, 11, 'b', 'r') == 6);
  std::print("wavelet tree: all passed\n");
}
