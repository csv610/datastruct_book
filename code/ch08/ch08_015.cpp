// Abseil flat_hash_map -- the fastest general-purpose hash table
#include <absl/container/flat_hash_map.h>

absl::flat_hash_map<std::string, int> map;
map["hello"] = 1;
map["world"] = 2;
