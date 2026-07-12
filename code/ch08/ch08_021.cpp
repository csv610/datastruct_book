std::unordered_map<std::string, int> map;
map["apple"] = 5;
map["banana"] = 3;

auto it = map.find("apple");
if (it != map.end()) {
    std::cout << it->second;  // 5
}

// Iterate (order is unspecified)
for (const auto& [key, value] : map) {
    std::cout << key << ": " << value << '\n';
}
