#pragma once
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <vector>

namespace dsa {

// AA tree: simplified red-black tree where all red links are right links.
// Based on Arne Andersson's 1993 paper.
template <typename K, typename V, typename Compare = std::less<K>>
class aa_tree {
public:
    aa_tree() = default;
    ~aa_tree() { destroy(root_); }

    aa_tree(const aa_tree&) = delete;
    aa_tree& operator=(const aa_tree&) = delete;

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
    struct node {
        K key;
        V val;
        int level = 1;  // 1 = leaf
        node* left = nullptr;
        node* right = nullptr;
        node* parent = nullptr;  // for parent pointer updates
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

    node* skew(node* t) {
        if (!t || !t->left || t->left->level != t->level) return t;
        auto l = t->left;
        t->left = l->right;
        l->right = t;
        return l;
    }

    node* split(node* t) {
        if (!t || !t->right || !t->right->right || t->right->right->level != t->level)
            return t;
        auto r = t->right;
        t->right = r->left;
        r->left = t;
        r->level++;
        return r;
    }

    void insert(node*& t, const K& key, const V& val) {
        if (!t) { t = new node(key, val); ++count_; return; }
        if (comp_(key, t->key)) insert(t->left, key, val);
        else if (comp_(t->key, key)) insert(t->right, key, val);
        else { t->val = val; return; }
        t = skew(t);
        t = split(t);
    }

    V* find_rec(node* t, const K& key) {
        if (!t) return nullptr;
        if (comp_(key, t->key)) return find_rec(t->left, key);
        if (comp_(t->key, key)) return find_rec(t->right, key);
        return &t->val;
    }

    void erase_rec(node*& t, const K& key, bool& removed) {
        if (!t) return;
        if (comp_(key, t->key)) {
            erase_rec(t->left, key, removed);
            fixup(t);
            return;
        }
        if (comp_(t->key, key)) {
            erase_rec(t->right, key, removed);
            fixup(t);
            return;
        }
        removed = true;
        --count_;
        if (!t->left) {
            node* r = t->right;
            delete t;
            t = r;
            return;
        }
        if (!t->right) {
            node* l = t->left;
            delete t;
            t = l;
            return;
        }
        // Two children: replace with predecessor (max of left subtree)
        node* pred = t->left;
        while (pred->right) pred = pred->right;
        t->key = pred->key;
        t->val = pred->val;
        erase_rec(t->left, pred->key, removed);
        fixup(t);
    }

    void fixup(node*& t) {
        if (!t) return;
        if (t->left && t->left->level == t->level) t = skew(t);
        if (t->right && t->right->level == t->level) t = skew(t);
        if (t->right && t->right->right && t->right->right->level == t->level)
            t = split(t);
        int ml = std::min({t->left ? t->left->level : 0,
                           t->right ? t->right->level : 0}) + 1;
        if (t->level > ml) {
            t->level = ml;
            if (t->right && t->right->level > ml) t->right->level = ml;
        }
    }

    void in_order(node* t, std::vector<std::pair<K, V>>& result) const {
        if (!t) return;
        in_order(t->left, result);
        result.push_back({t->key, t->val});
        in_order(t->right, result);
    }

    Compare comp_{};
};

}  // namespace dsa
