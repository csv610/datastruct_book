#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <vector>

namespace dsa {

// Scapegoat tree: self-balancing BST with O(log n) amortized operations.
// Rebuilds subtree when balance factor exceeds threshold alpha.
template <typename K, typename V, typename Compare = std::less<K>>
class scapegoat_tree {
public:
    scapegoat_tree() = default;
    ~scapegoat_tree() { destroy(root_); }

    scapegoat_tree(const scapegoat_tree&) = delete;
    scapegoat_tree& operator=(const scapegoat_tree&) = delete;

    void insert(const K& key, const V& val) { insert(root_, key, val); }

    V* find(const K& key) { return find_rec(root_, key); }

    bool erase(const K& key) {
        bool removed = false;
        erase_rec(root_, key, removed);
        return removed;
    }

    bool empty() const { return !root_; }
    std::size_t size() const { return count_; }

    std::vector<std::pair<K, V>> to_sorted_vector() const {
        std::vector<std::pair<K, V>> result;
        in_order(root_, result);
        return result;
    }

private:
    static constexpr double ALPHA = 0.75;

    struct node {
        K key;
        V val;
        int size = 1;
        node* left = nullptr;
        node* right = nullptr;
        node(const K& k, const V& v) : key(k), val(v) {}
    };

    node* root_ = nullptr;
    std::size_t count_ = 0;

    static void destroy(node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    static int sz(node* n) { return n ? n->size : 0; }

    static void update(node* n) {
        if (n) n->size = 1 + sz(n->left) + sz(n->right);
    }

    static bool is_unbalanced(node* n) {
        if (!n) return false;
        return sz(n->left) > ALPHA * (sz(n) + 1) ||
               sz(n->right) > ALPHA * (sz(n) + 1);
    }

    void insert(node*& n, const K& key, const V& val) {
        if (!n) { n = new node(key, val); ++count_; return; }
        if (comp_(key, n->key)) insert(n->left, key, val);
        else if (comp_(n->key, key)) insert(n->right, key, val);
        else { n->val = val; return; }
        update(n);
        if (is_unbalanced(n)) n = rebuild_subtree(n);
    }

    V* find_rec(node* n, const K& key) {
        if (!n) return nullptr;
        if (comp_(key, n->key)) return find_rec(n->left, key);
        if (comp_(n->key, key)) return find_rec(n->right, key);
        return &n->val;
    }

    void erase_rec(node*& n, const K& key, bool& removed) {
        if (!n) return;
        if (comp_(key, n->key)) { erase_rec(n->left, key, removed); update(n); return; }
        if (comp_(n->key, key)) { erase_rec(n->right, key, removed); update(n); return; }
        removed = true;
        --count_;
        if (!n->left || !n->right) {
            node* child = n->left ? n->left : n->right;
            delete n;
            n = child;
            return;
        }
        node* pred = n->left;
        while (pred->right) pred = pred->right;
        n->key = pred->key;
        n->val = pred->val;
        erase_rec(n->left, pred->key, removed);
        update(n);
    }

    // Flatten subtree into sorted array
    void flatten(node* n, std::vector<node*>& out) {
        if (!n) return;
        flatten(n->left, out);
        out.push_back(n);
        flatten(n->right, out);
        n->left = n->right = nullptr;
        n->size = 1;
    }

    // Build balanced BST from sorted array
    node* build_balanced(std::vector<node*>& nodes, int lo, int hi) {
        if (lo > hi) return nullptr;
        int mid = lo + (hi - lo) / 2;
        node* n = nodes[mid];
        n->left = build_balanced(nodes, lo, mid - 1);
        n->right = build_balanced(nodes, mid + 1, hi);
        update(n);
        return n;
    }

    node* rebuild_subtree(node* n) {
        std::vector<node*> nodes;
        flatten(n, nodes);
        return build_balanced(nodes, 0, static_cast<int>(nodes.size()) - 1);
    }

    void in_order(node* n, std::vector<std::pair<K, V>>& result) const {
        if (!n) return;
        in_order(n->left, result);
        result.push_back({n->key, n->val});
        in_order(n->right, result);
    }

    Compare comp_{};
};

}  // namespace dsa
