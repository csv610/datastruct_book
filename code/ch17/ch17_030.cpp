std::string edit_ops(std::string_view a, std::string_view b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    // ... fill dp table as above ...

    std::string ops;
    int i = m, j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && a[i-1] == b[j-1]) {
            --i; --j;  // match, no operation
        } else if (i > 0 && j > 0 && dp[i][j] == dp[i-1][j-1] + 1) {
            ops += "SUB " + std::string(1, a[i-1]) + "->" + std::string(1, b[j-1]) + "\n";
            --i; --j;
        } else if (i > 0 && dp[i][j] == dp[i-1][j] + 1) {
            ops += "DEL " + std::string(1, a[i-1]) + "\n";
            --i;
        } else {
            ops += "INS " + std::string(1, b[j-1]) + "\n";
            --j;
        }
    }
    std::reverse(ops.begin(), ops.end());
    return ops;
}
