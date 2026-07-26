#include "sorting.h"
#include "max_heap.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> a = {5, 3, 8, 1, 2, 7, 4, 6};
  dsa::insertion_sort(std::span<int>(a));
  assert((a == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

  std::vector<int> b = {5, 3, 8, 1, 2, 7, 4, 6};
  dsa::merge_sort(std::span<int>(b));
  assert((b == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

  std::vector<int> c = {5, 3, 8, 1, 2, 7, 4, 6};
  dsa::quick_sort(std::span<int>(c));
  assert((c == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

  std::vector<int> d = {5, 3, 8, 1, 2, 7, 4, 6};
  assert(dsa::quick_select(std::span<int>(d), 0) == 1);
  assert(dsa::quick_select(std::span<int>(d), 4) == 5);
  assert(dsa::quick_select(std::span<int>(d), 7) == 8);

  std::vector<int> e = {4, 2, 2, 8, 3, 3, 1};
  dsa::counting_sort(e);
  assert((e == std::vector<int>{1, 2, 2, 3, 3, 4, 8}));

  std::vector<int> f = {-3, 1, -1, 0, 2, -5};
  dsa::counting_sort(f);
  assert((f == std::vector<int>{-5, -3, -1, 0, 1, 2}));

  std::vector<int> g = {170, 45, 75, 90, 802, 24, 2, 66};
  dsa::radix_sort(g);
  assert((g == std::vector<int>{2, 24, 45, 66, 75, 90, 170, 802}));

  std::vector<float> h = {0.78f, 0.17f, 0.39f, 0.26f, 0.72f, 0.94f, 0.21f, 0.12f, 0.23f, 0.68f};
  dsa::bucket_sort(std::span<float>(h));
  for (std::size_t i = 1; i < h.size(); ++i)
    assert(h[i - 1] <= h[i]);

  std::vector<int> hs = {5, 3, 8, 1, 2, 7, 4, 6};
  dsa::heap_sort(std::span<int>(hs));
  assert((hs == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

  std::vector<int> single = {42};
  dsa::counting_sort(single);
  assert(single[0] == 42);
  dsa::radix_sort(single);
  assert(single[0] == 42);

  std::vector<int> empty;
  dsa::counting_sort(empty);
  dsa::radix_sort(empty);

  std::cout << "sorting: all passed\n";
}
