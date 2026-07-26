#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace dsa {

// B+ tree: all data in leaves, internal nodes guide search.
// Leaf nodes are linked for efficient range queries.
template <typename K, typename V, int ORDER = 4>
class bplus_tree {
    static_assert(ORDER >= 3);

public:
    bplus_tree() = default;

    void insert(const K& key, const V& val) {
        K push_key{};
        auto result = insert_rec(root_, key, val, push_key);
        if (result) {
            auto parent = std::make_unique<node>(false);
            parent->keys[0] = push_key;
            parent->children[0] = std::move(root_);
            parent->children[1] = std::move(result);
            parent->n = 1;
            root_ = std::move(parent);
        }
        ++count_;
    }

    V* find(const K& key) {
        if (!root_) return nullptr;
        return find_rec(root_.get(), key);
    }

    const V* find(const K& key) const {
        if (!root_) return nullptr;
        return find_rec(root_.get(), key);
    }

    template <typename... Args>
    void emplace(const K& key, Args&&... args) {
        insert(key, V(std::forward<Args>(args)...));
    }

    bool erase(const K& key) {
        if (!root_) return false;
        bool removed = false;
        erase_rec(root_, key, removed);
        if (root_ && root_->n == 0 && !root_->is_leaf) {
            root_ = std::move(root_->children[0]);
        }
        if (removed) --count_;
        return removed;
    }

    bool empty() const noexcept { return !root_; }
    std::size_t size() const noexcept { return count_; }

    std::vector<std::pair<K, V>> range(const K& lo, const K& hi) const {
        std::vector<std::pair<K, V>> result;
        if (!root_) return result;
        range_rec(root_.get(), lo, hi, result);
        return result;
    }

    std::vector<std::pair<K, V>> to_sorted_vector() const {
        std::vector<std::pair<K, V>> result;
        if (!root_) return result;
        in_order(root_.get(), result);
        return result;
    }

private:
    static constexpr int MAX_KEYS = ORDER - 1;
    static constexpr int MAX_CHILDREN = ORDER;

    struct node {
        int n = 0;
        bool is_leaf = false;
        std::vector<K> keys;
        std::vector<V> vals;
        std::vector<std::unique_ptr<node>> children;
        node* next = nullptr;

        node(bool leaf) : is_leaf(leaf) {
            keys.resize(MAX_KEYS);
            if (leaf) vals.resize(MAX_KEYS);
            children.resize(MAX_CHILDREN);
        }
    };

    std::unique_ptr<node> root_;
    std::size_t count_ = 0;

    V* find_rec(node* n, const K& key) {
        if (n->is_leaf) {
            for (int i = 0; i < n->n; ++i)
                if (n->keys[i] == key) return &n->vals[i];
            return nullptr;
        }
        int i = 0;
        while (i < n->n && key >= n->keys[i]) ++i;
        return find_rec(n->children[i].get(), key);
    }

    const V* find_rec(node* n, const K& key) const {
        if (n->is_leaf) {
            for (int i = 0; i < n->n; ++i)
                if (n->keys[i] == key) return &n->vals[i];
            return nullptr;
        }
        int i = 0;
        while (i < n->n && key >= n->keys[i]) ++i;
        return find_rec(n->children[i].get(), key);
    }

