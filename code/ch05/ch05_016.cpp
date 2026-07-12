// Solve tridiagonal system:
// a[i]*x[i-1] + b[i]*x[i] + c[i]*x[i+1] = d[i]
// a[0] = 0 (no sub-diagonal for first row)
// c[n-1] = 0 (no super-diagonal for last row)
std::vector<double> thomas_solve(
    std::vector<double> a,  // sub-diagonal (size n, a[0] unused)
    std::vector<double> b,  // main diagonal (size n)
    std::vector<double> c,  // super-diagonal (size n, c[n-1] unused)
    std::vector<double> d   // right-hand side (size n)
) {
    size_t n = b.size();

    // Forward elimination
    for (size_t i = 1; i < n; ++i) {
        double m = a[i] / b[i - 1];
        b[i] -= m * c[i - 1];
        d[i] -= m * d[i - 1];
    }

    // Back substitution
    std::vector<double> x(n);
    x[n - 1] = d[n - 1] / b[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        x[i] = (d[i] - c[i] * x[i + 1]) / b[i];
    }
    return x;
}
