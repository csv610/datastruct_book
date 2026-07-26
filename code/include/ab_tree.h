#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace dsa {

// (a,b)-tree: generalization of B-tree.
// Every internal node has between a and b children (a >= 2).
// All leaves at the same depth. Keys in a node are sorted.
template <typename K, typename V, int A = 2, int B = 4>
class ab_tree {
    static_assert(A >= 2 && B > 2 * A - 1);

public:
    ab_tree() = default;

    void insert(const K& key, const V& val) {
        if (!root_) {
            root_ = std::make_unique<node>();
            root_->keys[0] = key;
            root_->vals[0] = val;
            root_->n = 1;
            ++count_;
            return;
        }
        if (root_->n == B - 1) {
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
        K keys[B - 1];
        V vals[B - 1];
        std::unique_ptr<node> children[B];
        node() = default;
    };

    std::unique_ptr<node> root_;
    std::size_t count_ = 0;

    void split_child(node* parent, int i) {
        node* full = parent->children[i].get();
        int mid = (B - 1) / 2;
        auto right = std::make_unique<node>();
        right->is_leaf = full->is_leaf;
        right->n = full->n - mid - 1;
        for (int j = 0; j < right->n; ++j) {
            right->keys[j] = full->keys[mid + 1 + j];
            right->vals[j] = full->vals[mid + 1 + j];
        }
        if (!full->is_leaf) {
            for (int j = 0; j <= right->n; ++j)
                right->children[j] = std::move(full->children[mid + 1 + j]);
        }
        for (int j = parent->n; j > i; --j) {
            parent->children[j + 1] = std::move(parent->children[j]);
            parent->keys[j] = parent->keys[j - 1];
            parent->vals[j] = parent->vals[j - 1];
        }
        parent->children[i + 1] = std::move(right);
        parent->keys[i] = full->keys[mid];
        parent->vals[i] = full->vals[mid];
        full->n = mid;
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
        if (n->children[i]->n == B - 1) {
            split_child(n, i);
            if (key > n->keys[i]) ++i;
        }
        insert_nonfull(n->children[i].get(), key, val);
    }

    V* find_rec(node* n, const K& key) {
        if (!n) return nullptr;
        for (int i = 0; i < n->n; ++i) {
            if (n->keys[i] == key) return &n->vals[i];
            if (key < n->keys[i])
                return n->is_leaf ? nullptr : find_rec(n->children[i].get(), key);
        }
        return n->is_leaf ? nullptr : find_rec(n->children[n->n].get(), key);
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
