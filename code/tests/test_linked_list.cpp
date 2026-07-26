#include "linked_list.h"
#include <cassert>
#include <string>
#include <vector>
#include <print>

int main() {
  dsa::linked_list<int> empty;
  assert(empty.size() == 0);
  assert(empty.empty());

  dsa::linked_list<int> a{1, 2, 3, 4, 5};
  assert(a.size() == 5);
  assert(a.front() == 1);
  assert(a.back() == 5);

  dsa::linked_list<int> b(a);
  assert(b.size() == 5);
  assert(b.front() == 1);
  b.pop_front();
  assert(b.size() == 4);
  assert(b.front() == 2);
  assert(a.size() == 5);

  dsa::linked_list<int> c;
  c = a;
  assert(c.size() == 5);
  c.clear();
  assert(c.empty());

  a.push_front(0);
  assert(a.front() == 0);
  assert(a.size() == 6);

  a.reverse();
  std::vector<int> vals;
  for (auto& v : a) vals.push_back(v);
  assert((vals == std::vector<int>{5, 4, 3, 2, 1, 0}));

  dsa::linked_list<std::string> s{"hello", "world"};
  assert(s.front() == "hello");
  s.push_back("!");
  assert(s.back() == "!");

  dsa::linked_list<int> d{10};
  assert(d.size() == 1);
  assert(d.front() == 10);
  assert(d.back() == 10);

  dsa::linked_list<int> moved = std::move(b);
  assert(moved.size() == 4);
  assert(moved.front() == 2);

  std::print("linked_list: all passed\n");
}
