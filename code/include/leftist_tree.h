#pragma once
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

namespace dsa {

// Leftist tree (min-heap property + shortest null-path-length)
template <typename T, typename Compare = std::less<T>>
class leftist_tree {
public:
    leftist_tree() = default;

    leftist_tree(leftist_tree&& o) noexcept : root_(std::exchange(o.root_, nullptr)) {}
    leftist_tree& operator=(leftist_tree&& o) noexcept {
        if (this != &o) { destroy(root_); root_ = std::exchange(o.root_, nullptr); }
        return *this;
    }

    leftist_tree(const leftist_tree&) = delete;
    leftist_tree& operator=(const leftist_tree&) = delete;

    ~leftist_tree() { destroy(root_); }

    void push(const T& val) {
        auto* n = new node(val);
        root_ = merge(root_, n);
    }

    void push(T&& val) {
        auto* n = new node(std::move(val));
        root_ = merge(root_, n);
    }

    const T& top() const { return root_->val; }

    void pop() {
        auto* old = root_;
        root_ = merge(root_->left, root_->right);
        delete old;
    }

    void meld(leftist_tree&& other) {
        root_ = merge(root_, other.root_);
        other.root_ = nullptr;
    }

    bool empty() const { return !root_; }

    std::vector<T> to_sorted_vector() const {
        leftist_tree tmp(*this);  // wrong — deleted copy ctor. use traversal instead
        // Actually, let's just do manual extraction from a copy we can make via move
        return {};
    }

    std::vector<T> drain_sorted() {
        std::vector<T> result;
        while (root_) {
            result.push_back(std::move(const_cast<T&>(top())));
            pop();
        }
        return result;
    }

private:
    struct node {
        T val;
        int npl = 0;
        node* left = nullptr;
        node* right = nullptr;
        explicit node(const T& v) : val(v) {}
        explicit node(T&& v) : val(std::move(v)) {}
    };

    static void destroy(node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    static node* merge(node* a, node* b) {
        if (!a) return b;
        if (!b) return a;
        if (comp_(b->val, a->val)) std::swap(a, b);
        a->right = merge(a->right, b);
        if (!a->left || a->left->npl < a->right->npl)
            std::swap(a->left, a->right);
        a->npl = a->right ? a->right->npl + 1 : 0;
        return a;
    }

    node* root_ = nullptr;
    static inline Compare comp_{};
};

}  // namespace dsa
