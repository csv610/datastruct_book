struct SubarrayResult {
    int sum;
    int start;
    int end;
};

SubarrayResult maxSubarray(const std::vector<int>& a) {
    int cur = 0, best = INT_MIN;
    int curStart = 0, bestStart = 0, bestEnd = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (cur < 0) {
            cur = a[i];
            curStart = i;
        } else {
            cur += a[i];
        }
        if (cur > best) {
            best = cur;
            bestStart = curStart;
            bestEnd = i;
        }
    }
    return {best, bestStart, bestEnd};
}
