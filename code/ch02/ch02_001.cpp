// Find the maximum element
int find_max(const std::vector<int>& a) {
    int max = a[0];                          // 1
    for (size_t i = 1; i < a.size(); ++i) {  // n-1 iterations
        if (a[i] > max) max = a[i];          // 2 ops per iteration
    }
    return max;
}
