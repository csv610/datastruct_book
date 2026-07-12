// O(1) extra space
int sum(const std::vector<int>& a) {
    int total = 0;
    for (int x : a) total += x;
    return total;
}

// O(n) extra space
std::vector<int> duplicate(const std::vector<int>& a) {
    std::vector<int> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = a[i];
    return result;
}

// O(n) stack space (recursive)
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);  // n stack frames
}
