int coin_change(std::span<const int> coins, int amount) {
    std::vector<int> dp(amount + 1, INT_MAX / 2);
    dp[0] = 0;

    for (int c = 1; c <= amount; ++c) {
        for (int coin : coins) {
            if (coin <= c) {
                dp[c] = std::min(dp[c], dp[c - coin] + 1);
            }
        }
    }
    return dp[amount] == INT_MAX / 2 ? -1 : dp[amount];
}
