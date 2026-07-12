int binary_search(const std::vector<int>& a, int target, int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target) low = mid + 1;
        else                 high = mid - 1;
    }
    return -1;
}
