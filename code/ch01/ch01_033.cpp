std::map<std::string, int> ages;
ages["Alice"] = 30;
ages["Bob"] = 25;
auto it = ages.find("Alice");  // O(log n)
if (it != ages.end()) {
    // it->second == 30
}

std::set<int> s = {5, 3, 1, 4, 2};
// s is {1, 2, 3, 4, 5} -- always sorted
s.insert(6);   // O(log n)
s.erase(3);    // O(log n)
