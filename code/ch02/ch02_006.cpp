int binary_search(const std::vector<int>& a, int target, int low, int high) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (a[mid] == target) return mid;
    if (a[mid] > target) return binary_search(a, target, low, mid - 1);
    return binary_search(a, target, mid + 1, high);
}
