#include "splay_tree.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
  dsa::SplayTree<int> tree;
  assert(tree.empty());

  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(5);
  tree.insert(15);
  assert(tree.size() == 5);

  assert(tree.contains(10));
  assert(tree.contains(20));
  assert(!tree.contains(100));

  auto v = tree.inorder();
  std::vector<int> expected = {5, 10, 15, 20, 30};
  assert(v == expected);

  tree.erase(20);
  assert(!tree.contains(20));
  assert(tree.size() == 4);

  tree.erase(5);
  tree.erase(30);
  v = tree.inorder();
  expected = {10, 15};
  assert(v == expected);

  dsa::SplayTree<std::string> stree;
  stree.insert("cherry");
  stree.insert("apple");
  stree.insert("banana");
  auto sv = stree.inorder();
  assert(sv[0] == "apple");
  assert(sv[1] == "banana");
  assert(sv[2] == "cherry");

  dsa::SplayTree<int> copy(tree);
  assert(copy.size() == tree.size());
  assert(copy.contains(15));

  std::cout << "splay_tree: all passed\n";
}
