template <typename K, typename V>
class avl_tree {
    struct avl_node {
        K key;
        V value;
        std::unique_ptr<avl_node> left;
        std::unique_ptr<avl_node> right;
        int height = 1;

        avl_node(const K& k, const V& v) : key(k), value(v) {}
    };

    using node_ptr = std::unique_ptr<avl_node>;

    int height(const avl_node* n) const {
        return n ? n->height : 0;
    }

    int balance_factor(const avl_node* n) const {
        return height(n->left.get()) - height(n->right.get());
    }

    void update_height(avl_node* n) {
        n->height = 1 + std::max(height(n->left.get()), height(n->right.get()));
    }

    // Right rotation
    node_ptr rotate_right(node_ptr z) {
        auto y = std::move(z->left);
        z->left = std::move(y->right);
        update_height(z.get());
        y->right = std::move(z);
        update_height(y.get());
        return y;
    }

    // Left rotation
    node_ptr rotate_left(node_ptr z) {
        auto y = std::move(z->right);
        z->right = std::move(y->left);
        update_height(z.get());
        y->left = std::move(z);
        update_height(y.get());
        return y;
    }

    // Rebalance: apply rotations if needed
    node_ptr rebalance(node_ptr n) {
        if (!n) return nullptr;
        update_height(n.get());
        int bf = balance_factor(n.get());

        if (bf > 1) {
            // Left-heavy: check left child's balance
            if (balance_factor(n->left.get()) < 0) {
                n->left = rotate_left(std::move(n->left));
            }
            return rotate_right(std::move(n));
        }
        if (bf < -1) {
            // Right-heavy: check right child's balance
            if (balance_factor(n->right.get()) > 0) {
                n->right = rotate_right(std::move(n->right));
            }
            return rotate_left(std::move(n));
        }
        return n;
    }

public:
    void insert(const K& key, const V& value) {
        root_ = insert(std::move(root_), key, value);
    }

    void erase(const K& key) {
        root_ = erase(std::move(root_), key);
    }

    std::optional<V> find(const K& key) const {
        auto* n = find(root_.get(), key);
        return n ? std::optional<V>(n->value) : std::nullopt;
    }

private:
    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<avl_node>(key, value);
        if (key < n->key) n->left = insert(std::move(n->left), key, value);
        else if (key > n->key) n->right = insert(std::move(n->right), key, value);
        else n->value = value;  // update
        return rebalance(std::move(n));
    }

    node_ptr erase(node_ptr n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = erase(std::move(n->left), key);
        } else if (key > n->key) {
            n->right = erase(std::move(n->right), key);
        } else {
            if (!n->left) return std::move(n->right);
            if (!n->right) return std::move(n->left);
            // Two children: find inorder successor
            auto successor = find_min(std::move(n->right));
            successor->right = delete_min(std::move(n->right));
            successor->left = std::move(n->left);
            return rebalance(std::move(successor));
        }
        return rebalance(std::move(n));
    }

    static avl_node* find(avl_node* n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) return find(n->left.get(), key);
        if (key > n->key) return find(n->right.get(), key);
        return n;
    }

    node_ptr root_;
};
