bool subset_sum_optimized(std::span<const int> nums, int target) {
    std::vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int num : nums) {
        for (int w = target; w >= num; --w) {
            dp[w] = dp[w] || dp[w - num];
        }
    }
    return dp[target];
}
