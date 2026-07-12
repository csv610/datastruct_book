std::vector<int> lis_with_sequence(std::span<const int> nums) {
    int n = nums.size();
    std::vector<int> tails;
    std::vector<int> tail_indices;   // index in nums of each tail
    std::vector<int> prev(n, -1);    // predecessor of nums[i] in LIS
    std::vector<int> dp(n, 1);

    for (int i = 0; i < n; ++i) {
        auto it = std::lower_bound(tails.begin(), tails.end(), nums[i]);
        int pos = it - tails.begin();
        if (it == tails.end()) {
            tails.push_back(nums[i]);
            tail_indices.push_back(i);
        } else {
            *it = nums[i];
            tail_indices[pos] = i;
        }
        dp[i] = pos + 1;
        prev[i] = (pos > 0) ? tail_indices[pos - 1] : -1;
    }

    // Reconstruct by following predecessors from the last element
    std::vector<int> result;
    int k = tail_indices[tails.size() - 1];
    while (k >= 0) {
        result.push_back(nums[k]);
        k = prev[k];
    }
    std::reverse(result.begin(), result.end());
    return result;
}
