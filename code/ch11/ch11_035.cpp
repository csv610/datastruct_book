std::map<std::string, int> m;
m["apple"] = 5;
m["banana"] = 3;

auto it = m.find("apple");
if (it != m.end()) {
    std::cout << it->first << ": " << it->second;
}

// Ordered iteration (guaranteed by the standard)
for (const auto& [key, value] : m) {
    std::cout << key << " " << value << '\n';
}

// Range queries
auto [lo, hi] = m.equal_range("b");
// Iterates all keys starting with "b"
