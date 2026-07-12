size_t partition(std::span<int> arr) {
    // Lomuto partition scheme
    int pivot = arr.back();
    size_t i = 0;
    for (size_t j = 0; j < arr.size() - 1; ++j) {
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr.back());
    return i;
}

void quick_sort(std::span<int> arr) {
    if (arr.size() <= 1) return;
    size_t p = partition(arr);
    quick_sort(arr.subspan(0, p));
    quick_sort(arr.subspan(p + 1));
}
