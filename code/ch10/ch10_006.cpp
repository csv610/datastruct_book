template <typename T, typename Compare = std::less<T>>
void heapify(std::vector<T>& data, Compare compare = {}) {
    int n = data.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        sift_down(data, i, n, compare);
    }
}
