#include "two_three_tree.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
  dsa::TwoThreeTree<int> tree;
  assert(tree.empty());

  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);
  tree.insert(25);
  tree.insert(35);
  assert(tree.size() == 7);

  assert(tree.contains(10));
  assert(tree.contains(25));
  assert(!tree.contains(100));

  auto v = tree.inorder();
  std::vector<int> expected = {5, 10, 15, 20, 25, 30, 35};
  assert(v == expected);

  tree.erase(20);
  assert(!tree.contains(20));
  assert(tree.size() == 6);

  tree.erase(5);
  tree.erase(35);
  v = tree.inorder();
  expected = {10, 15, 25, 30};
  assert(v == expected);

  dsa::TwoThreeTree<std::string> stree;
  stree.insert("cherry");
  stree.insert("apple");
  stree.insert("banana");
  auto sv = stree.inorder();
  assert(sv[0] == "apple");
  assert(sv[1] == "banana");
  assert(sv[2] == "cherry");

  dsa::TwoThreeTree<int> copy(tree);
  assert(copy.size() == tree.size());
  assert(copy.contains(15));

  std::cout << "two_three_tree: all passed\n";
}
