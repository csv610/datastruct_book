int quick_select(std::span<int> arr, size_t k) {
    if (arr.size() == 1) return arr[0];

    size_t p = random_partition(arr);

    if (k == p) return arr[p];
    if (k < p) return quick_select(arr.subspan(0, p), k);
    return quick_select(arr.subspan(p + 1), k - p - 1);
}
