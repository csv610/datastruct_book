struct Fenwick2D {
    int n, m;
    vector<vector<long long>> tree;

    Fenwick2D(int n, int m) : n(n), m(m),
        tree(n + 1, vector<long long>(m + 1, 0)) {}

    // Add val at position (x, y) (1-indexed)
    void update(int x, int y, long long val) {
        for (int i = x; i <= n; i += i & (-i))
            for (int j = y; j <= m; j += j & (-j))
                tree[i][j] += val;
    }

    // Prefix sum of rectangle [1..x] x [1..y]
    long long query(int x, int y) {
        long long res = 0;
        for (int i = x; i > 0; i -= i & (-i))
            for (int j = y; j > 0; j -= j & (-j))
                res += tree[i][j];
        return res;
    }

    // Sum over rectangle [x1..x2] x [y1..y2]
    long long query(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x1 - 1, y2)
             - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
    }
};
