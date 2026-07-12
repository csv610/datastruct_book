int rob_optimized(std::span<const int> values) {
    int prev2 = 0, prev1 = 0;
    for (int v : values) {
        int curr = std::max(prev1, prev2 + v);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
