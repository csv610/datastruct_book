#pragma once
#include <algorithm>
#include <utility>

namespace dsa {

// Skew heap: self-adjusting meldable heap (Brass Ch5 §5.5)
// All operations amortized O(log n). Supports push, pop, top, meld.
template <typename T, typename Compare = std::less<T>>
class skew_heap {
public:
    skew_heap() = default;

    skew_heap(skew_heap&& o) noexcept : root_(std::exchange(o.root_, nullptr)) {}
    skew_heap& operator=(skew_heap&& o) noexcept {
        if (this != &o) { destroy(root_); root_ = std::exchange(o.root_, nullptr); }
        return *this;
    }

    skew_heap(const skew_heap&) = delete;
    skew_heap& operator=(const skew_heap&) = delete;

    ~skew_heap() { destroy(root_); }

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

    void meld(skew_heap&& other) {
        root_ = merge(root_, other.root_);
        other.root_ = nullptr;
    }

    bool empty() const { return !root_; }

private:
    struct node {
        T val;
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

    static void swap_children(node* n) {
        std::swap(n->left, n->right);
    }

    static node* merge(node* a, node* b) {
        if (!a) return b;
        if (!b) return a;
        if (comp_(b->val, a->val)) std::swap(a, b);
        a->right = merge(a->right, b);
        swap_children(a);
        return a;
    }

    node* root_ = nullptr;
    static inline Compare comp_{};
};

}  // namespace dsa
