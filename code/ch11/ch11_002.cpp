template <typename K, typename V>
class bst {
public:
    using node_ptr = std::unique_ptr<node>;

    bst() = default;

    void insert(const K& key, const V& value) {
        insert(root_, key, value);
    }

    std::optional<V> find(const K& key) const {
        auto* n = find(root_.get(), key);
        if (n) return n->value;
        return std::nullopt;
    }

    void erase(const K& key) {
        root_ = erase(std::move(root_), key);
    }

    void inorder(std::ostream& os) const {
        inorder(root_.get(), os);
    }

private:
    struct node {
        K key;
        V value;
        node_ptr left;
        node_ptr right;

        node(const K& k, const V& v) : key(k), value(v) {}
    };

    static void insert(node_ptr& n, const K& key, const V& value) {
        if (!n) {
            n = std::make_unique<node>(key, value);
            return;
        }
        if (key < n->key) insert(n->left, key, value);
        else if (key > n->key) insert(n->right, key, value);
        else n->value = value;  // update
    }

    static node* find(node* n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) return find(n->left.get(), key);
        if (key > n->key) return find(n->right.get(), key);
        return n;
    }

    static node_ptr erase(node_ptr n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = erase(std::move(n->left), key);
            return n;
        }
        if (key > n->key) {
            n->right = erase(std::move(n->right), key);
            return n;
        }

        // Found the node -- handle the three cases
        if (!n->left) return std::move(n->right);
        if (!n->right) return std::move(n->left);

        // Two children: replace with inorder successor
        auto successor = find_min(std::move(n->right));
        successor->right = delete_min(std::move(n->right));
        successor->left = std::move(n->left);
        return successor;
    }

    static node_ptr find_min(node_ptr n) {
        auto* current = n.get();
        while (current && current->left) current = current->left.get();
        auto result = std::make_unique<node>(current->key, current->value);
        return result;
    }

    static node_ptr delete_min(node_ptr n) {
        if (!n->left) return std::move(n->right);
        n->left = delete_min(std::move(n->left));
        return n;
    }

    static void inorder(node* n, std::ostream& os) {
        if (!n) return;
        inorder(n->left.get(), os);
        os << n->key << " ";
        inorder(n->right.get(), os);
    }

    node_ptr root_;
};
