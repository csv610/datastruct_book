int maxSubarraySum(const std::vector<int>& a) {
    int cur = 0, best = INT_MIN;
    for (int x : a) {
        cur = std::max(x, cur + x);
        best = std::max(best, cur);
    }
    return best;
}
