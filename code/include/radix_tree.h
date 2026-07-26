#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dsa {

template <typename V>
class RadixTree {
 public:
  RadixTree() : root_(std::make_unique<Node>()) {}

  void insert(const std::string& key, const V& value) {
    insert_impl(root_.get(), key, 0, value);
  }

  bool contains(const std::string& key) const {
    Node* node = find_impl(root_.get(), key, 0);
    return node && node->has_value;
  }

  const V* find(const std::string& key) const {
    Node* node = find_impl(root_.get(), key, 0);
    if (!node || !node->has_value) return nullptr;
    return &node->value;
  }

  bool erase(const std::string& key) { return erase_impl(root_.get(), key, 0); }

  std::vector<std::pair<std::string, V>> prefix_search(const std::string& prefix) const {
    std::vector<std::pair<std::string, V>> results;
    Node* node = find_impl(root_.get(), prefix, 0);
    if (node) collect(node, prefix, results);
    return results;
  }

  std::size_t size() const { return size_; }

 private:
  struct Node {
    std::string edge;
    bool has_value = false;
    V value;
    std::unordered_map<char, std::unique_ptr<Node>> children;
  };

  std::unique_ptr<Node> root_;
  std::size_t size_ = 0;

  void insert_impl(Node* node, const std::string& key, std::size_t depth, const V& value) {
    if (depth == key.size()) {
      if (!node->has_value) ++size_;
      node->has_value = true;
      node->value = value;
      return;
    }
    char c = key[depth];
    auto it = node->children.find(c);
    if (it == node->children.end()) {
      auto child = std::make_unique<Node>();
      child->edge = key.substr(depth);
      child->has_value = true;
      child->value = value;
      node->children[c] = std::move(child);
      ++size_;
      return;
    }
    Node* child = it->second.get();
    std::size_t common = common_prefix_length(child->edge, key.substr(depth));
    if (common == child->edge.size()) {
      insert_impl(child, key, depth + common, value);
    } else {
      auto split = std::make_unique<Node>();
      split->edge = child->edge.substr(0, common);
      std::string remaining = child->edge.substr(common);
      child->edge = remaining;
      auto child_ptr = std::move(node->children[c]);
      split->children[remaining[0]] = std::move(child_ptr);
      node->children[c] = std::move(split);
      insert_impl(node->children[c].get(), key, depth + common, value);
    }
  }

  Node* find_impl(Node* node, const std::string& key, std::size_t depth) const {
    if (depth == key.size()) return node;
    char c = key[depth];
    auto it = node->children.find(c);
    if (it == node->children.end()) return nullptr;
    Node* child = it->second.get();
    std::size_t common = common_prefix_length(child->edge, key.substr(depth));
    if (common < child->edge.size()) return nullptr;
    return find_impl(child, key, depth + common);
  }

  bool erase_impl(Node* node, const std::string& key, std::size_t depth) {
    if (depth == key.size()) {
      if (!node->has_value) return false;
      node->has_value = false;
      --size_;
      return true;
    }
    char c = key[depth];
    auto it = node->children.find(c);
    if (it == node->children.end()) return false;
    Node* child = it->second.get();
    std::size_t common = common_prefix_length(child->edge, key.substr(depth));
    if (common < child->edge.size()) return false;
    return erase_impl(child, key, depth + common);
  }

  void collect(Node* node, const std::string& prefix,
               std::vector<std::pair<std::string, V>>& results) const {
    if (node->has_value) {
      results.push_back({prefix, node->value});
    }
    for (auto& [ch, child] : node->children) {
      collect(child.get(), prefix + child->edge, results);
    }
  }

  static std::size_t common_prefix_length(const std::string& a, const std::string& b) {
    std::size_t len = std::min(a.size(), b.size());
    std::size_t i = 0;
    while (i < len && a[i] == b[i]) ++i;
    return i;
  }
};

}  // namespace dsa
