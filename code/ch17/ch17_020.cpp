int count_ways(std::span<const int> coins, int amount) {
    std::vector<int> dp(amount + 1, 0);
    dp[0] = 1;
    for (int coin : coins)
        for (int c = coin; c <= amount; ++c)
            dp[c] += dp[c - coin];
    return dp[amount];
}
