#include "binary_tree.h"
#include <cassert>
#include <vector>
#include <print>

int main() {
  dsa::binary_tree<int> tree;
  assert(tree.empty());

  tree.insert(5);
  tree.insert(3);
  tree.insert(7);
  tree.insert(1);
  tree.insert(4);
  tree.insert(6);
  tree.insert(8);
  assert(tree.size() == 7);
  assert(tree.height() == 3);

  assert(tree.contains(5));
  assert(tree.contains(1));
  assert(!tree.contains(99));

  assert(*tree.find(3) == 3);
  assert(tree.find(99) == nullptr);

  assert(*tree.min() == 1);
  assert(*tree.max() == 8);

  std::vector<int> in;
  tree.inorder([&](const int& v) { in.push_back(v); });
  assert((in == std::vector<int>{1, 3, 4, 5, 6, 7, 8}));

  std::vector<int> pre;
  tree.preorder([&](const int& v) { pre.push_back(v); });
  assert(!pre.empty());

  tree.erase(1);
  assert(!tree.contains(1));
  assert(tree.size() == 6);
  tree.erase(5);
  assert(!tree.contains(5));
  assert(tree.size() == 5);
  tree.erase(8);
  assert(!tree.contains(8));

  in.clear();
  tree.inorder([&](const int& v) { in.push_back(v); });
  assert((in == std::vector<int>{3, 4, 6, 7}));

  tree.erase(3);
  tree.erase(4);
  tree.erase(6);
  tree.erase(7);
  assert(tree.empty());

  tree.insert(10);
  assert(tree.size() == 1);
  assert(*tree.min() == 10);
  assert(*tree.max() == 10);
  tree.erase(10);
  assert(tree.empty());

  dsa::binary_tree<int> from_traversals =
      dsa::binary_tree<int>::from_inorder_preorder(
          std::vector<int>{1, 2, 3, 4, 5},
          std::vector<int>{4, 2, 1, 3, 5});
  assert(from_traversals.size() == 5);
  std::vector<int> result;
  from_traversals.inorder([&](const int& v) { result.push_back(v); });
  assert((result == std::vector<int>{1, 2, 3, 4, 5}));

  std::print("binary_tree: all passed\n");
}
