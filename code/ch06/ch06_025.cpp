#include <vector>
#include <stack>
#include <utility>

bool dfs_maze(const std::vector<std::vector<bool>>& wall,
              int n, int m) {
    std::stack<std::pair<int,int>> stk;
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m));
    stk.push({0, 0});
    visited[0][0] = true;

    const int dr[] = {1, 0};  // down, right
    const int dc[] = {0, 1};

    while (!stk.empty()) {
        auto [r, c] = stk.top();
        stk.pop();
        if (r == n - 1 && c == m - 1) return true;

        for (int d = 0; d < 2; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < n && nc < m && !wall[nr][nc] &&
                !visited[nr][nc]) {
                visited[nr][nc] = true;
                stk.push({nr, nc});
            }
        }
    }
    return false;
}
