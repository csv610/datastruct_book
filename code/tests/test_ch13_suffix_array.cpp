#include "ch13_suffix_array.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
  ds::SuffixArray sa("banana");
  auto indices = sa.search("ana");
  assert(indices.size() == 2);
  assert(indices[0] == 1);
  assert(indices[1] == 3);

  assert(sa.count_occurrences("na") == 2);
  assert(sa.count_occurrences("xyz") == 0);

  auto indices2 = sa.search("banana");
  assert(indices2.size() == 1);
  assert(indices2[0] == 0);

  ds::SuffixArray sa2("mississippi");
  auto idx = sa2.search("issi");
  assert(idx.size() == 1);
  assert(idx[0] == 1);

  std::cout << "ch13_suffix_array: all passed\n";
}
