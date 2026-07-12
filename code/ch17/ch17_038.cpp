#include <vector>
#include <span>

bool subset_sum(std::span<const int> nums, int target) {
    int n = nums.size();
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(target + 1, false));
    dp[0][0] = true;

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= target; ++w) {
            dp[i][w] = dp[i - 1][w];  // skip nums[i-1]
            if (w >= nums[i - 1]) {
                dp[i][w] = dp[i][w] || dp[i - 1][w - nums[i - 1]];
            }
        }
    }
    return dp[n][target];
}
