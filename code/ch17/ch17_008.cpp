int matrix_chain_order(std::span<const int> dims) {
    int n = dims.size() - 1;  // number of matrices
    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));

    for (int len = 2; len <= n; ++len) {      // chain length
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = m[i][k] + m[k + 1][j]
                         + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < m[i][j]) m[i][j] = cost;
            }
        }
    }
    return m[0][n - 1];
}
