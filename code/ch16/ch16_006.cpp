template <typename T>
int binary_search_rec(const std::vector<T>& arr, int low, int high, const T& target) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] > target) return binary_search_rec(arr, low, mid - 1, target);
    return binary_search_rec(arr, mid + 1, high, target);
}