    // Returns new right node if split occurred, nullptr otherwise.
    // push_key is set to the key to push up to the parent on split.
    std::unique_ptr<node> insert_rec(std::unique_ptr<node>& n, const K& key,
                                      const V& val, K& push_key) {
        if (!n) {
            auto leaf = std::make_unique<node>(true);
            leaf->keys[0] = key;
            leaf->vals[0] = val;
            leaf->n = 1;
            return leaf;
        }
        if (n->is_leaf) {
            if (n->n < MAX_KEYS) {
                int i = n->n - 1;
                while (i >= 0 && n->keys[i] > key) {
                    n->keys[i + 1] = n->keys[i];
                    n->vals[i + 1] = n->vals[i];
                    --i;
                }
                n->keys[i + 1] = key;
                n->vals[i + 1] = val;
                ++n->n;
                return nullptr;
            }
            auto right = std::make_unique<node>(true);
            K tmp_keys[MAX_KEYS + 1];
            V tmp_vals[MAX_KEYS + 1];
            int j = 0, inserted = 0;
            for (int k = 0; k < n->n; ++k) {
                if (!inserted && key < n->keys[k]) {
                    tmp_keys[j] = key;
                    tmp_vals[j] = val;
                    ++j;
                    inserted = 1;
                }
                tmp_keys[j] = n->keys[k];
                tmp_vals[j] = n->vals[k];
                ++j;
            }
            if (!inserted) {
                tmp_keys[j] = key;
                tmp_vals[j] = val;
                ++j;
            }
            int total = j;
            int mid = total / 2;
            n->n = mid;
            for (int k = 0; k < mid; ++k) {
                n->keys[k] = tmp_keys[k];
                n->vals[k] = tmp_vals[k];
            }
            right->n = total - mid;
            for (int k = 0; k < right->n; ++k) {
                right->keys[k] = tmp_keys[mid + k];
                right->vals[k] = tmp_vals[mid + k];
            }
            right->next = n->next;
            n->next = right.get();
            push_key = right->keys[0];
            return right;
        }
        // Internal node
        int i = 0;
        while (i < n->n && key >= n->keys[i]) ++i;
        K child_push{};
        auto result = insert_rec(n->children[i], key, val, child_push);
        if (!result) return nullptr;

        if (n->n < MAX_KEYS) {
            for (int j = n->n - 1; j >= i; --j) {
                n->keys[j + 1] = n->keys[j];
                n->children[j + 2] = std::move(n->children[j + 1]);
            }
            n->keys[i] = child_push;
            n->children[i + 1] = std::move(result);
            ++n->n;
            return nullptr;
        }

        // Full internal node — split
        K tmp_keys[MAX_KEYS + 1];
        std::unique_ptr<node> tmp_children[MAX_CHILDREN + 1];
        for (int j = 0; j < MAX_KEYS; ++j) tmp_keys[j] = n->keys[j];
        for (int j = 0; j < MAX_CHILDREN; ++j) tmp_children[j] = std::move(n->children[j]);

        for (int j = MAX_KEYS - 1; j >= i; --j) tmp_keys[j + 1] = tmp_keys[j];
        tmp_keys[i] = child_push;
        for (int j = MAX_CHILDREN - 1; j >= i + 1; --j)
            tmp_children[j + 1] = std::move(tmp_children[j]);
        tmp_children[i + 1] = std::move(result);

        int mid = MAX_KEYS / 2;
        push_key = tmp_keys[mid];

        n->n = mid;
        for (int j = 0; j < mid; ++j) n->keys[j] = tmp_keys[j];
        for (int j = 0; j <= mid; ++j) n->children[j] = std::move(tmp_children[j]);

        auto right = std::make_unique<node>(false);
        right->n = MAX_KEYS - mid;
        for (int j = 0; j < right->n; ++j) right->keys[j] = tmp_keys[mid + 1 + j];
        for (int j = 0; j <= right->n; ++j)
            right->children[j] = std::move(tmp_children[mid + 1 + j]);

        return right;
    }

    void erase_rec(std::unique_ptr<node>& n, const K& key, bool& removed) {
        if (!n) return;
        if (n->is_leaf) {
            for (int i = 0; i < n->n; ++i) {
                if (n->keys[i] == key) {
                    for (int j = i; j < n->n - 1; ++j) {
                        n->keys[j] = n->keys[j + 1];
                        n->vals[j] = n->vals[j + 1];
                    }
                    --n->n;
                    removed = true;
                    return;
                }
            }
            return;
        }
        int i = 0;
        while (i < n->n && key >= n->keys[i]) ++i;
        erase_rec(n->children[i], key, removed);
    }

    void range_rec(node* n, const K& lo, const K& hi,
                   std::vector<std::pair<K, V>>& result) const {
        if (!n) return;
        if (n->is_leaf) {
            for (int i = 0; i < n->n; ++i) {
                if (n->keys[i] >= lo && n->keys[i] <= hi)
                    result.push_back({n->keys[i], n->vals[i]});
            }
            return;
        }
        for (int i = 0; i <= n->n; ++i)
            range_rec(n->children[i].get(), lo, hi, result);
    }

    void in_order(node* n, std::vector<std::pair<K, V>>& result) const {
        if (!n) return;
        if (n->is_leaf) {
            for (int i = 0; i < n->n; ++i)
                result.push_back({n->keys[i], n->vals[i]});
            return;
        }
        for (int i = 0; i <= n->n; ++i)
            in_order(n->children[i].get(), result);
    }
};

}  // namespace dsa
