#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class BinomialHeap {
 public:
  BinomialHeap() = default;

  explicit BinomialHeap(const T& value) : size_(1) {
    roots_.push_back(make_node(value));
  }

  BinomialHeap(const BinomialHeap&) = delete;
  BinomialHeap& operator=(const BinomialHeap&) = delete;
  BinomialHeap(BinomialHeap&&) = default;
  BinomialHeap& operator=(BinomialHeap&&) = default;

  ~BinomialHeap() { clear(); }

  bool empty() const { return size_ == 0; }
  std::size_t size() const { return size_; }

  const T& top() const {
    if (empty()) throw std::runtime_error("top on empty BinomialHeap");
    Node* best = roots_[0];
    for (std::size_t i = 1; i < roots_.size(); ++i) {
      if (comp_(roots_[i]->key, best->key)) best = roots_[i];
    }
    return best->key;
  }

  void push(const T& value) {
    BinomialHeap tmp(value);
    merge(tmp);
  }

  T pop() {
    if (empty()) throw std::runtime_error("pop on empty BinomialHeap");
    std::size_t best = 0;
    for (std::size_t i = 1; i < roots_.size(); ++i) {
      if (comp_(roots_[i]->key, roots_[best]->key)) best = i;
    }
    Node* removed = roots_[best];
    T result = removed->key;

    std::vector<Node*> child_roots;
    for (Node* c : removed->children) {
      child_roots.push_back(c);
    }
    removed->children.clear();
    delete removed;
    roots_.erase(roots_.begin() + static_cast<std::ptrdiff_t>(best));

    for (Node* c : child_roots) {
      roots_.push_back(c);
    }
    combine_roots();
    --size_;
    return result;
  }

  void merge(BinomialHeap& other) {
    if (this == &other || other.empty()) return;

    for (Node* r : other.roots_) {
      roots_.push_back(r);
    }
    size_ += other.size_;
    other.roots_.clear();
    other.size_ = 0;
    combine_roots();
  }

  void clear() {
    for (Node* r : roots_) destroy(r);
    roots_.clear();
    size_ = 0;
  }

 private:
  struct Node {
    T key;
    std::size_t degree = 0;
    std::vector<Node*> children;
  };

  static Node* make_node(const T& value) {
    auto n = new Node();
    n->key = value;
    return n;
  }

  static Node* join(Node* a, Node* b) {
    if (comp_(b->key, a->key)) std::swap(a, b);
    a->children.push_back(b);
    ++a->degree;
    return a;
  }

  void combine_roots() {
    if (roots_.empty()) return;
    std::sort(roots_.begin(), roots_.end(),
              [](Node* a, Node* b) { return a->degree < b->degree; });

    std::vector<Node*> result;
    for (Node* node : roots_) {
      Node* carry = node;
      while (!result.empty() && result.back()->degree == carry->degree) {
        Node* prev = result.back();
        result.pop_back();
        carry = join(carry, prev);
      }
      result.push_back(carry);
    }
    roots_ = std::move(result);
  }

  static void destroy(Node* n) {
    if (!n) return;
    for (Node* c : n->children) destroy(c);
    delete n;
  }

  static inline Compare comp_{};
  std::vector<Node*> roots_;
  std::size_t size_ = 0;
};

}  // namespace dsa
