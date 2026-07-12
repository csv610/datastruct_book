int kth_smallest_minheap(std::vector<int> data, int k) {
    // Floyd's O(n) build
    std::make_heap(data.begin(), data.end(), std::greater<int>{});
    for (int i = 0; i < k - 1; ++i) {
        std::pop_heap(data.begin(), data.end(), std::greater<int>{});
        data.pop_back();
    }
    return data.front();  // kth smallest
}
