#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace dsa {

// 2-3-4 tree: a B-tree of order 4 where all leaves are at the same depth.
// Each node has 1-3 keys and 2-4 children.
// Red-black tree in disguise (isomorphic).
template <typename K, typename V>
class two_three_four_tree {
public:
    two_three_four_tree() = default;

    void insert(const K& key, const V& val) {
        if (!root_) {
            root_ = std::make_unique<node>();
            root_->keys[0] = key;
            root_->vals[0] = val;
            root_->n = 1;
            ++count_;
            return;
        }
        if (root_->n == 3) {
            auto new_root = std::make_unique<node>();
            new_root->is_leaf = false;
            new_root->children[0] = std::move(root_);
            split_child(new_root.get(), 0);
            root_ = std::move(new_root);
        }
        insert_nonfull(root_.get(), key, val);
    }

    V* find(const K& key) {
        if (!root_) return nullptr;
        return find_rec(root_.get(), key);
    }

    bool erase(const K& key) {
        if (!root_) return false;
        bool removed = false;
        erase_rec(root_.get(), key, removed);
        if (root_ && root_->n == 0 && !root_->is_leaf) {
            root_ = std::move(root_->children[0]);
        }
        return removed;
    }

    bool empty() const { return !root_; }
    std::size_t size() const { return count_; }

    std::vector<std::pair<K, V>> to_sorted_vector() const {
        std::vector<std::pair<K, V>> result;
        in_order(root_.get(), result);
        return result;
    }

private:
    struct node {
        int n = 0;
        bool is_leaf = true;
        K keys[3];
        V vals[3];
        std::unique_ptr<node> children[4];

        node() = default;
    };

    std::unique_ptr<node> root_;
    std::size_t count_ = 0;

    void split_child(node* parent, int i) {
        node* full = parent->children[i].get();
        auto right = std::make_unique<node>();
        right->is_leaf = full->is_leaf;
        right->n = 1;
        right->keys[0] = full->keys[2];
        right->vals[0] = full->vals[2];
        if (!full->is_leaf) {
            right->children[0] = std::move(full->children[2]);
            right->children[1] = std::move(full->children[3]);
        }
        full->n = 1;
        // Shift parent
        for (int j = parent->n; j > i; --j) {
            parent->children[j + 1] = std::move(parent->children[j]);
            parent->keys[j] = parent->keys[j - 1];
            parent->vals[j] = parent->vals[j - 1];
        }
        parent->children[i + 1] = std::move(right);
        parent->keys[i] = full->keys[1];
        parent->vals[i] = full->vals[1];
        ++parent->n;
    }

    void insert_nonfull(node* n, const K& key, const V& val) {
        int i = n->n - 1;
        if (n->is_leaf) {
            while (i >= 0 && n->keys[i] > key) {
                n->keys[i + 1] = n->keys[i];
                n->vals[i + 1] = n->vals[i];
                --i;
            }
            n->keys[i + 1] = key;
            n->vals[i + 1] = val;
            ++n->n;
            ++count_;
            return;
        }
        while (i >= 0 && n->keys[i] > key) --i;
        ++i;
        if (n->children[i]->n == 3) {
            split_child(n, i);
            if (key > n->keys[i]) ++i;
        }
        insert_nonfull(n->children[i].get(), key, val);
    }

    V* find_rec(node* n, const K& key) {
        if (!n) return nullptr;
        for (int i = 0; i < n->n; ++i) {
            if (n->keys[i] == key) return &n->vals[i];
            if (key < n->keys[i]) {
                return n->is_leaf ? nullptr : find_rec(n->children[i].get(), key);
            }
        }
        return n->is_leaf ? nullptr : find_rec(n->children[n->n].get(), key);
    }

    void erase_rec(node* n, const K& key, bool& removed) {
        if (!n) return;
        if (n->is_leaf) {
            for (int i = 0; i < n->n; ++i) {
                if (n->keys[i] == key) {
                    for (int j = i; j < n->n - 1; ++j) {
                        n->keys[j] = n->keys[j + 1];
                        n->vals[j] = n->vals[j + 1];
                    }
                    --n->n;
                    --count_;
                    removed = true;
                    return;
                }
            }
            return;
        }
        int i = 0;
        while (i < n->n && key > n->keys[i]) ++i;
        if (i < n->n && key == n->keys[i]) {
            // Replace with predecessor from leaf
            node* pred = n->children[i].get();
            while (!pred->is_leaf) pred = pred->children[pred->n].get();
            n->keys[i] = pred->keys[pred->n - 1];
            n->vals[i] = pred->vals[pred->n - 1];
            erase_rec(n->children[i].get(), pred->keys[pred->n - 1], removed);
        } else {
            erase_rec(n->children[i].get(), key, removed);
        }
    }

    void in_order(node* n, std::vector<std::pair<K, V>>& result) const {
        if (!n) return;
        for (int i = 0; i < n->n; ++i) {
            if (!n->is_leaf) in_order(n->children[i].get(), result);
            result.push_back({n->keys[i], n->vals[i]});
        }
        if (!n->is_leaf) in_order(n->children[n->n].get(), result);
    }
};

}  // namespace dsa
