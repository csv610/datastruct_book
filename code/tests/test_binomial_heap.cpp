#include "binomial_heap.h"
#include <cassert>
#include <string>
#include <vector>
#include <print>

int main() {
  dsa::BinomialHeap<int> h;
  h.push(5);
  h.push(3);
  h.push(8);
  h.push(1);
  h.push(4);
  assert(h.size() == 5);
  assert(h.top() == 1);

  assert(h.pop() == 1);
  assert(h.pop() == 3);
  assert(h.pop() == 4);
  assert(h.size() == 2);

  dsa::BinomialHeap<int> h2;
  h2.push(2);
  h2.push(6);
  h.merge(h2);
  assert(h.size() == 4);
  assert(h.top() == 2);

  std::vector<int> sorted;
  while (!h.empty()) sorted.push_back(h.pop());
  std::vector<int> expected = {2, 5, 6, 8};
  assert(sorted == expected);

  dsa::BinomialHeap<std::string> sh;
  sh.push("cherry");
  sh.push("apple");
  sh.push("banana");
  assert(sh.top() == "apple");
  assert(sh.pop() == "apple");
  assert(sh.pop() == "banana");

  std::print("binomial_heap: all passed\n");
}
