#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class TwoThreeTree {
 public:
  TwoThreeTree() = default;

  ~TwoThreeTree() { destroy(root_); }

  TwoThreeTree(const TwoThreeTree& other) : root_(clone(other.root_)), size_(other.size_) {}

  TwoThreeTree& operator=(const TwoThreeTree& other) {
    if (this != &other) {
      destroy(root_);
      root_ = clone(other.root_);
      size_ = other.size_;
    }
    return *this;
  }

  bool empty() const { return root_ == nullptr; }
  std::size_t size() const { return size_; }

  bool contains(const Key& key) const { return search(root_, key); }

  bool insert(const Key& key) {
    if (!root_) {
      root_ = new Node();
      root_->keys[0] = key;
      root_->count = 1;
      ++size_;
      return true;
    }
    bool added = insert_rec(root_, key);
    if (!added) return false;
    if (root_->count == 3) {
      Node* new_root = new Node();
      new_root->children[0] = root_;
      split_node(new_root, 0);
      root_ = new_root;
    }
    ++size_;
    return true;
  }

  bool erase(const Key& key) {
    if (!root_) return false;
    bool found = erase_impl(root_, key);
    if (found) --size_;
    return found;
  }

  std::vector<Key> inorder() const {
    std::vector<Key> result;
    result.reserve(size_);
    inorder_collect(root_, result);
    return result;
  }

 private:
  struct Node {
    Key keys[3]{};
    Node* children[4]{nullptr, nullptr, nullptr, nullptr};
    std::size_t count = 0;
  };

  Node* root_ = nullptr;
  std::size_t size_ = 0;
  Compare comp_;

  static bool is_leaf(Node* n) { return n && !n->children[0]; }

  bool search(Node* n, const Key& key) const {
    if (!n) return false;
    for (std::size_t i = 0; i < n->count; ++i) {
      if (!comp_(key, n->keys[i]) && !comp_(n->keys[i], key)) return true;
    }
    if (n->children[0]) {
      if (comp_(key, n->keys[0])) return search(n->children[0], key);
      if (n->count == 1 || comp_(n->keys[0], key)) {
        if (n->count == 2 && comp_(key, n->keys[1]))
          return search(n->children[1], key);
        if (n->count == 2 && comp_(n->keys[1], key))
          return search(n->children[2], key);
        if (n->count == 1) return search(n->children[1], key);
        return search(n->children[2], key);
      }
    }
    return false;
  }

  std::size_t child_index(Node* node, const Key& key) const {
    if (node->count == 1) return comp_(key, node->keys[0]) ? 0 : 1;
    if (comp_(key, node->keys[0])) return 0;
    if (comp_(node->keys[1], key)) return 2;
    return 1;
  }

  void split_node(Node* parent, std::size_t idx) {
    Node* child = parent->children[idx];
    if (child->count < 3) return;

    Key mid = child->keys[1];

    Node* left = new Node();
    left->keys[0] = child->keys[0];
    left->count = 1;

    Node* right = new Node();
    right->keys[0] = child->keys[2];
    right->count = 1;

    if (is_leaf(child)) {
      left->children[0] = nullptr;
      left->children[1] = nullptr;
      left->children[2] = nullptr;
      right->children[0] = nullptr;
      right->children[1] = nullptr;
      right->children[2] = nullptr;
    } else {
      left->children[0] = child->children[0];
      left->children[1] = child->children[1];
      left->children[2] = nullptr;
      right->children[0] = child->children[2];
      right->children[1] = child->children[3];
      right->children[2] = nullptr;
    }

    delete child;

    for (std::size_t i = parent->count; i > idx; --i) {
      parent->children[i + 1] = parent->children[i];
    }
    parent->children[idx + 1] = right;
    parent->children[idx] = left;

    for (std::size_t i = parent->count; i > idx; --i) {
      parent->keys[i] = parent->keys[i - 1];
    }
    parent->keys[idx] = mid;
    ++parent->count;
  }

  bool insert_rec(Node* node, const Key& key) {
    if (is_leaf(node)) {
      std::size_t i = node->count;
      while (i > 0 && comp_(key, node->keys[i - 1])) {
        node->keys[i] = node->keys[i - 1];
        --i;
      }
      if (i > 0 && !comp_(key, node->keys[i - 1]) && !comp_(node->keys[i - 1], key))
        return false;
      node->keys[i] = key;
      ++node->count;
      return true;
    }

    std::size_t idx = child_index(node, key);
    bool added = insert_rec(node->children[idx], key);

    if (node->children[idx]->count == 3) {
      split_node(node, idx);
    }

    return added;
  }

  Node* find_min_node(Node* n) const {
    while (!is_leaf(n)) n = n->children[0];
    return n;
  }

  bool erase_impl(Node* node, const Key& key) {
    if (!node) return false;

    for (std::size_t i = 0; i < node->count; ++i) {
      if (!comp_(key, node->keys[i]) && !comp_(node->keys[i], key)) {
        if (is_leaf(node)) {
          for (std::size_t j = i; j + 1 < node->count; ++j) {
            node->keys[j] = node->keys[j + 1];
          }
          --node->count;
          return true;
        }
        Node* pred = node->children[i];
        while (!is_leaf(pred)) pred = pred->children[pred->count];
        node->keys[i] = pred->keys[pred->count - 1];
        return erase_impl(pred, pred->keys[pred->count - 1]);
      }
    }

    std::size_t idx = node->count;
    for (std::size_t i = 0; i < node->count; ++i) {
      if (comp_(key, node->keys[i])) {
        idx = i;
        break;
      }
    }
    return erase_impl(node->children[idx], key);
  }

  Node* clone(Node* n) const {
    if (!n) return nullptr;
    auto c = new Node();
    for (std::size_t i = 0; i < 3; ++i) c->keys[i] = n->keys[i];
    c->count = n->count;
    for (int i = 0; i < 4; ++i) {
      c->children[i] = clone(n->children[i]);
    }
    return c;
  }

  static void destroy(Node* n) {
    if (!n) return;
    for (int i = 0; i < 4; ++i) destroy(n->children[i]);
    delete n;
  }

  void inorder_collect(Node* n, std::vector<Key>& v) const {
    if (!n) return;
    if (n->count >= 1) {
      inorder_collect(n->children[0], v);
      v.push_back(n->keys[0]);
    }
    if (n->count == 2) {
      inorder_collect(n->children[1], v);
      v.push_back(n->keys[1]);
      inorder_collect(n->children[2], v);
    } else if (n->count == 1) {
      inorder_collect(n->children[1], v);
    }
  }
};

}  // namespace dsa
