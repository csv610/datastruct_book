#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

template <typename K, typename V, typename Compare = std::less<K>>
    requires std::regular<K> && std::regular<V>
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

public:
    avl_tree() = default;

    void insert(const K& key, const V& value) {
        root_ = insert(std::move(root_), key, value);
    }

    void erase(const K& key) {
        root_ = erase(std::move(root_), key);
    }

    std::optional<V> find(const K& key) const {
        auto* n = find(root_.get(), key);
        if (n) return n->value;
        return std::nullopt;
    }

    bool contains(const K& key) const { return find(key).has_value(); }

    bool        empty()       const noexcept { return root_ == nullptr; }
    std::size_t size()  const { return size(root_.get()); }

private:
    int height(const avl_node* n) const { return n ? n->height : 0; }

    int balance_factor(const avl_node* n) const {
        return height(n->left.get()) - height(n->right.get());
    }

    void update_height(avl_node* n) {
        n->height = 1 + std::max(height(n->left.get()), height(n->right.get()));
    }

    // Right rotation (LL imbalance)
    node_ptr rotate_right(node_ptr z) {
        auto y = std::move(z->left);
        z->left = std::move(y->right);
        update_height(z.get());
        y->right = std::move(z);
        update_height(y.get());
        return y;
    }

    // Left rotation (RR imbalance)
    node_ptr rotate_left(node_ptr z) {
        auto y = std::move(z->right);
        z->right = std::move(y->left);
        update_height(z.get());
        y->left = std::move(z);
        update_height(y.get());
        return y;
    }

    // Rebalance a node after insertion or deletion
    node_ptr rebalance(node_ptr n) {
        if (!n) return nullptr;
        update_height(n.get());
        int bf = balance_factor(n.get());

        // Left heavy
        if (bf > 1) {
            if (balance_factor(n->left.get()) < 0)
                n->left = rotate_left(std::move(n->left));
            return rotate_right(std::move(n));
        }

        // Right heavy
        if (bf < -1) {
            if (balance_factor(n->right.get()) > 0)
                n->right = rotate_right(std::move(n->right));
            return rotate_left(std::move(n));
        }

        return n;
    }

    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<avl_node>(key, value);

        if (compare_(key, n->key))
            n->left = insert(std::move(n->left), key, value);
        else if (compare_(n->key, key))
            n->right = insert(std::move(n->right), key, value);
        else
            n->value = value;  // update existing key

        return rebalance(std::move(n));
    }

    node_ptr erase(node_ptr n, const K& key) {
        if (!n) return nullptr;

        if (compare_(key, n->key)) {
            n->left = erase(std::move(n->left), key);
        } else if (compare_(n->key, key)) {
            n->right = erase(std::move(n->right), key);
        } else {
            // Found the node to erase
            if (!n->left)  return std::move(n->right);
            if (!n->right) return std::move(n->left);

            // Two children: replace with inorder successor
            node_ptr successor = find_min(std::move(n->right));
            successor->right = delete_min(std::move(n->right));
            successor->left  = std::move(n->left);
            return rebalance(std::move(successor));
        }

        return rebalance(std::move(n));
    }

    static node_ptr find_min(node_ptr n) {
        auto* cur = n.get();
        while (cur && cur->left) cur = cur->left.get();
        auto result = std::make_unique<avl_node>(cur->key, cur->value);
        return result;
    }

    static node_ptr delete_min(node_ptr n) {
        if (!n->left) return std::move(n->right);
        n->left = delete_min(std::move(n->left));
        return n;
    }

    static avl_node* find(avl_node* n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) return find(n->left.get(), key);
        if (n->key < key) return find(n->right.get(), key);
        return n;
    }

    static std::size_t size(avl_node* n) {
        if (!n) return 0;
        return 1 + size(n->left.get()) + size(n->right.get());
    }

    node_ptr root_;
    Compare  compare_;
};

#endif
