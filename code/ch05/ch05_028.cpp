void reverse(std::vector<int>& a, int lo, int hi) {
    while (lo < hi) std::swap(a[lo++], a[hi--]);
}

void reversalRotate(std::vector<int>& a, int d) {
    int n = a.size();
    d = ((d % n) + n) % n;
    reverse(a, n - d, n - 1);
    reverse(a, 0, n - d - 1);
    reverse(a, 0, n - 1);
}
