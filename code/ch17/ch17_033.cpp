#include <vector>
#include <span>
#include <climits>

int matrix_chain_order(std::span<const int> p) {
    int n = p.size() - 1;
    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = m[i][k] + m[k + 1][j]
                         + p[i] * p[k + 1] * p[j + 1];
                if (cost < m[i][j]) m[i][j] = cost;
            }
        }
    }
    return m[0][n - 1];
}
