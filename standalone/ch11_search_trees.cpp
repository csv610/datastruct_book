#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

// -----------------------------------------------------------------------
// Red-Black Tree
// -----------------------------------------------------------------------

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

    static bool is_red(const rb_node* n) {
        return n && n->col == RED;
    }

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

    node_ptr fixup(node_ptr n) {
        if (!n) return nullptr;

        if (is_red(n->left.get()) && (is_red(n->left->left.get()) || is_red(n->left->right.get()))) {
            if (is_red(n->right.get())) {
                n->left->col = BLACK;
                n->right->col = BLACK;
                n->col = RED;
            } else {
                if (is_red(n->left->right.get())) {
                    n->left = rotate_left(std::move(n->left));
                }
                n = rotate_right(std::move(n));
                n->col = BLACK;
                n->right->col = RED;
            }
            return n;
        }

        if (is_red(n->right.get()) && (is_red(n->right->left.get()) || is_red(n->right->right.get()))) {
            if (is_red(n->left.get())) {
                n->left->col = BLACK;
                n->right->col = BLACK;
                n->col = RED;
            } else {
                if (is_red(n->right->left.get())) {
                    n->right = rotate_right(std::move(n->right));
                }
                n = rotate_left(std::move(n));
                n->col = BLACK;
                n->left->col = RED;
            }
            return n;
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

    void inorder(std::ostream& os, rb_node* n) const {
        if (!n) return;
        inorder(os, n->left.get());
        os << n->key << "(" << (n->col == RED ? "R" : "B") << ") ";
        inorder(os, n->right.get());
    }

public:
    rb_tree() = default;

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

    void inorder(std::ostream& os) const {
        inorder(os, root_.get());
    }
};

// -----------------------------------------------------------------------
// Demonstration
// -----------------------------------------------------------------------

int main() {
    std::cout << "=== Red-Black Tree ===\n\n";

    rb_tree<int, std::string> rbt;

    // Insert keys that exercise rotations and recolorings
    rbt.insert(10, "ten");
    rbt.insert(20, "twenty");
    rbt.insert(30, "thirty");   // RR rotation
    rbt.insert(15, "fifteen");
    rbt.insert(25, "twenty-five");
    rbt.insert(5,  "five");
    rbt.insert(1,  "one");      // LL rotation
    rbt.insert(3,  "three");    // LR rotation
    rbt.insert(7,  "seven");
    rbt.insert(35, "thirty-five");

    std::cout << "Inorder traversal (key, color): ";
    rbt.inorder(std::cout);
    std::cout << "\n\n";

    // Lookups
    std::cout << "Lookups:\n";
    for (int k : {10, 15, 99}) {
        auto v = rbt.find(k);
        if (v)
            std::cout << "  " << k << " -> " << *v << "\n";
        else
            std::cout << "  " << k << " not found\n";
    }

    std::cout << "\nInsert 15 again (update):\n";
    rbt.insert(15, "fifteen-updated");
    auto v = rbt.find(15);
    if (v) std::cout << "  15 -> " << *v << "\n";

    return 0;
}
