void jugglingRotate(std::vector<int>& a, int d) {
    int n = a.size();
    d = ((d % n) + n) % n;  // normalise
    int g = std::gcd(n, d);
    for (int i = 0; i < g; ++i) {
        int tmp = a[i];
        int j = i;
        while (true) {
            int k = (j + d) % n;
            if (k == i) break;
            a[j] = a[k];
            j = k;
        }
        a[j] = tmp;
    }
}
