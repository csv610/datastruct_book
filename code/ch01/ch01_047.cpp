std::vector<Buffer> v;
v.push_back(Buffer(100));
// vector may reallocate: elements are moved, not copied
// This is fast because Buffer has a noexcept move constructor
