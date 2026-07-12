void blockSwap(std::vector<int>& a, int d) {
    int n = a.size();
    d = ((d % n) + n) % n;
    if (d == 0 || d == n) return;
    int i = d, j = n - d;
    while (i != j) {
        if (i < j) {
            std::swap_ranges(a.begin() + d - i, a.begin() + d,
                             a.begin() + d + j - i);
            j -= i;
        } else {
            std::swap_ranges(a.begin() + d, a.begin() + d + i - j,
                             a.begin() + d + i);
            i -= j;
        }
    }
    std::swap_ranges(a.begin() + d - i, a.begin() + d,
                     a.begin() + d + i);
}
