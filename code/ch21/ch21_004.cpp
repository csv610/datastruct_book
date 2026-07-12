template <typename T>
struct parray_node {
    T value;
    std::shared_ptr<parray_node> left, right;
};

template <typename T>
class persistent_array {
public:
    persistent_array(size_t n, const T& default_val)
        : n_(n), versions_{build(0, n, default_val)} {}

    T get(size_t idx, size_t ver = 0) const {
        return get(versions_[ver], 0, n_, idx);
    }

    persistent_array set(size_t idx, const T& val, size_t ver = 0) {
        persistent_array result(*this);
        result.versions_.push_back(
            set(versions_[ver], 0, n_, idx, val));
        return result;
    }

    size_t size() const { return n_; }
    size_t versions() const { return versions_.size(); }

private:
    size_t n_;
    std::vector<std::shared_ptr<parray_node<T>>> versions_;

    std::shared_ptr<parray_node<T>> build(
            size_t lo, size_t hi, const T& val) {
        if (lo + 1 == hi)
            return std::make_shared<parray_node<T>>(
                parray_node<T>{val, nullptr, nullptr});
        size_t mid = lo + (hi - lo) / 2;
        auto l = build(lo, mid, val);
        auto r = build(mid, hi, val);
        return std::make_shared<parray_node<T>>(
            parray_node<T>{T{}, l, r});
    }

    T get(const std::shared_ptr<parray_node<T>>& node,
          size_t lo, size_t hi, size_t idx) const {
        if (lo + 1 == hi) return node->value;
        size_t mid = lo + (hi - lo) / 2;
        if (idx < mid) return get(node->left, lo, mid, idx);
        else return get(node->right, mid, hi, idx);
    }

    std::shared_ptr<parray_node<T>> set(
            const std::shared_ptr<parray_node<T>>& node,
            size_t lo, size_t hi, size_t idx, const T& val) {
        if (lo + 1 == hi)
            return std::make_shared<parray_node<T>>(
                parray_node<T>{val, nullptr, nullptr});
        size_t mid = lo + (hi - lo) / 2;
        if (idx < mid)
            return std::make_shared<parray_node<T>>(
                parray_node<T>{T{},
                    set(node->left, lo, mid, idx, val),
                    node->right});
        else
            return std::make_shared<parray_node<T>>(
                parray_node<T>{T{},
                    node->left,
                    set(node->right, mid, hi, idx, val)});
    }
};
