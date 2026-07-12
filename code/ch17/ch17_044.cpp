int lis_dp(std::span<const int> nums) {
    int n = nums.size();
    std::vector<int> dp(n, 1);  // every element is an LIS of length 1
    int best = 1;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i])
                dp[i] = std::max(dp[i], dp[j] + 1);
        }
        best = std::max(best, dp[i]);
    }
    return best;
}
