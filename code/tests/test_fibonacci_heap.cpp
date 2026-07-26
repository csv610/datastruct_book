#include "fibonacci_heap.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
  dsa::FibonacciHeap<int> h;
  h.push(5);
  h.push(3);
  h.push(8);
  h.push(1);
  h.push(4);
  assert(h.size() == 5);
  assert(h.top() == 1);

  assert(h.pop() == 1);
  assert(h.pop() == 3);
  assert(h.size() == 3);

  dsa::FibonacciHeap<int> h2;
  h2.push(2);
  h2.push(6);
  h.merge(h2);
  assert(h.size() == 5);
  assert(h.top() == 2);

  std::vector<int> sorted;
  while (!h.empty()) sorted.push_back(h.pop());
  std::vector<int> expected = {2, 4, 5, 6, 8};
  assert(sorted == expected);

  dsa::FibonacciHeap<std::string> sh;
  sh.push("cherry");
  sh.push("apple");
  sh.push("banana");
  assert(sh.top() == "apple");

  std::cout << "fibonacci_heap: all passed\n";
}
