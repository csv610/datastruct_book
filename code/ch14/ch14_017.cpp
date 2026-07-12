class fenwick {
    std::vector<int> tree_;
public:
    fenwick(int n) : tree_(n + 1, 0) {}
    void add(int i, int delta) {
        for (++i; i < tree_.size(); i += i & (-i))
            tree_[i] += delta;
    }
    int sum(int i) {
        int s = 0;
        for (++i; i > 0; i -= i & (-i))
            s += tree_[i];
        return s;
    }
    int range_sum(int l, int r) { return sum(r) - sum(l - 1); }
};
