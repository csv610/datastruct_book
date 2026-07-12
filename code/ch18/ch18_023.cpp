class subset_sum_dp {
public:
    bool has_solution(const std::vector<int>& vals, int target) {
        int n = vals.size();
        std::vector<std::vector<bool>> dp(n + 1,
            std::vector<bool>(target + 1, false));

        for (int i = 0; i <= n; ++i) dp[i][0] = true;

        for (int i = 1; i <= n; ++i) {
            for (int s = 0; s <= target; ++s) {
                dp[i][s] = dp[i - 1][s];
                if (s >= vals[i - 1]) {
                    dp[i][s] = dp[i][s]
                             || dp[i - 1][s - vals[i - 1]];
                }
            }
        }
        return dp[n][target];
    }
};
