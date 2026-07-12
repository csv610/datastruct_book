struct SegTreeBeats {
    int n;
    vector<long long> sum;
    vector<int> mx, se, cmx;

    void build(const vector<int>& a, int node, int lo, int hi) {
        if (lo == hi) {
            sum[node] = mx[node] = a[lo];
            se[node] = -1; // -1 means no second max
            cmx[node] = 1;
            return;
        }
        int mid = (lo + hi) / 2;
        build(a, 2*node, lo, mid);
        build(a, 2*node+1, mid+1, hi);
        pull(node);
    }

    void pull(int node) {
        int l = 2*node, r = 2*node+1;
        sum[node] = sum[l] + sum[r];

        // Merge max and second max from children
        if (mx[l] == mx[r]) {
            mx[node] = mx[l];
            cmx[node] = cmx[l] + cmx[r];
            se[node] = max(se[l], se[r]);
        } else if (mx[l] > mx[r]) {
            mx[node] = mx[l];
            cmx[node] = cmx[l];
            se[node] = max(se[l], mx[r]);
        } else {
            mx[node] = mx[r];
            cmx[node] = cmx[r];
            se[node] = max(mx[l], se[r]);
        }
    }

    // Apply chmin: reduce all elements equal to old_mx down to v
    void apply_chmin(int node, int v) {
        sum[node] -= (long long)(mx[node] - v) * cmx[node];
        mx[node] = v;
    }

    void push(int node, int lo, int hi) {
        if (lo == hi) return;
        // Push chmin to children if needed
        for (int child : {2*node, 2*node+1}) {
            if (mx[child] > mx[node]) {
                apply_chmin(child, mx[node]);
            }
        }
    }

    void range_chmin(int node, int lo, int hi,
                     int ql, int qr, int v) {
        if (ql > hi || qr < lo || mx[node] <= v)
            return;
        if (ql <= lo && hi <= qr && se[node] < v) {
            apply_chmin(node, v);
            return;
        }
        push(node, lo, hi);
        int mid = (lo + hi) / 2;
        range_chmin(2*node, lo, mid, ql, qr, v);
        range_chmin(2*node+1, mid+1, hi, ql, qr, v);
        pull(node);
    }

    long long range_sum(int node, int lo, int hi,
                        int ql, int qr) {
        if (ql > hi || qr < lo) return 0;
        if (ql <= lo && hi <= qr) return sum[node];
        push(node, lo, hi);
        int mid = (lo + hi) / 2;
        return range_sum(2*node, lo, mid, ql, qr)
             + range_sum(2*node+1, mid+1, hi, ql, qr);
    }

    // Public interface
    SegTreeBeats(const vector<int>& a) {
        n = a.size();
        int size = 4 * n;
        sum.resize(size);
        mx.resize(size);
        se.resize(size);
        cmx.resize(size);
        build(a, 1, 0, n - 1);
    }

    void chmin(int l, int r, int v) {
        range_chmin(1, 0, n - 1, l, r, v);
    }

    long long query(int l, int r) {
        return range_sum(1, 0, n - 1, l, r);
    }
};
