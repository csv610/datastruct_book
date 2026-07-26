#include "hash_table.h"
#include <cassert>
#include <string>
#include <print>

int main() {
  dsa::hash_table_chaining<std::string, int> ht;
  assert(ht.empty());

  ht.insert("apple", 1);
  ht.insert("banana", 2);
  ht.insert("cherry", 3);
  assert(ht.size() == 3);
  assert(ht.contains("apple"));
  assert(!ht.contains("date"));

  auto v = ht.find("banana");
  assert(v.has_value());
  assert(v.value() == 2);

  ht["date"] = 4;
  assert(ht.size() == 4);
  assert(ht["date"] == 4);

  ht.insert("apple", 10);
  assert(ht.size() == 4);
  assert(ht.find("apple").value() == 10);

  ht.erase("banana");
  assert(!ht.contains("banana"));
  assert(ht.size() == 3);

  for (int i = 0; i < 200; ++i)
    ht.insert("key" + std::to_string(i), i);
  assert(ht.size() == 203);
  assert(ht.find("key42").has_value());
  assert(ht.find("key42").value() == 42);
  assert(ht.bucket_count() > 16);

  ht.clear();
  assert(ht.empty());

  std::print("hash_table: all passed\n");
}
