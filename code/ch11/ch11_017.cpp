template <typename K, typename V>
class splay_tree {
    struct node {
        K key;
        V value;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;

        node(const K& k, const V& v) : key(k), value(v) {}
    };

    using node_ptr = std::unique_ptr<node>;
    node_ptr root_;

    // Right rotation (zig)
    node_ptr rotate_right(node_ptr p) {
        auto x = std::move(p->left);
        p->left = std::move(x->right);
        x->right = std::move(p);
        return x;
    }

    // Left rotation (zig)
    node_ptr rotate_left(node_ptr p) {
        auto x = std::move(p->right);
        p->right = std::move(x->left);
        x->left = std::move(p);
        return x;
    }

    // Splay node with given key to the root.
    // Returns the new root of the splayed subtree.
    node_ptr splay(node_ptr n, const K& key) {
        if (!n || n->key == key) return n;

        // Dummy root to simplify zig at the real root
        node dummy(K{}, V{});
        node_ptr* parent_link = &dummy.right;

        while (n && n->key != key) {
            if (key < n->key) {
                if (!n->left) break;
                if (key < n->left->key) {
                    // Zig-zig: left-left
                    n = rotate_right(std::move(n));
                    if (!n->left) break;
                }
                // Link n's left child up, move down
                *parent_link = std::move(n);
                parent_link = &(*parent_link)->left;
                n = std::move(*parent_link);
            } else {
                if (!n->right) break;
                if (key > n->right->key) {
                    // Zig-zig: right-right
                    n = rotate_left(std::move(n));
                    if (!n->right) break;
                }
                // Link n's right child up, move down
                *parent_link = std::move(n);
                parent_link = &(*parent_link)->right;
                n = std::move(*parent_link);
            }
        }

        // Final rotation to bring accessed node to root
        if (n) {
            if (key < n->key) {
                auto right = std::move(n->right);
                n->right = nullptr;
                auto rotated = rotate_right(std::move(n));
                rotated->right = std::move(right);
                *parent_link = std::move(rotated);
            } else {
                auto left = std::move(n->left);
                n->left = nullptr;
                auto rotated = rotate_left(std::move(n));
                rotated->left = std::move(left);
                *parent_link = std::move(rotated);
            }
        }

        return std::move(dummy.right);
    }

public:
    void insert(const K& key, const V& value) {
        if (!root_) {
            root_ = std::make_unique<node>(key, value);
            return;
        }
        root_ = splay(std::move(root_), key);
        if (root_->key == key) {
            root_->value = value;
            return;
        }
        auto new_node = std::make_unique<node>(key, value);
        if (key < root_->key) {
            new_node->right = std::move(root_);
            new_node->left = std::move(new_node->right->left);
            new_node->right->left = nullptr;
        } else {
            new_node->left = std::move(root_);
            new_node->right = std::move(new_node->left->right);
            new_node->left->right = nullptr;
        }
        root_ = std::move(new_node);
    }

    std::optional<V> find(const K& key) {
        if (!root_) return std::nullopt;
        root_ = splay(std::move(root_), key);
        if (root_->key == key) return root_->value;
        return std::nullopt;
    }

    void erase(const K& key) {
        if (!root_ || root_->key != key) return;
        if (!root_->left) {
            root_ = std::move(root_->right);
            return;
        }
        auto right = std::move(root_->right);
        root_ = std::move(root_->left);
        root_ = splay(std::move(root_), key);
        root_->right = std::move(right);
    }
};
