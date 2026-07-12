struct result { int left, right, sum; };

result max_crossing(std::span<const int> arr, int lo, int mid, int hi) {
    int left_sum = std::numeric_limits<int>::min();
    int sum = 0, best_left = mid;
    for (int i = mid; i >= lo; --i) {
        sum += arr[i];
        if (sum > left_sum) { left_sum = sum; best_left = i; }
    }

    int right_sum = std::numeric_limits<int>::min();
    sum = 0;
    int best_right = mid + 1;
    for (int j = mid + 1; j <= hi; ++j) {
        sum += arr[j];
        if (sum > right_sum) { right_sum = sum; best_right = j; }
    }
    return {best_left, best_right, left_sum + right_sum};
}

result max_subarray(std::span<const int> arr, int lo, int hi) {
    if (lo == hi) return {lo, hi, arr[lo]};

    int mid = lo + (hi - lo) / 2;
    auto left  = max_subarray(arr, lo, mid);
    auto right = max_subarray(arr, mid + 1, hi);
    auto cross = max_crossing(arr, lo, mid, hi);

    if (left.sum >= right.sum && left.sum >= cross.sum) return left;
    if (right.sum >= left.sum && right.sum >= cross.sum) return right;
    return cross;
}
