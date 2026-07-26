#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace dsa {

// Left-Leaning Red-Black Tree (Sedgewick 2008).
// Simpler than standard RB: only right red links allowed, at most one
// consecutive left red link.
template <typename K, typename V, typename Compare = std::less<K>>
class llrb_tree {
public:
    llrb_tree() = default;
    ~llrb_tree() { destroy(root_); }

    llrb_tree(const llrb_tree&) = delete;
    llrb_tree& operator=(const llrb_tree&) = delete;

    void insert(const K& key, const V& val) { insert(root_, key, val); }

    V* find(const K& key) { return find_rec(root_, key); }

    bool erase(const K& key) {
        bool removed = false;
        erase_rec(root_, key, removed);
        if (root_) root_->red = false;
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
    struct node {
        K key;
        V val;
        bool red = true;  // new nodes are red
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

    static bool is_red(node* n) { return n && n->red; }

    static node* rotate_left(node* h) {
        auto x = h->right;
        h->right = x->left;
        x->left = h;
        x->red = h->red;
        h->red = true;
        return x;
    }

    static node* rotate_right(node* h) {
        auto x = h->left;
        h->left = x->right;
        x->right = h;
        x->red = h->red;
        h->red = true;
        return x;
    }

    static void flip_colors(node* h) {
        h->red = !h->red;
        h->left->red = !h->left->red;
        h->right->red = !h->right->red;
    }

    static node* fix_up(node* h) {
        if (is_red(h->right) && !is_red(h->left)) h = rotate_left(h);
        if (is_red(h->left) && is_red(h->left->left)) h = rotate_right(h);
        if (is_red(h->left) && is_red(h->right)) flip_colors(h);
        return h;
    }

    static node* move_red_left(node* h) {
        flip_colors(h);
        if (is_red(h->right->left)) {
            h->right = rotate_right(h->right);
            h = rotate_left(h);
            flip_colors(h);
        }
        return h;
    }

    static node* move_red_right(node* h) {
        flip_colors(h);
        if (is_red(h->left->left)) {
            h = rotate_right(h);
            flip_colors(h);
        }
        return h;
    }

    static node* delete_min(node* h) {
        if (!h->left) return nullptr;
        if (!is_red(h->left) && !is_red(h->left->left))
            h = move_red_left(h);
        h->left = delete_min(h->left);
        return fix_up(h);
    }

    void insert(node*& h, const K& key, const V& val) {
        if (!h) { h = new node(key, val); ++count_; return; }
        if (comp_(key, h->key)) insert(h->left, key, val);
        else if (comp_(h->key, key)) insert(h->right, key, val);
        else { h->val = val; return; }
        h = fix_up(h);
    }

    V* find_rec(node* h, const K& key) {
        if (!h) return nullptr;
        if (comp_(key, h->key)) return find_rec(h->left, key);
        if (comp_(h->key, key)) return find_rec(h->right, key);
        return &h->val;
    }

    void erase_rec(node*& h, const K& key, bool& removed) {
        if (!h) return;
        if (comp_(key, h->key)) {
            if (!is_red(h->left) && !is_red(h->left ? h->left->left : nullptr))
                h = move_red_left(h);
            erase_rec(h->left, key, removed);
        } else {
            if (is_red(h->left)) h = rotate_right(h);
            if (h->key == key && !h->right) {
                delete h; h = nullptr; --count_; removed = true;
                return;
            }
            if (!is_red(h->right) && !is_red(h->right ? h->right->left : nullptr))
                h = move_red_right(h);
            if (h->key == key) {
                node* min_node = h->right;
                while (min_node->left) min_node = min_node->left;
                h->key = min_node->key;
                h->val = min_node->val;
                erase_rec(h->right, min_node->key, removed);
            } else {
                erase_rec(h->right, key, removed);
            }
        }
        if (h) h = fix_up(h);
    }

    void in_order(node* h, std::vector<std::pair<K, V>>& result) const {
        if (!h) return;
        in_order(h->left, result);
        result.push_back({h->key, h->val});
        in_order(h->right, result);
    }

    Compare comp_{};
};

}  // namespace dsa
