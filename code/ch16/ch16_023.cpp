result kadane(std::span<const int> arr) {
    int best_sum = arr[0], cur_sum = arr[0];
    int best_start = 0, best_end = 0, cur_start = 0;

    for (int i = 1; i < (int)arr.size(); ++i) {
        if (cur_sum < 0) { cur_sum = arr[i]; cur_start = i; }
        else { cur_sum += arr[i]; }

        if (cur_sum > best_sum) {
            best_sum = cur_sum;
            best_start = cur_start;
            best_end = i;
        }
    }
    return {best_start, best_end, best_sum};
}
