template <typename T, typename Compare = std::less<T>>
void build_heap(std::vector<T>& data, Compare comp = {}) {
    int n = static_cast<int>(data.size());
    // Start from last non-leaf node, sift down to root
    for (int i = n / 2 - 1; i >= 0; --i) {
        int current = i;
        while (true) {
            int largest = current;
            int left  = 2 * current + 1;
            int right = 2 * current + 2;
            if (left < n && comp(data[largest], data[left]))
                largest = left;
            if (right < n && comp(data[largest], data[right]))
                largest = right;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }
}
