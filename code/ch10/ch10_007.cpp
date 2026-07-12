template <typename T>
void heap_sort(std::span<T> data) {
    // Build max-heap
    int n = data.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        // sift_down
        int current = i;
        while (true) {
            int largest = current;
            int left = 2 * current + 1;
            int right = 2 * current + 2;
            if (left < n && data[left] > data[largest]) largest = left;
            if (right < n && data[right] > data[largest]) largest = right;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }

    // Extract elements in sorted order
    for (int i = n - 1; i > 0; --i) {
        std::swap(data[0], data[i]);
        // sift_down on reduced heap
        int current = 0;
        int heap_size = i;
        while (true) {
            int largest = current;
            int left = 2 * current + 1;
            int right = 2 * current + 2;
            if (left < heap_size && data[left] > data[largest]) largest = left;
            if (right < heap_size && data[right] > data[largest]) largest = right;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }
}
