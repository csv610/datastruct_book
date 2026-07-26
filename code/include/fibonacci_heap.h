#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class FibonacciHeap {
 public:
  FibonacciHeap() = default;

  explicit FibonacciHeap(const T& value) {
    min_ = make_node(value);
    min_->left = min_;
    min_->right = min_;
    size_ = 1;
  }

  bool empty() const { return size_ == 0; }
  std::size_t size() const { return size_; }

  const T& top() const {
    if (empty()) throw std::runtime_error("top on empty FibonacciHeap");
    return min_->value;
  }

  void push(const T& value) {
    Node* node = make_node(value);
    if (!min_) {
      node->left = node;
      node->right = node;
      min_ = node;
    } else {
      insert_into_root_list(node);
      if (comp_(node->value, min_->value)) min_ = node;
    }
    ++size_;
  }

  T pop() {
    if (empty()) throw std::runtime_error("pop on empty FibonacciHeap");
    T result = min_->value;
    if (min_->child) {
      Node* c = min_->child;
      do {
        Node* next = c->right;
        c->parent = nullptr;
        insert_into_root_list(c);
        c = next;
      } while (c != min_->child);
    }
    Node* next_root = (min_->right != min_) ? min_->right : nullptr;
    remove_from_root_list(min_);
    delete min_;
    --size_;
    if (size_ == 0) {
      min_ = nullptr;
    } else {
      min_ = consolidate(next_root);
    }
    return result;
  }

  void merge(FibonacciHeap& other) {
    if (this == &other) return;
    if (!other.min_) return;
    if (!min_) {
      min_ = other.min_;
      size_ = other.size_;
    } else {
      concatenate_root_lists(min_, other.min_);
      if (comp_(other.min_->value, min_->value)) min_ = other.min_;
      size_ += other.size_;
    }
    other.min_ = nullptr;
    other.size_ = 0;
  }

  void clear() {
    if (min_) {
      Node* start = min_;
      do {
        Node* next = start->right;
        destroy(start);
        start = next;
      } while (start != min_);
    }
    min_ = nullptr;
    size_ = 0;
  }

  ~FibonacciHeap() { clear(); }

 private:
  struct Node {
    T value;
    std::size_t degree = 0;
    bool mark = false;
    Node* parent = nullptr;
    Node* child = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
  };

  static Node* make_node(const T& value) {
    auto n = new Node();
    n->value = value;
    return n;
  }

  void insert_into_root_list(Node* node) {
    node->left = min_->left;
    node->right = min_;
    min_->left->right = node;
    min_->left = node;
  }

  void remove_from_root_list(Node* node) {
    node->left->right = node->right;
    node->right->left = node->left;
  }

  static void concatenate_root_lists(Node* a, Node* b) {
    Node* a_end = a->left;
    Node* b_end = b->left;
    a->left = b_end;
    b_end->right = a;
    b->left = a_end;
    a_end->right = b;
  }

  Node* consolidate(Node* start) {
    std::vector<Node*> A(max_degree() + 1, nullptr);
    std::vector<Node*> roots;
    Node* curr = start;
    do {
      roots.push_back(curr);
      curr = curr->right;
    } while (curr != start);

    for (Node* w : roots) {
      Node* x = w;
      std::size_t d = x->degree;
      while (d < A.size() && A[d]) {
        Node* y = A[d];
        if (comp_(y->value, x->value)) std::swap(x, y);
        link_nodes(y, x);
        A[d] = nullptr;
        ++d;
      }
      if (d >= A.size()) A.resize(d + 1, nullptr);
      A[d] = x;
    }

    Node* new_min = nullptr;
    for (Node* n : A) {
      if (!n) continue;
      n->left = n;
      n->right = n;
      if (!new_min) {
        new_min = n;
      } else {
        concatenate_root_lists(new_min, n);
        if (comp_(n->value, new_min->value)) new_min = n;
      }
    }
    return new_min;
  }

  void link_nodes(Node* y, Node* x) {
    remove_from_root_list(y);
    y->parent = x;
    if (!x->child) {
      x->child = y;
      y->left = y;
      y->right = y;
    } else {
      y->left = x->child->left;
      y->right = x->child;
      x->child->left->right = y;
      x->child->left = y;
    }
    ++x->degree;
    y->mark = false;
  }

  std::size_t max_degree() const {
    return static_cast<std::size_t>(2.08 * std::log2(static_cast<double>(size_))) + 1;
  }

  static void destroy(Node* n) {
    if (!n) return;
    if (n->child) {
      Node* c = n->child;
      do {
        Node* next = c->right;
        destroy(c);
        c = next;
      } while (c != n->child);
    }
    delete n;
  }

  static inline Compare comp_{};
  Node* min_ = nullptr;
  std::size_t size_ = 0;
};

}  // namespace dsa
