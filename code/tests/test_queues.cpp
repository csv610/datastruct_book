#include "queues.h"
#include <cassert>
#include <print>

int main() {
  dsa::circular_queue<int> cq;
  assert(cq.empty());
  cq.push(1); cq.push(2); cq.push(3);
  assert(cq.size() == 3);
  assert(cq.front() == 1);
  assert(cq.back() == 3);
  cq.pop();
  assert(cq.front() == 2);
  assert(cq.size() == 2);

  for (int i = 0; i < 100; ++i) cq.push(i);
  assert(cq.size() == 102);
  assert(cq.front() == 2);
  cq.pop();
  assert(cq.front() == 3);

  dsa::linked_queue<int> lq;
  assert(lq.empty());
  lq.push(10); lq.push(20); lq.push(30);
  assert(lq.front() == 10);
  assert(lq.back() == 30);
  lq.pop();
  assert(lq.front() == 20);
  lq.pop();
  lq.pop();
  assert(lq.empty());

  for (int i = 0; i < 50; ++i) lq.push(i);
  assert(lq.size() == 50);
  assert(lq.front() == 0);

  std::print("queues: all passed\n");
}
