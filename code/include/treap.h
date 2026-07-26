#pragma once
#include <algorithm>
#include <random>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class treap {
public:
    treap() = default;
    ~treap() { destroy(root_); }

    treap(const treap&) = delete;
    treap& operator=(const treap&) = delete;

    bool insert(const Key& key) {
        return insert_rec(root_, key);
    }

    bool erase(const Key& key) {
        return erase_rec(root_, key);
    }

    bool contains(const Key& key) const {
        return search(root_, key);
    }

    std::vector<Key> inorder() const {
        std::vector<Key> result;
        inorder_rec(root_, result);
        return result;
    }

    bool empty() const { return !root_; }
    std::size_t size() const { return size_; }

private:
    struct node {
        Key key;
        int priority;
        node* left = nullptr;
        node* right = nullptr;
        explicit node(const Key& k, int p) : key(k), priority(p) {}
    };

    int rand_priority() {
        static std::mt19937 rng(42);
        static std::uniform_int_distribution<int> dist(0, 1000000);
        return dist(rng);
    }

    bool insert_rec(node*& n, const Key& key) {
        if (!n) { n = new node(key, rand_priority()); ++size_; return true; }
        if (comp_(key, n->key)) {
            if (insert_rec(n->left, key)) {
                if (n->left->priority > n->priority)
                    rotate_right(n);
                return true;
            }
        } else if (comp_(n->key, key)) {
            if (insert_rec(n->right, key)) {
                if (n->right->priority > n->priority)
                    rotate_left(n);
                return true;
            }
        }
        return false;
    }

    bool erase_rec(node*& n, const Key& key) {
        if (!n) return false;
        if (comp_(key, n->key)) return erase_rec(n->left, key);
        if (comp_(n->key, key)) return erase_rec(n->right, key);
        // Found
        if (!n->left && !n->right) {
            delete n; n = nullptr;
            --size_;
        } else if (!n->left) {
            node* tmp = n; n = n->right; delete tmp;
            --size_;
        } else if (!n->right) {
            node* tmp = n; n = n->left; delete tmp;
            --size_;
        } else {
            if (n->left->priority > n->right->priority) {
                rotate_right(n);
                erase_rec(n->right, key);
            } else {
                rotate_left(n);
                erase_rec(n->left, key);
            }
        }
        return true;
    }

    bool search(node* n, const Key& key) const {
        if (!n) return false;
        if (comp_(key, n->key)) return search(n->left, key);
        if (comp_(n->key, key)) return search(n->right, key);
        return true;
    }

    void inorder_rec(node* n, std::vector<Key>& out) const {
        if (!n) return;
        inorder_rec(n->left, out);
        out.push_back(n->key);
        inorder_rec(n->right, out);
    }

    void rotate_right(node*& n) {
        node* l = n->left;
        n->left = l->right;
        l->right = n;
        n = l;
    }

    void rotate_left(node*& n) {
        node* r = n->right;
        n->right = r->left;
        r->left = n;
        n = r;
    }

    void destroy(node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    node* root_ = nullptr;
    std::size_t size_ = 0;
    static inline Compare comp_{};
};

}  // namespace dsa
