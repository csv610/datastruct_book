int rob(std::span<const int> values) {
    int n = values.size();
    if (n == 0) return 0;
    if (n == 1) return values[0];
    std::vector<int> dp(n);
    dp[0] = values[0];
    dp[1] = std::max(values[0], values[1]);
    for (int i = 2; i < n; ++i)
        dp[i] = std::max(dp[i - 1], dp[i - 2] + values[i]);
    return dp[n - 1];
}
