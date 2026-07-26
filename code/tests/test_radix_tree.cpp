#include "radix_tree.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
  dsa::RadixTree<int> tree;
  tree.insert("apple", 1);
  tree.insert("app", 2);
  tree.insert("banana", 3);
  tree.insert("band", 4);
  assert(tree.size() == 4);

  assert(tree.contains("apple"));
  assert(tree.contains("app"));
  assert(tree.contains("banana"));
  assert(!tree.contains("ap"));

  assert(*tree.find("apple") == 1);
  assert(*tree.find("app") == 2);
  assert(*tree.find("banana") == 3);
  assert(*tree.find("band") == 4);
  assert(tree.find("xyz") == nullptr);

  auto results = tree.prefix_search("app");
  assert(results.size() == 2);

  auto band_results = tree.prefix_search("ban");
  assert(band_results.size() == 2);

  tree.erase("banana");
  assert(!tree.contains("banana"));
  assert(tree.contains("band"));
  assert(tree.size() == 3);

  std::cout << "radix_tree: all passed\n";
}
