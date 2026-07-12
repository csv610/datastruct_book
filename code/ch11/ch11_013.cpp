template <typename K, typename V>
class treap {
    struct treap_node {
        K key;
        V value;
        int priority;
        std::unique_ptr<treap_node> left;
        std::unique_ptr<treap_node> right;

        treap_node(const K& k, const V& v)
            : key(k), value(v), priority(std::rand()) {}
    };

    node_ptr rotate_right(node_ptr p) {
        node_ptr q = std::move(p->left);
        p->left = std::move(q->right);
        q->right = std::move(p);
        return q;
    }

    node_ptr rotate_left(node_ptr p) {
        node_ptr q = std::move(p->right);
        p->right = std::move(q->left);
        q->left = std::move(p);
        return q;
    }

    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<treap_node>(key, value);
        if (key < n->key) {
            n->left = insert(std::move(n->left), key, value);
            if (n->left->priority > n->priority)
                n = rotate_right(std::move(n));
        } else if (key > n->key) {
            n->right = insert(std::move(n->right), key, value);
            if (n->right->priority > n->priority)
                n = rotate_left(std::move(n));
        } else {
            n->value = value;
        }
        return n;
    }
};
