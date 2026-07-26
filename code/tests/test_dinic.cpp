#include "dinic.h"
#include <cassert>
#include <iostream>

int main() {
  dsa::dinic<int> d(6);
  d.add_edge(0, 1, 10);
  d.add_edge(0, 2, 10);
  d.add_edge(1, 2, 2);
  d.add_edge(1, 3, 8);
  d.add_edge(2, 4, 9);
  d.add_edge(3, 5, 10);
  d.add_edge(4, 3, 7);
  d.add_edge(4, 5, 10);

  int flow = d.max_flow(0, 5);
  assert(flow == 17);

  dsa::dinic<int> d2(4);
  d2.add_edge(0, 1, 1);
  d2.add_edge(0, 2, 1);
  d2.add_edge(1, 3, 1);
  d2.add_edge(2, 3, 1);
  assert(d2.max_flow(0, 3) == 2);

  dsa::dinic<int> d3(3);
  d3.add_edge(0, 1, 100);
  d3.add_edge(1, 2, 100);
  assert(d3.max_flow(0, 2) == 100);

  dsa::dinic<int> d4(2);
  d4.add_edge(0, 1, 5);
  assert(d4.max_flow(0, 1) == 5);
  auto cut = d4.min_cut(0);
  assert(cut[0] == true);
  assert(cut[1] == false);

  dsa::dinic<int> d5(3);
  assert(d5.max_flow(0, 2) == 0);

  std::cout << "dinic: all passed\n";
}
