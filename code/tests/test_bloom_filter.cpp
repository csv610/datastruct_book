#include "bloom_filter.h"
#include <cassert>
#include <iostream>

int main() {
  dsa::bloom_filter<1024, 3> bf;
  bf.insert("hello");
  bf.insert("world");
  bf.insert("foo");

  assert(bf.might_contain("hello"));
  assert(bf.might_contain("world"));
  assert(bf.might_contain("foo"));
  assert(!bf.might_contain("bar"));

  dsa::bloom_filter<1024, 3> bf2;
  bf2.insert("hello");
  bf2.insert("baz");
  bf.merge_with(bf2);
  assert(bf.might_contain("hello"));
  assert(bf.might_contain("baz"));
  assert(bf.might_contain("world"));

  dsa::bloom_filter<1024, 3> bf3;
  bf3.insert("hello");
  bf3.insert("world");
  bf3.intersect_with(bf);
  assert(bf3.might_contain("hello"));
  assert(bf3.might_contain("world"));
  assert(!bf3.might_contain("foo"));

  double rate = bf.false_positive_rate();
  assert(rate >= 0.0 && rate <= 1.0);

  std::cout << "bloom_filter: all passed\n";
}
