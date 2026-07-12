int kth_largest_maxheap(std::span<int> data, int k) {
    // Build max-heap of first k elements
    std::vector<int> heap(data.begin(), data.begin() + k);
    std::make_heap(heap.begin(), heap.end());

    for (int i = k; i < static_cast<int>(data.size()); ++i) {
        if (data[i] < heap.front()) {
            heap.front() = data[i];
            std::sift_down_heap(heap.begin(), heap.end());
        }
    }
    return heap.front();  // kth largest
}
