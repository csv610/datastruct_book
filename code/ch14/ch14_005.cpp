template <typename T, typename Combine = std::plus<T>>
class segment_tree {
public:
    segment_tree(std::span<const T> data, Combine combine = {})
        : n_(data.size()), tree_(4 * n_), combine_(combine) {
        build(data, 1, 0, n_ - 1);
    }

    // Query over range [l, r] inclusive
    T query(size_t l, size_t r) {
        return query(1, 0, n_ - 1, l, r);
    }

    // Point update: set position pos to value
    void update(size_t pos, const T& value) {
        update(1, 0, n_ - 1, pos, value);
    }

private:
    void build(std::span<const T> data, size_t node, size_t l, size_t r) {
        if (l == r) {
            tree_[node] = data[l];
            return;
        }
        size_t mid = l + (r - l) / 2;
        build(data, 2 * node, l, mid);
        build(data, 2 * node + 1, mid + 1, r);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    T query(size_t node, size_t l, size_t r, size_t ql, size_t qr) {
        if (ql > r || qr < l) return T{};  // no overlap (identity for combine)
        if (ql <= l && r <= qr) return tree_[node];  // total overlap
        size_t mid = l + (r - l) / 2;
        auto left = query(2 * node, l, mid, ql, qr);
        auto right = query(2 * node + 1, mid + 1, r, ql, qr);
        return combine_(left, right);
    }

    void update(size_t node, size_t l, size_t r, size_t pos, const T& value) {
        if (l == r) {
            tree_[node] = value;
            return;
        }
        size_t mid = l + (r - l) / 2;
        if (pos <= mid) update(2 * node, l, mid, pos, value);
        else update(2 * node + 1, mid + 1, r, pos, value);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    size_t n_;
    std::vector<T> tree_;
    Combine combine_;
};
