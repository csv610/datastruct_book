#include "fenwick_segment.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  dsa::fenwick_tree<int> ft(8);
  ft.add(0, 1); ft.add(1, 2); ft.add(2, 3); ft.add(3, 4);
  assert(ft.prefix_sum(4) == 10);
  assert(ft.range_sum(1, 3) == 5);
  assert(ft.point_value(2) == 3);

  ft.add(2, 10);
  assert(ft.point_value(2) == 13);
  assert(ft.range_sum(0, 4) == 20);

  std::vector<int> data = {1, 3, 5, 7, 9, 11};
  dsa::segment_tree<int> st(data);
  assert(st.query(0, 2) == 9);
  assert(st.query(1, 4) == 24);
  assert(st.query(0, 5) == 36);

  st.update(2, 10);
  assert(st.query(0, 2) == 14);
  assert(st.query(2, 2) == 10);

  dsa::fenwick_tree<int> ft2(5);
  for (int i = 0; i < 5; ++i) ft2.add(i, i + 1);
  assert(ft2.prefix_sum(5) == 15);
  assert(ft2.range_sum(0, 1) == 1);
  assert(ft2.range_sum(3, 5) == 9);

  std::cout << "fenwick_segment: all passed\n";
}
