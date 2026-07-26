#include "deque.h"
#include <cassert>
#include <string>
#include <print>

int main() {
  dsa::Deque<int> dq;

  dq.push_back(1);
  dq.push_back(2);
  dq.push_front(0);
  assert(dq.size() == 3);
  assert(dq[0] == 0 && dq[1] == 1 && dq[2] == 2);

  dq.pop_front();
  assert(dq.front() == 1);
  dq.pop_back();
  assert(dq.back() == 1);
  assert(dq.size() == 1);

  dq.clear();
  for (int i = 0; i < 100; ++i) dq.push_back(i);
  assert(dq.size() == 100);
  for (int i = 0; i < 100; ++i) assert(dq[i] == i);

  dsa::Deque<std::string> sdq;
  sdq.push_back("hello");
  sdq.push_back("world");
  sdq.push_front("hi");
  assert(sdq.front() == "hi");
  assert(sdq.back() == "world");

  dsa::Deque<int> copy(dq);
  assert(copy.size() == dq.size());
  assert(copy[50] == 50);
  assert(copy.front() == 0);
  assert(copy.back() == 99);

  std::print("deque: all passed\n");
}
