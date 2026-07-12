template <typename K, typename V>
struct pnode {
    K key;
    V val;
    int height;
    std::shared_ptr<const pnode<K, V>> left, right;
};

template <typename K, typename V>
class persistent_avl {
public:
    persistent_avl() : root_(nullptr) {}

    persistent_avl insert(const K& k, const V& v) const {
        return persistent_avl(insert(root_, k, v));
    }

    bool contains(const K& k) const {
        return find(root_, k) != nullptr;
    }

    const V& get(const K& k) const {
        auto node = find(root_, k);
        return node->val;
    }

    size_t size() const { return count(root_); }

private:
    std::shared_ptr<const pnode<K, V>> root_;

    explicit persistent_avl(
        std::shared_ptr<const pnode<K, V>> r) : root_(r) {}

    static int ht(
        const std::shared_ptr<const pnode<K, V>>& n) {
        return n ? n->height : 0;
    }

    static int bf(
        const std::shared_ptr<const pnode<K, V>>& n) {
        return n ? ht(n->left) - ht(n->right) : 0;
    }

    static std::shared_ptr<const pnode<K, V>> make(
        const K& k, const V& v,
        std::shared_ptr<const pnode<K, V>> l,
        std::shared_ptr<const pnode<K, V>> r) {
        return std::make_shared<const pnode<K, V>>(
            pnode<K, V>{k, v, std::max(ht(l), ht(r)) + 1, l, r});
    }

    static std::shared_ptr<const pnode<K, V>> rotate_l(
        std::shared_ptr<const pnode<K, V>> n) {
        auto y = n->right;
        return make(y->key, y->val, make(n->key, n->val, n->left, y->left), y->right);
    }

    static std::shared_ptr<const pnode<K, V>> rotate_r(
        std::shared_ptr<const pnode<K, V>> n) {
        auto y = n->left;
        return make(y->key, y->val, y->left, make(n->key, n->val, y->right, n->right));
    }

    static std::shared_ptr<const pnode<K, V>> rebalance(
        std::shared_ptr<const pnode<K, V>> n) {
        if (!n) return n;
        if (bf(n) > 1) {
            if (bf(n->left) < 0)
                n = make(n->key, n->val, rotate_l(n->left), n->right);
            return rotate_r(n);
        }
        if (bf(n) < -1) {
            if (bf(n->right) > 0)
                n = make(n->key, n->val, n->left, rotate_r(n->right));
            return rotate_l(n);
        }
        return n;
    }

    static std::shared_ptr<const pnode<K, V>> insert(
        const std::shared_ptr<const pnode<K, V>>& n,
        const K& k, const V& v) {
        if (!n) return make(k, v, nullptr, nullptr);
        if (k < n->key)
            return rebalance(make(n->key, n->val, insert(n->left, k, v), n->right));
        if (k > n->key)
            return rebalance(make(n->key, n->val, n->left, insert(n->right, k, v)));
        return make(k, v, n->left, n->right);
    }

    static std::shared_ptr<const pnode<K, V>> find(
        const std::shared_ptr<const pnode<K, V>>& n, const K& k) {
        if (!n) return nullptr;
        if (k < n->key) return find(n->left, k);
        if (k > n->key) return find(n->right, k);
        return n;
    }

    static size_t count(
        const std::shared_ptr<const pnode<K, V>>& n) {
        if (!n) return 0;
        return 1 + count(n->left) + count(n->right);
    }
};
