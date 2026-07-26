#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class PairingHeap {
 public:
  PairingHeap() = default;

  explicit PairingHeap(const T& value) : root_(make_node(value)) { size_ = 1; }

  bool empty() const { return size_ == 0; }
  std::size_t size() const { return size_; }

  const T& top() const {
    if (empty()) throw std::runtime_error("top on empty PairingHeap");
    return root_->value;
  }

  void push(const T& value) {
    Node* node = make_node(value);
    if (!root_) {
      root_ = node;
    } else {
      root_ = meld(root_, node);
    }
    ++size_;
  }

  T pop() {
    if (empty()) throw std::runtime_error("pop on empty PairingHeap");
    T result = root_->value;
    Node* child = root_->child;
    delete root_;
    --size_;
    if (!child) {
      root_ = nullptr;
      return result;
    }
    root_ = two_pass_meld(child);
    return result;
  }

  void merge(PairingHeap& other) {
    if (this == &other) return;
    root_ = meld(root_, other.root_);
    size_ += other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
  }

  void clear() {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
  }

  ~PairingHeap() { clear(); }

 private:
  struct Node {
    T value;
    Node* child = nullptr;
    Node* sibling = nullptr;
    Node* prev = nullptr;
  };

  static Node* make_node(const T& value) {
    auto n = new Node();
    n->value = value;
    return n;
  }

  static Node* meld(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;
    if (comp_(b->value, a->value)) std::swap(a, b);
    b->sibling = a->child;
    if (a->child) a->child->prev = b;
    a->child = b;
    b->prev = a;
    return a;
  }

  static Node* two_pass_meld(Node* first) {
    if (!first || !first->sibling) return first;
    std::vector<Node*> heaps;
    Node* curr = first;
    while (curr) {
      Node* next = curr->sibling;
      curr->sibling = nullptr;
      curr->prev = nullptr;
      heaps.push_back(curr);
      curr = next;
    }
    std::vector<Node*> paired;
    for (std::size_t i = 1; i < heaps.size(); i += 2) {
      paired.push_back(meld(heaps[i - 1], heaps[i]));
    }
    if (heaps.size() % 2 == 1) {
      paired.push_back(heaps.back());
    }
    Node* result = paired.back();
    for (int i = static_cast<int>(paired.size()) - 2; i >= 0; --i) {
      result = meld(paired[i], result);
    }
    return result;
  }

  static void destroy(Node* n) {
    if (!n) return;
    destroy(n->child);
    destroy(n->sibling);
    delete n;
  }

  static inline Compare comp_{};
  Node* root_ = nullptr;
  std::size_t size_ = 0;
};

}  // namespace dsa
