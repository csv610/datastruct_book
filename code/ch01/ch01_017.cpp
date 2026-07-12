std::vector<int> v;           // empty
std::vector<int> v(10);       // 10 elements, default-initialized
std::vector<int> v = {1,2,3}; // initializer list

v.push_back(4);               // add to end: O(1) amortized
v.pop_back();                 // remove from end: O(1)
v[0] = 10;                    // O(1) index
v.size();                     // number of elements
v.empty();                    // true if size == 0
