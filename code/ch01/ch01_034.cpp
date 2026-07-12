std::unordered_map<int, std::string> names;
names[1] = "Alice";
names[2] = "Bob";
auto it = names.find(1);  // O(1) average
