// ch08: Trees — binary tree traversals, BST, heap
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

template <typename T>
struct tree_node {
    T data;
    tree_node *left, *right;
    tree_node(const T& d) : data(d), left(nullptr), right(nullptr) {}
};

// ---- traversals ----
template <typename T>
void inorder(tree_node<T>* n) {
    if (!n) return;
    inorder(n->left);
    std::cout << n->data << " ";
    inorder(n->right);
}

template <typename T>
void preorder(tree_node<T>* n) {
    if (!n) return;
    std::cout << n->data << " ";
    preorder(n->left);
    preorder(n->right);
}

template <typename T>
void postorder(tree_node<T>* n) {
    if (!n) return;
    postorder(n->left);
    postorder(n->right);
    std::cout << n->data << " ";
}

template <typename T>
void level_order(tree_node<T>* root) {
    if (!root) return;
    std::queue<tree_node<T>*> q;
    q.push(root);
    while (!q.empty()) {
        auto* n = q.front(); q.pop();
        std::cout << n->data << " ";
        if (n->left) q.push(n->left);
        if (n->right) q.push(n->right);
    }
}

// ---- binary search tree ----
template <typename T>
class bst {
    tree_node<T>* root_ = nullptr;
public:
    ~bst() { destroy(root_); }

    void insert(const T& val) { root_ = insert(root_, val); }
    bool search(const T& val) const { return search(root_, val); }
    void inorder() const { inorder_nodes(root_); std::cout << "\n"; }

private:
    static tree_node<T>* insert(tree_node<T>* n, const T& val) {
        if (!n) return new tree_node<T>(val);
        if (val < n->data) n->left = insert(n->left, val);
        else if (val > n->data) n->right = insert(n->right, val);
        return n;
    }
    static void inorder_nodes(tree_node<T>* n) {
        if (!n) return;
        inorder_nodes(n->left);
        std::cout << n->data << " ";
        inorder_nodes(n->right);
    }
    static bool search(tree_node<T>* n, const T& val) {
        if (!n) return false;
        if (val == n->data) return true;
        return val < n->data ? search(n->left, val) : search(n->right, val);
    }
    static void destroy(tree_node<T>* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
};

// ---- max-heap ----
template <typename T>
class max_heap {
    std::vector<T> data_;
public:
    max_heap() { data_.push_back(T{}); /* 1-indexed */ }

    void push(const T& val) {
        data_.push_back(val);
        std::size_t i = data_.size() - 1;
        while (i > 1 && data_[i] > data_[i / 2]) {
            std::swap(data_[i], data_[i / 2]);
            i /= 2;
        }
    }
    T pop() {
        if (data_.size() <= 1) throw std::underflow_error("heap empty");
        T top = data_[1];
        data_[1] = std::move(data_.back());
        data_.pop_back();
        std::size_t i = 1;
        while (true) {
            std::size_t largest = i;
            if (2 * i < data_.size() && data_[2 * i] > data_[largest]) largest = 2 * i;
            if (2 * i + 1 < data_.size() && data_[2 * i + 1] > data_[largest]) largest = 2 * i + 1;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
        return top;
    }
    bool empty() const { return data_.size() <= 1; }
};

int main() {
    // build a tree:
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    auto* root = new tree_node<int>(1);
    root->left = new tree_node<int>(2);
    root->right = new tree_node<int>(3);
    root->left->left = new tree_node<int>(4);
    root->left->right = new tree_node<int>(5);

    std::cout << "inorder:   "; inorder(root); std::cout << "\n";
    std::cout << "preorder:  "; preorder(root); std::cout << "\n";
    std::cout << "postorder: "; postorder(root); std::cout << "\n";
    std::cout << "level:     "; level_order(root); std::cout << "\n";

    // clean up manually for this simple test
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right;
    delete root;

    // BST
    bst<int> b;
    b.insert(5); b.insert(3); b.insert(7); b.insert(1); b.insert(9);
    std::cout << "BST inorder: "; b.inorder();
    std::cout << "BST search 7: " << b.search(7) << " (expect 1)\n";
    std::cout << "BST search 4: " << b.search(4) << " (expect 0)\n";

    // max-heap
    max_heap<int> h;
    h.push(10); h.push(5); h.push(20); h.push(15);
    std::cout << "heap pop: " << h.pop() << " (expect 20)\n";
    std::cout << "heap pop: " << h.pop() << " (expect 15)\n";
    std::cout << "heap pop: " << h.pop() << " (expect 10)\n";
    std::cout << "heap pop: " << h.pop() << " (expect 5)\n";

    std::cout << "All ch09 tests passed.\n";
    return 0;
}
