#include <vector>
#include <span>
#include <string>

std::string matrix_chain_parens(std::span<const int> p) {
    int n = p.size() - 1;
    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));
    std::vector<std::vector<int>> s(n, std::vector<int>(n, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = m[i][k] + m[k + 1][j]
                         + p[i] * p[k + 1] * p[j + 1];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }

    // Recursive helper to build the string
    auto build = [&](auto&& self, int i, int j) -> std::string {
        if (i == j) return "A" + std::to_string(i + 1);
        int k = s[i][j];
        return "(" + self(self, i, k) + " x "
                   + self(self, k + 1, j) + ")";
    };
    return build(build, 0, n - 1);
}
