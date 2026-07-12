#include <vector>

enum state { UNVISITED, IN_STACK, DONE };

bool has_cycle_dfs(int u, const std::vector<std::vector<int>>& adj,
                   std::vector<state>& vis) {
    vis[u] = IN_STACK;
    for (int v : adj[u]) {
        if (vis[v] == IN_STACK) return true;   // back edge -> cycle
        if (vis[v] == UNVISITED &&
            has_cycle_dfs(v, adj, vis))
            return true;
    }
    vis[u] = DONE;
    return false;
}

bool has_cycle(int n, const std::vector<std::vector<int>>& adj) {
    std::vector<state> vis(n, UNVISITED);
    for (int i = 0; i < n; ++i) {
        if (vis[i] == UNVISITED && has_cycle_dfs(i, adj, vis))
            return true;
    }
    return false;
}
