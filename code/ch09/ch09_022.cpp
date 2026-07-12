#include <vector>
#include <algorithm>

struct LCA {
    int n, LOG;
    std::vector<std::vector<int>> adj;
    std::vector<std::vector<int>> up;   // up[k][v] = 2^k-th ancestor
    std::vector<int> depth;

    LCA(int n, const std::vector<std::vector<int>>& g, int root = 0)
        : n(n), adj(g) {
        LOG = 0;
        while ((1 << LOG) <= n) ++LOG;
        up.assign(LOG, std::vector<int>(n, -1));
        depth.assign(n, 0);
        dfs(root, -1);
    }

    void dfs(int v, int p) {
        up[0][v] = p;
        for (int k = 1; k < LOG; ++k) {
            int mid = up[k - 1][v];
            up[k][v] = (mid == -1) ? -1 : up[k - 1][mid];
        }
        for (int to : adj[v]) {
            if (to == p) continue;
            depth[to] = depth[v] + 1;
            dfs(to, v);
        }
    }

    // Lift node v up by delta levels
    int lift(int v, int delta) const {
        for (int k = 0; k < LOG; ++k) {
            if (delta & (1 << k))
                v = up[k][v];
        }
        return v;
    }

    // LCA of u and v
    int lca(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);
        u = lift(u, depth[u] - depth[v]); // equalise depths
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][u] != up[k][v]) {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u]; // parent of both
    }
};
