#include "cuckoo_filter.h"
#include <cassert>
#include <print>
#include <string>

int main() {
  dsa::cuckoo_filter filter(16);
  for (int i = 0; i < 20; ++i) assert(filter.insert("key-" + std::to_string(i)));
  for (int i = 0; i < 20; ++i) assert(filter.contains("key-" + std::to_string(i)));
  assert(filter.erase("key-7"));
  assert(!filter.contains("key-7"));
  assert(filter.size() == 19);
  std::print("cuckoo filter: all passed\n");
}
