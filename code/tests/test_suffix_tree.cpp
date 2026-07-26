#include "suffix_tree.h"
#include <cassert>
#include <string>
#include <vector>
#include <print>

int main() {
  dsa::SuffixTree st("banana");
  assert(st.contains("ana"));
  assert(st.contains("banana"));
  assert(st.contains("na"));
  assert(!st.contains("xyz"));

  auto indices = st.search("ana");
  assert(indices.size() == 2);
  assert(indices[0] == 1);
  assert(indices[1] == 3);

  assert(st.count_occurrences("na") == 2);
  assert(st.count_occurrences("xyz") == 0);

  dsa::SuffixTree st2("mississippi");
  auto idx = st2.search("issi");
  assert(idx.size() == 2);

  assert(st.longest_repeated_substring() == "ana");

  std::print("suffix_tree: all passed\n");
}
