struct pst_node {
    int sum = 0;
    int left = -1;   // index in node pool
    int right = -1;
};

class persistent_seg_tree {
public:
    explicit persistent_seg_tree(std::span<const int> data)
        : n_(data.size()) {
        versions_.push_back(build(data, 0, n_ - 1));
    }

    // Create new version by updating pos with val
    int update(int version, size_t pos, int val) {
        int new_root = update(versions_[version], 0, n_ - 1, pos, val);
        versions_.push_back(new_root);
        return versions_.size() - 1;  // return version index
    }

    // Query sum over [l, r] on a given version
    int query(int version, size_t l, size_t r) {
        return query(versions_[version], 0, n_ - 1, l, r);
    }

    int num_versions() const { return versions_.size(); }

private:
    std::vector<pst_node> pool_;
    std::vector<int> versions_;  // root index for each version
    size_t n_;

    int new_node(int sum, int left, int right) {
        pool_.push_back({sum, left, right});
        return pool_.size() - 1;
    }

    int build(std::span<const int> data, size_t l, size_t r) {
        if (l == r) return new_node(data[l], -1, -1);
        size_t mid = l + (r - l) / 2;
        int lc = build(data, l, mid);
        int rc = build(data, mid + 1, r);
        return new_node(pool_[lc].sum + pool_[rc].sum, lc, rc);
    }

    int update(int node, size_t l, size_t r, size_t pos, int val) {
        if (l == r) return new_node(val, -1, -1);
        size_t mid = l + (r - l) / 2;
        int lc = pool_[node].left;
        int rc = pool_[node].right;
        if (pos <= mid) lc = update(lc, l, mid, pos, val);
        else            rc = update(rc, mid + 1, r, pos, val);
        return new_node(pool_[lc].sum + pool_[rc].sum, lc, rc);
    }

    int query(int node, size_t l, size_t r, size_t ql, size_t qr) {
        if (node == -1 || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return pool_[node].sum;
        size_t mid = l + (r - l) / 2;
        return query(pool_[node].left, l, mid, ql, qr)
             + query(pool_[node].right, mid + 1, r, ql, qr);
    }
};
