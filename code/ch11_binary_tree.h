#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <concepts>
#include <functional>
#include <memory>
#include <queue>
#include <span>
#include <stack>
#include <stdexcept>
#include <utility>

template <typename T>
struct binary_node {
    T                       data;
    std::unique_ptr<binary_node> left;
    std::unique_ptr<binary_node> right;

    explicit binary_node(const T& d) : data(d) {}
    explicit binary_node(T&& d)      : data(std::move(d)) {}
};

template <std::regular T>
class binary_tree {
public:
    using node_ptr = std::unique_ptr<binary_node<T>>;

    binary_tree() = default;

    explicit binary_tree(node_ptr root) : root_(std::move(root)) {}

    bool empty() const noexcept { return root_ == nullptr; }

    // ---- Traversals (recursive) ----
    template <typename Visitor>
    void preorder(Visitor&& visit) const {
        preorder(root_.get(), std::forward<Visitor>(visit));
    }

    template <typename Visitor>
    void inorder(Visitor&& visit) const {
        inorder(root_.get(), std::forward<Visitor>(visit));
    }

    template <typename Visitor>
    void postorder(Visitor&& visit) const {
        postorder(root_.get(), std::forward<Visitor>(visit));
    }

    // ---- Iterative traversals ----
    template <typename Visitor>
    void preorder_iterative(Visitor&& visit) const {
        if (!root_) return;
        std::stack<binary_node<T>*> s;
        s.push(root_.get());
        while (!s.empty()) {
            auto* n = s.top(); s.pop();
            visit(n->data);
            if (n->right) s.push(n->right.get());
            if (n->left)  s.push(n->left.get());
        }
    }

    template <typename Visitor>
    void inorder_iterative(Visitor&& visit) const {
        std::stack<binary_node<T>*> s;
        auto* cur = root_.get();
        while (cur || !s.empty()) {
            while (cur) {
                s.push(cur);
                cur = cur->left.get();
            }
            cur = s.top(); s.pop();
            visit(cur->data);
            cur = cur->right.get();
        }
    }

    // ---- Level-order ----
    template <typename Visitor>
    void level_order(Visitor&& visit) const {
        if (!root_) return;
        std::queue<binary_node<T>*> q;
        q.push(root_.get());
        while (!q.empty()) {
            auto* n = q.front(); q.pop();
            visit(n->data);
            if (n->left)  q.push(n->left.get());
            if (n->right) q.push(n->right.get());
        }
    }

    // ---- Properties ----
    std::size_t height() const { return height(root_.get()); }
    std::size_t size()   const { return size(root_.get()); }

    // ---- Morris in-order traversal (O(1) space) ----
    template <typename Visitor>
    void morris_inorder(Visitor&& visit) const {
        auto* cur = root_.get();
        while (cur) {
            if (!cur->left) {
                visit(cur->data);
                cur = cur->right.get();
            } else {
                auto* pre = cur->left.get();
                while (pre->right && pre->right.get() != cur)
                    pre = pre->right.get();
                if (!pre->right) {
                    pre->right = std::unique_ptr<binary_node<T>>(cur);
                    cur = cur->left.get();
                } else {
                    pre->right.reset();
                    visit(cur->data);
                    cur = cur->right.get();
                }
            }
        }
    }

    // ---- Build from traversals ----
    static binary_tree from_inorder_preorder(
        std::span<const T> inorder,
        std::span<const T> preorder) {
        std::size_t idx = 0;
        return binary_tree(build_ip(inorder, preorder, idx, 0,
                                    inorder.size() - 1));
    }

private:
    static node_ptr build_ip(std::span<const T> inorder,
                             std::span<const T> preorder,
                             std::size_t& pre_idx,
                             std::size_t in_l, std::size_t in_r) {
        if (in_l > in_r) return nullptr;
        T val = preorder[pre_idx++];
        auto n = std::make_unique<binary_node<T>>(val);
        if (in_l == in_r) return n;

        std::size_t split = in_l;
        while (split <= in_r && inorder[split] != val) ++split;

        n->left  = build_ip(inorder, preorder, pre_idx, in_l, split - 1);
        n->right = build_ip(inorder, preorder, pre_idx, split + 1, in_r);
        return n;
    }

    static std::size_t height(binary_node<T>* n) {
        if (!n) return 0;
        return 1 + std::max(height(n->left.get()), height(n->right.get()));
    }

    static std::size_t size(binary_node<T>* n) {
        if (!n) return 0;
        return 1 + size(n->left.get()) + size(n->right.get());
    }

    template <typename Visitor>
    static void preorder(binary_node<T>* n, Visitor&& visit) {
        if (!n) return;
        visit(n->data);
        preorder(n->left.get(), visit);
        preorder(n->right.get(), visit);
    }

    template <typename Visitor>
    static void inorder(binary_node<T>* n, Visitor&& visit) {
        if (!n) return;
        inorder(n->left.get(), visit);
        visit(n->data);
        inorder(n->right.get(), visit);
    }

    template <typename Visitor>
    static void postorder(binary_node<T>* n, Visitor&& visit) {
        if (!n) return;
        postorder(n->left.get(), visit);
        postorder(n->right.get(), visit);
        visit(n->data);
    }

    node_ptr root_;
};

#endif
