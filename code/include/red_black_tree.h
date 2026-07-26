#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class RedBlackTree {
 public:
  RedBlackTree() = default;

  RedBlackTree(const RedBlackTree& other) : root_(clone(other.root_)), size_(other.size_) {}

  RedBlackTree& operator=(const RedBlackTree& other) {
    if (this != &other) {
      clear();
      root_ = clone(other.root_);
      size_ = other.size_;
    }
    return *this;
  }

  RedBlackTree(RedBlackTree&& other) noexcept : root_(other.root_) {
    other.root_ = nullptr;
  }

  RedBlackTree& operator=(RedBlackTree&& other) noexcept {
    if (this != &other) {
      clear();
      root_ = other.root_;
      other.root_ = nullptr;
    }
    return *this;
  }

  ~RedBlackTree() { clear(); }

  bool empty() const { return root_ == nullptr; }
  std::size_t size() const { return size_; }

  bool contains(const Key& key) const { return find(key) != nullptr; }

  const Key* find(const Key& key) const {
    Node* n = root_;
    while (n) {
      if (comp_(key, n->key)) n = n->left;
      else if (comp_(n->key, key)) n = n->right;
      else return &n->key;
    }
    return nullptr;
  }

  bool insert(const Key& key) { return insert_impl(key); }

  bool erase(const Key& key) {
    Node* n = find_node(root_, key);
    if (!n) return false;
    erase_node(n);
    --size_;
    return true;
  }

  void clear() {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
  }

  std::vector<Key> inorder() const {
    std::vector<Key> result;
    result.reserve(size_);
    inorder_collect(root_, result);
    return result;
  }

 private:
  enum class Color { Red, Black };

  struct Node {
    Key key;
    Color color = Color::Red;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;

    explicit Node(const Key& k) : key(k) {}
  };

  Node* root_ = nullptr;
  std::size_t size_ = 0;
  Compare comp_;

  static Node* minimum(Node* n) {
    while (n && n->left) n = n->left;
    return n;
  }

  static Node* maximum(Node* n) {
    while (n && n->right) n = n->right;
    return n;
  }

  static Node* successor(Node* n) {
    if (n->right) return minimum(n->right);
    Node* p = n->parent;
    while (p && n == p->right) { n = p; p = p->parent; }
    return p;
  }

  static Node* predecessor(Node* n) {
    if (n->left) return maximum(n->left);
    Node* p = n->parent;
    while (p && n == p->left) { n = p; p = p->parent; }
    return p;
  }

  void rotate_left(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root_ = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void rotate_right(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) root_ = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void insert_fixup(Node* z) {
    while (z->parent && z->parent->color == Color::Red) {
      Node* gp = z->parent->parent;
      if (z->parent == gp->left) {
        Node* uncle = gp->right;
        if (uncle && uncle->color == Color::Red) {
          z->parent->color = Color::Black;
          uncle->color = Color::Black;
          gp->color = Color::Red;
          z = gp;
        } else {
          if (z == z->parent->right) {
            z = z->parent;
            rotate_left(z);
          }
          z->parent->color = Color::Black;
          z->parent->parent->color = Color::Red;
          rotate_right(z->parent->parent);
        }
      } else {
        Node* uncle = gp->left;
        if (uncle && uncle->color == Color::Red) {
          z->parent->color = Color::Black;
          uncle->color = Color::Black;
          gp->color = Color::Red;
          z = gp;
        } else {
          if (z == z->parent->left) {
            z = z->parent;
            rotate_right(z);
          }
          z->parent->color = Color::Black;
          z->parent->parent->color = Color::Red;
          rotate_left(z->parent->parent);
        }
      }
    }
    root_->color = Color::Black;
  }

  void transplant(Node* u, Node* v) {
    if (!u->parent) root_ = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
  }

  void erase_node(Node* z) {
    Node* y = z;
    Node* x = nullptr;
    Color y_orig = y->color;

    if (!z->left) {
      x = z->right;
      transplant(z, z->right);
    } else if (!z->right) {
      x = z->left;
      transplant(z, z->left);
    } else {
      y = minimum(z->right);
      y_orig = y->color;
      x = y->right;
      if (y->parent == z) {
        if (x) x->parent = y;
      } else {
        transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }

    delete z;

    if (y_orig == Color::Black && x) {
      erase_fixup(x);
    }
  }

  void erase_fixup(Node* x) {
    while (x != root_ && x->color == Color::Black) {
      if (x == x->parent->left) {
        Node* w = x->parent->right;
        if (w && w->color == Color::Red) {
          w->color = Color::Black;
          x->parent->color = Color::Red;
          rotate_left(x->parent);
          w = x->parent->right;
        }
        bool left_black = (!w->left || w->left->color == Color::Black);
        bool right_black = (!w->right || w->right->color == Color::Black);
        if (left_black && right_black) {
          w->color = Color::Red;
          x = x->parent;
        } else {
          if (right_black) {
            if (w->left) w->left->color = Color::Black;
            w->color = Color::Red;
            rotate_right(w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = Color::Black;
          if (w->right) w->right->color = Color::Black;
          rotate_left(x->parent);
          x = root_;
        }
      } else {
        Node* w = x->parent->left;
        if (w && w->color == Color::Red) {
          w->color = Color::Black;
          x->parent->color = Color::Red;
          rotate_right(x->parent);
          w = x->parent->left;
        }
        bool left_black = (!w->left || w->left->color == Color::Black);
        bool right_black = (!w->right || w->right->color == Color::Black);
        if (left_black && right_black) {
          w->color = Color::Red;
          x = x->parent;
        } else {
          if (left_black) {
            if (w->right) w->right->color = Color::Black;
            w->color = Color::Red;
            rotate_left(w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = Color::Black;
          if (w->left) w->left->color = Color::Black;
          rotate_right(x->parent);
          x = root_;
        }
      }
    }
    if (x) x->color = Color::Black;
  }

  bool insert_impl(const Key& key) {
    Node* parent = nullptr;
    Node* curr = root_;
    while (curr) {
      parent = curr;
      if (comp_(key, curr->key)) curr = curr->left;
      else if (comp_(curr->key, key)) curr = curr->right;
      else return false;
    }
    auto z = std::make_unique<Node>(key);
    z->parent = parent;
    if (!parent) root_ = z.release();
    else if (comp_(key, parent->key)) { parent->left = z.release(); parent->left->parent = parent; }
    else { parent->right = z.release(); parent->right->parent = parent; }
    ++size_;
    if (parent) insert_fixup(parent->left ? parent->left : parent->right);
    else root_->color = Color::Black;
    return true;
  }

  Node* find_node(Node* n, const Key& key) const {
    while (n) {
      if (comp_(key, n->key)) n = n->left;
      else if (comp_(n->key, key)) n = n->right;
      else return n;
    }
    return nullptr;
  }

  Node* clone(Node* n) const {
    if (!n) return nullptr;
    auto c = std::make_unique<Node>(n->key);
    c->color = n->color;
    c->left = clone(n->left);
    c->right = clone(n->right);
    if (c->left) c->left->parent = c.get();
    if (c->right) c->right->parent = c.get();
    return c.release();
  }

  void destroy(Node* n) {
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
