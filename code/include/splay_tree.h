#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class SplayTree {
 public:
  SplayTree() = default;

  SplayTree(const SplayTree& other) : root_(clone(other.root_)), size_(other.size_) {
    if (root_) root_->parent = nullptr;
  }

  SplayTree& operator=(const SplayTree& other) {
    if (this != &other) {
      clear();
      root_ = clone(other.root_);
      size_ = other.size_;
      if (root_) root_->parent = nullptr;
    }
    return *this;
  }

  SplayTree(SplayTree&& other) noexcept : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
    if (root_) root_->parent = nullptr;
  }

  SplayTree& operator=(SplayTree&& other) noexcept {
    if (this != &other) {
      clear();
      root_ = other.root_;
      size_ = other.size_;
      other.root_ = nullptr;
      other.size_ = 0;
      if (root_) root_->parent = nullptr;
    }
    return *this;
  }

  ~SplayTree() { clear(); }

  bool empty() const { return root_ == nullptr; }
  std::size_t size() const { return size_; }

  bool contains(const Key& key) {
    Node* n = find_node(root_, key);
    if (!n) return false;
    splay(n);
    return true;
  }

  const Key* find(const Key& key) {
    Node* n = find_node(root_, key);
    if (!n) return nullptr;
    splay(n);
    return &root_->key;
  }

  bool insert(const Key& key) {
    if (!root_) {
      root_ = new Node(key);
      ++size_;
      return true;
    }
    Node* curr = root_;
    Node* parent = nullptr;
    while (curr) {
      parent = curr;
      if (comp_(key, curr->key)) {
        curr = curr->left;
      } else if (comp_(curr->key, key)) {
        curr = curr->right;
      } else {
        splay(curr);
        return false;
      }
    }
    auto node = new Node(key);
    node->parent = parent;
    if (comp_(key, parent->key)) {
      parent->left = node;
    } else {
      parent->right = node;
    }
    ++size_;
    splay(node);
    return true;
  }

  bool erase(const Key& key) {
    Node* n = find_node(root_, key);
    if (!n) return false;
    splay(n);
    Node* to_delete = root_;
    if (!root_->left) {
      root_ = root_->right;
      if (root_) root_->parent = nullptr;
    } else {
      Node* left_sub = root_->left;
      Node* max_left = left_sub;
      while (max_left->right) max_left = max_left->right;
      splay(max_left);
      root_->right = to_delete->right;
      if (root_->right) root_->right->parent = root_;
    }
    delete to_delete;
    --size_;
    return true;
  }

  std::vector<Key> inorder() const {
    std::vector<Key> result;
    result.reserve(size_);
    inorder_collect(root_, result);
    return result;
  }

  void clear() {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
  }

 private:
  struct Node {
    Key key;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    explicit Node(const Key& k) : key(k) {}
  };

  Node* root_ = nullptr;
  std::size_t size_ = 0;
  Compare comp_;

  void rotate_right(Node* n) {
    Node* p = n->parent;
    Node* left = n->left;
    n->left = left->right;
    if (left->right) left->right->parent = n;
    left->right = n;
    left->parent = p;
    n->parent = left;
    if (!p) {
      root_ = left;
    } else if (p->left == n) {
      p->left = left;
    } else {
      p->right = left;
    }
  }

  void rotate_left(Node* n) {
    Node* p = n->parent;
    Node* right = n->right;
    n->right = right->left;
    if (right->left) right->left->parent = n;
    right->left = n;
    right->parent = p;
    n->parent = right;
    if (!p) {
      root_ = right;
    } else if (p->left == n) {
      p->left = right;
    } else {
      p->right = right;
    }
  }

  void splay(Node* x) {
    if (!x) return;
    while (x->parent) {
      Node* p = x->parent;
      Node* g = p->parent;
      if (!g) {
        if (p->left == x)
          rotate_right(p);
        else
          rotate_left(p);
      } else if (g->left == p && p->left == x) {
        rotate_right(g);
        rotate_right(p);
      } else if (g->right == p && p->right == x) {
        rotate_left(g);
        rotate_left(p);
      } else if (g->left == p && p->right == x) {
        rotate_left(p);
        rotate_right(g);
      } else {
        rotate_right(p);
        rotate_left(g);
      }
    }
  }

  Node* find_node(Node* n, const Key& key) const {
    while (n) {
      if (comp_(key, n->key))
        n = n->left;
      else if (comp_(n->key, key))
        n = n->right;
      else
        return n;
    }
    return nullptr;
  }

  Node* clone(Node* n) const {
    if (!n) return nullptr;
    auto c = new Node(n->key);
    c->left = clone(n->left);
    c->right = clone(n->right);
    if (c->left) c->left->parent = c;
    if (c->right) c->right->parent = c;
    return c;
  }

  static void destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
  }

  void inorder_collect(Node* n, std::vector<Key>& v) const {
    if (!n) return;
    inorder_collect(n->left, v);
    v.push_back(n->key);
    inorder_collect(n->right, v);
  }
};

}  // namespace dsa
