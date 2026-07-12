long long merge_and_count(std::vector<int>& arr, std::vector<int>& tmp,
                         int lo, int mid, int hi) {
    long long inversions = 0;
    int i = lo, j = mid, k = lo;

    while (i < mid && j <= hi) {
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
            inversions += mid - i;  // all remaining left > arr[j]
        }
    }
    while (i < mid) tmp[k++] = arr[i++];
    while (j <= hi)  tmp[k++] = arr[j++];

    for (int i = lo; i <= hi; ++i) arr[i] = tmp[i];
    return inversions;
}

long long count_inversions(std::vector<int>& arr, std::vector<int>& tmp,
                           int lo, int hi) {
    if (lo >= hi) return 0;
    int mid = lo + (hi - lo) / 2;
    long long inv = 0;
    inv += count_inversions(arr, tmp, lo, mid);
    inv += count_inversions(arr, tmp, mid + 1, hi);
    inv += merge_and_count(arr, tmp, lo, mid, hi);
    return inv;
}
