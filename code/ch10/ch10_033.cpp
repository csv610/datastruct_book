std::vector<int> v = {30, 10, 50, 20, 40};
std::make_heap(v.begin(), v.end());         // O(n)
std::pop_heap(v.begin(), v.end());          // move max to end, O(log n)
v.pop_back();
std::push_heap(v.begin(), v.end());         // after push_back, O(log n)
std::sort_heap(v.begin(), v.end());         // O(n log n)
