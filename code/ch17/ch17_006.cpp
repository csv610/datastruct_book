std::vector<size_t> knapsack_items(std::span<const int> weights,
                                    std::span<const int> values, int capacity) {
    int n = weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    // ... fill dp table as above ...

    std::vector<size_t> selected;
    int w = capacity;
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(i - 1);
            w -= weights[i - 1];
        }
    }
    return selected;
}
