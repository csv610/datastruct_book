#pragma once
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class b_tree {
public:
    explicit b_tree(int order = 5) : order_(order), root_(new node(true)) {}

    void insert(const Key& key) {
        node* r = root_;
        if (r->keys.size() == static_cast<std::size_t>(order_) - 1) {
            auto* s = new node(false);
            s->children.push_back(r);
            split_child(s, 0);
            root_ = s;
        }
        insert_non_full(root_, key);
    }

    bool contains(const Key& key) const { return search(root_, key); }

    bool erase(const Key& key) {
        if (!contains(key)) return false;
        erase_rec(root_, key);
        if (root_->keys.empty() && !root_->is_leaf) {
            node* old = root_;
            root_ = root_->children[0];
            delete old;
        }
        return true;
    }

    std::vector<Key> inorder() const {
        std::vector<Key> result;
        inorder_rec(root_, result);
        return result;
    }

    ~b_tree() { destroy(root_); }
    b_tree(const b_tree&) = delete;
    b_tree& operator=(const b_tree&) = delete;

private:
    struct node {
        bool is_leaf;
        std::vector<Key> keys;
        std::vector<node*> children;
        explicit node(bool leaf) : is_leaf(leaf) {}
    };

    void split_child(node* parent, int i) {
        node* full = parent->children[i];
        int mid = static_cast<int>(full->keys.size()) / 2;
        auto* z = new node(full->is_leaf);

        z->keys.assign(full->keys.begin() + mid + 1, full->keys.end());
        full->keys.resize(mid);

        if (!full->is_leaf) {
            z->children.assign(full->children.begin() + mid + 1, full->children.end());
            full->children.resize(mid + 1);
        }

        parent->children.insert(parent->children.begin() + i + 1, z);
        parent->keys.insert(parent->keys.begin() + i, full->keys[mid]);
    }

    void insert_non_full(node* n, const Key& key) {
        int i = static_cast<int>(n->keys.size()) - 1;
        if (n->is_leaf) {
            n->keys.push_back(key);
            while (i >= 0 && comp_(key, n->keys[i])) {
                n->keys[i + 1] = n->keys[i];
                --i;
            }
            n->keys[i + 1] = key;
        } else {
            while (i >= 0 && comp_(key, n->keys[i])) --i;
            ++i;
            if (n->children[i]->keys.size() == static_cast<std::size_t>(order_) - 1) {
                split_child(n, i);
                if (!comp_(n->keys[i], key) && !comp_(key, n->keys[i])) return;
                i += (key > n->keys[i]) ? 1 : 0;
            }
            insert_non_full(n->children[i], key);
        }
    }

    bool search(node* n, const Key& key) const {
        if (!n) return false;
        std::size_t i = 0;
        while (i < n->keys.size() && comp_(n->keys[i], key)) ++i;
        if (i < n->keys.size() && !comp_(key, n->keys[i]) && !comp_(n->keys[i], key))
            return true;
        return n->is_leaf ? false : search(n->children[i], key);
    }

    void inorder_rec(node* n, std::vector<Key>& out) const {
        if (!n) return;
        for (std::size_t i = 0; i < n->keys.size(); ++i) {
            if (!n->is_leaf) inorder_rec(n->children[i], out);
            out.push_back(n->keys[i]);
        }
        if (!n->is_leaf) inorder_rec(n->children.back(), out);
    }

    void erase_rec(node* n, const Key& key) {
        std::size_t i = 0;
        while (i < n->keys.size() && comp_(n->keys[i], key)) ++i;

        if (i < n->keys.size() && !comp_(key, n->keys[i]) && !comp_(n->keys[i], key)) {
            if (n->is_leaf) {
                n->keys.erase(n->keys.begin() + i);
                return;
            }
            erase_internal(n, i);
        } else {
            if (n->is_leaf) return;
            bool last = (i == n->keys.size());
            if (n->children[i]->keys.size() < static_cast<std::size_t>(order_) / 2 + 1)
                fill(n, i);
            if (last && i > n->keys.size())
                erase_rec(n->children[i - 1], key);
            else
                erase_rec(n->children[i], key);
        }
    }

    void fill(node* n, int i) {
        if (i > 0 && n->children[i - 1]->keys.size() > static_cast<std::size_t>(order_) / 2)
            borrow_from_prev(n, i);
        else if (i < static_cast<int>(n->children.size()) - 1 &&
                 n->children[i + 1]->keys.size() > static_cast<std::size_t>(order_) / 2)
            borrow_from_next(n, i);
        else {
            if (i < static_cast<int>(n->keys.size()))
                merge(n, i);
            else
                merge(n, i - 1);
        }
    }

    void borrow_from_prev(node* n, int i) {
        node* child = n->children[i];
        node* sibling = n->children[i - 1];
        child->keys.insert(child->keys.begin(), n->keys[i - 1]);
        n->keys[i - 1] = sibling->keys.back();
        sibling->keys.pop_back();
        if (!child->is_leaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
    }

    void borrow_from_next(node* n, int i) {
        node* child = n->children[i];
        node* sibling = n->children[i + 1];
        child->keys.push_back(n->keys[i]);
        n->keys[i] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
        if (!child->is_leaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
    }

    void merge(node* n, int i) {
        node* left = n->children[i];
        node* right = n->children[i + 1];
        left->keys.push_back(n->keys[i]);
        left->keys.insert(left->keys.end(), right->keys.begin(), right->keys.end());
        if (!left->is_leaf)
            left->children.insert(left->children.end(),
                                  right->children.begin(), right->children.end());
        n->keys.erase(n->keys.begin() + i);
        n->children.erase(n->children.begin() + i + 1);
        delete right;
    }

    void erase_internal(node* n, int i) {
        Key pred = predecessor(n, i);
        n->keys[i] = pred;
        erase_rec(n->children[i], pred);
    }

    Key predecessor(node* n, int i) {
        node* cur = n->children[i];
        while (!cur->is_leaf) cur = cur->children.back();
        return cur->keys.back();
    }

    void destroy(node* n) {
        if (!n) return;
        for (auto* c : n->children) destroy(c);
        delete n;
    }

    int order_;
    node* root_;
    static inline Compare comp_{};
};

}  // namespace dsa
