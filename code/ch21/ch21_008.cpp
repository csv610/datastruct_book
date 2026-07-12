struct pst_node {
    int sum;
    std::shared_ptr<const pst_node> left, right;
};

using pst_ptr = std::shared_ptr<const pst_node>;

class persistent_seg {
public:
    persistent_seg(size_t n, const std::vector<int>& init)
        : n_(n), roots_{build(init, 0, n)} {}

    int query(int ver, int l, int r) const {
        return query(roots_[ver], 0, n_, l, r);
    }

    int update(int ver, int pos, int val) const {
        int old = query(ver, pos, pos);
        roots_.push_back(update(roots_[ver], 0, n_, pos, val - old));
        return roots_.size() - 1;
    }

    size_t versions() const { return roots_.size(); }

private:
    size_t n_;
    std::vector<pst_ptr> roots_;

    pst_ptr build(const std::vector<int>& a, int lo, int hi) {
        if (lo + 1 == hi)
            return std::make_shared<pst_node>(
                pst_node{lo < (int)a.size() ? a[lo] : 0, nullptr, nullptr});
        int mid = lo + (hi - lo) / 2;
        auto l = build(a, lo, mid);
        auto r = build(a, mid, hi);
        return std::make_shared<pst_node>(
            pst_node{l->sum + r->sum, l, r});
    }

    int query(const pst_ptr& node, int lo, int hi, int l, int r) const {
        if (!node || r <= lo || hi <= l) return 0;
        if (l <= lo && hi <= r) return node->sum;
        int mid = lo + (hi - lo) / 2;
        return query(node->left, lo, mid, l, r)
             + query(node->right, mid, hi, l, r);
    }

    pst_ptr update(const pst_ptr& node, int lo, int hi, int pos, int diff) const {
        if (lo + 1 == hi)
            return std::make_shared<pst_node>(
                pst_node{node->sum + diff, nullptr, nullptr});
        int mid = lo + (hi - lo) / 2;
        if (pos < mid)
            return std::make_shared<pst_node>(
                pst_node{node->sum + diff,
                    update(node->left, lo, mid, pos, diff),
                    node->right});
        else
            return std::make_shared<pst_node>(
                pst_node{node->sum + diff,
                    node->left,
                    update(node->right, mid, hi, pos, diff)});
    }
};
