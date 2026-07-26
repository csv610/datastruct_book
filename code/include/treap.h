#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <random>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class treap {
public:
    using key_type = Key;
    using size_type = std::size_t;

    treap() = default;

    explicit treap(Compare cmp) : comp_(std::move(cmp)) {}

    treap(std::initializer_list<Key> init, Compare cmp = {})
        : comp_(std::move(cmp)) {
        for (const auto& k : init) insert(k);
    }

    ~treap() { destroy(root_); }

    treap(const treap& other) : root_(clone(other.root_)), size_(other.size_), comp_(other.comp_) {}

    treap& operator=(const treap& other) {
        if (this != &other) {
            destroy(root_);
            root_ = clone(other.root_);
            size_ = other.size_;
            comp_ = other.comp_;
        }
        return *this;
    }

    treap(treap&& other) noexcept
        : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)),
          rng_(std::move(other.rng_)) {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    treap& operator=(treap&& other) noexcept {
        if (this != &other) {
            destroy(root_);
            root_ = other.root_;
            size_ = other.size_;
            comp_ = std::move(other.comp_);
            rng_ = std::move(other.rng_);
            other.root_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    bool insert(const Key& key) {
        return insert_rec(root_, key);
    }

    template <typename... Args>
    bool emplace(Args&&... args) {
        Key key(std::forward<Args>(args)...);
        return insert_rec(root_, std::move(key));
    }

    bool erase(const Key& key) {
        return erase_rec(root_, key);
    }

    const Key* find(const Key& key) const {
        return search(root_, key);
    }

    bool contains(const Key& key) const {
        return search(root_, key) != nullptr;
    }

    bool empty() const noexcept { return !root_; }
    size_type size() const noexcept { return size_; }

    Compare key_comp() const { return comp_; }

    std::vector<Key> inorder() const {
        std::vector<Key> result;
        result.reserve(size_);
        inorder_rec(root_, result);
        return result;
    }

    template <typename Visitor>
    void for_each(Visitor&& visit) const {
        inorder_visitor(root_, std::forward<Visitor>(visit));
    }

private:
    struct node {
        Key key;
        int priority;
        node* left = nullptr;
        node* right = nullptr;
        explicit node(const Key& k, int p) : key(k), priority(p) {}
        node(Key&& k, int p) : key(std::move(k)), priority(p) {}
    };

    int rand_priority() {
        thread_local std::mt19937 rng(std::random_device{}());
        thread_local std::uniform_int_distribution<int> dist(0, 1000000);
        return dist(rng);
    }

    bool insert_rec(node*& n, Key key) {
        if (!n) { n = new node(std::move(key), rand_priority()); ++size_; return true; }
        if (comp_(key, n->key)) {
            if (insert_rec(n->left, std::move(key))) {
                if (n->left->priority > n->priority)
                    rotate_right(n);
                return true;
            }
        } else if (comp_(n->key, key)) {
            if (insert_rec(n->right, std::move(key))) {
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

    const Key* search(node* n, const Key& key) const {
        if (!n) return nullptr;
        if (comp_(key, n->key)) return search(n->left, key);
        if (comp_(n->key, key)) return search(n->right, key);
        return &n->key;
    }

    void inorder_rec(node* n, std::vector<Key>& out) const {
        if (!n) return;
        inorder_rec(n->left, out);
        out.push_back(n->key);
        inorder_rec(n->right, out);
    }

    template <typename Visitor>
    void inorder_visitor(node* n, Visitor&& visit) const {
        if (!n) return;
        inorder_visitor(n->left, visit);
        visit(n->key);
        inorder_visitor(n->right, visit);
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

    node* clone(node* n) const {
        if (!n) return nullptr;
        auto* c = new node(n->key, n->priority);
        c->left = clone(n->left);
        c->right = clone(n->right);
        return c;
    }

    void destroy(node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    node* root_ = nullptr;
    size_type size_ = 0;
    Compare comp_ = Compare{};
    std::mt19937 rng_{std::random_device{}()};
};

}  // namespace dsa
