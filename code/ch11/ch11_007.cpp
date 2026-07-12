template <typename K, typename V, typename Compare = std::less<K>>
class rb_tree {
    enum color { RED, BLACK };

    struct rb_node {
        K key;
        V value;
        color col = RED;
        std::unique_ptr<rb_node> left;
        std::unique_ptr<rb_node> right;

        rb_node(const K& k, const V& v) : key(k), value(v) {}
    };

    using node_ptr = std::unique_ptr<rb_node>;
    Compare comp_;
    node_ptr root_;

    static bool is_red(const rb_node* n) { return n && n->col == RED; }

    node_ptr rotate_left(node_ptr x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        return y;
    }

    node_ptr rotate_right(node_ptr y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        return x;
    }

    // Fix red-red violations on the way back up.
    // n is the grandparent of the red-red pair.
    node_ptr fixup(node_ptr n) {
        if (!n) return nullptr;

        // Left subtree violation
        if (is_red(n->left) && (is_red(n->left->left) || is_red(n->left->right))) {
            if (is_red(n->right)) {
                // Case 1: uncle is red  ->  recolor
                n->left->col = BLACK;
                n->right->col = BLACK;
                n->col = RED;
            } else {
                // Cases 2/3: uncle is black  ->  rotate
                if (is_red(n->left->right)) {
                    // LR: zig-zag -- rotate left at child first
                    n->left = rotate_left(std::move(n->left));
                }
                // LL: straight -- rotate right at grandparent
                n = rotate_right(std::move(n));
                n->col = BLACK;
                n->right->col = RED;
            }
            return n;
        }

        // Right subtree violation (symmetric)
        if (is_red(n->right) && (is_red(n->right->left) || is_red(n->right->right))) {
            if (is_red(n->left)) {
                n->left->col = BLACK;
                n->right->col = BLACK;
                n->col = RED;
            } else {
                if (is_red(n->right->left)) {
                    n->right = rotate_right(std::move(n->right));
                }
                n = rotate_left(std::move(n));
                n->col = BLACK;
                n->left->col = RED;
            }
        }
        return n;
    }

    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<rb_node>(key, value);

        if (comp_(key, n->key))
            n->left = insert(std::move(n->left), key, value);
        else if (comp_(n->key, key))
            n->right = insert(std::move(n->right), key, value);
        else {
            n->value = value;
            return n;
        }
        return fixup(std::move(n));
    }

public:
    void insert(const K& key, const V& value) {
        root_ = insert(std::move(root_), key, value);
        if (root_) root_->col = BLACK;
    }

    std::optional<V> find(const K& key) const {
        auto* n = root_.get();
        while (n) {
            if (comp_(key, n->key))      n = n->left.get();
            else if (comp_(n->key, key)) n = n->right.get();
            else return n->value;
        }
        return std::nullopt;
    }
};
