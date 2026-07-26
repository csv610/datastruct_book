#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dsa {

class SuffixTree {
 public:
  explicit SuffixTree(const std::string& s) : text_(s) { build(); }

  SuffixTree(const SuffixTree&) = delete;
  SuffixTree& operator=(const SuffixTree&) = delete;

  ~SuffixTree() { destroy(root_); }

  bool contains(const std::string& pattern) const {
    return find_node(pattern) != nullptr;
  }

  std::vector<std::size_t> search(const std::string& pattern) const {
    std::vector<std::size_t> results;
    Node* node = find_node(pattern);
    if (!node) return results;
    collect_leaves(node, results);
    std::sort(results.begin(), results.end());
    return results;
  }

  std::size_t count_occurrences(const std::string& pattern) const {
    return search(pattern).size();
  }

  std::string longest_repeated_substring() const {
    std::string best;
    longest_repeated_dfs(root_, std::string(), best);
    return best;
  }

  std::size_t node_count() const { return count_nodes(root_); }

 private:
  struct Node {
    std::size_t start = 0;
    std::size_t end = 0;
    std::ptrdiff_t suffix_idx = -1;
    std::unordered_map<char, Node*> children;
    bool is_leaf() const { return children.empty(); }
    std::size_t edge_length() const { return end - start; }
  };

  std::string text_;
  Node* root_ = nullptr;

  Node* make_node(std::size_t start, std::size_t end) {
    auto node = new Node();
    node->start = start;
    node->end = end;
    return node;
  }

  void destroy(Node* n) {
    if (!n) return;
    for (auto& [ch, child] : n->children) destroy(child);
    delete n;
  }

  void build() {
    root_ = make_node(0, 0);
    for (std::size_t i = 0; i < text_.size(); ++i) {
      insert_suffix(i);
    }
  }

  void insert_suffix(std::size_t suffix_start) {
    Node* curr = root_;
    std::size_t pos = suffix_start;

    while (pos < text_.size()) {
      char c = text_[pos];
      auto it = curr->children.find(c);

      if (it == curr->children.end()) {
        auto leaf = make_node(pos, text_.size());
        leaf->suffix_idx = static_cast<std::ptrdiff_t>(suffix_start);
        curr->children[c] = leaf;
        return;
      }

      Node* edge = it->second;
      std::size_t edge_len = edge->edge_length();

      std::size_t match = 0;
      while (match < edge_len && pos + match < text_.size() &&
             text_[edge->start + match] == text_[pos + match]) {
        ++match;
      }

      if (match == edge_len) {
        curr = edge;
        pos += match;
        continue;
      }

      Node* split = make_node(edge->start, edge->start + match);
      curr->children[c] = split;

      edge->start += match;
      split->children[text_[edge->start]] = edge;

      if (pos + match < text_.size()) {
        auto leaf = make_node(pos + match, text_.size());
        leaf->suffix_idx = static_cast<std::ptrdiff_t>(suffix_start);
        split->children[text_[pos + match]] = leaf;
      } else {
        split->suffix_idx = static_cast<std::ptrdiff_t>(suffix_start);
      }
      return;
    }
  }

  Node* find_node(const std::string& pattern) const {
    Node* curr = root_;
    std::size_t pos = 0;
    while (pos < pattern.size()) {
      char c = pattern[pos];
      auto it = curr->children.find(c);
      if (it == curr->children.end()) return nullptr;
      Node* child = it->second;
      std::size_t edge_len = child->edge_length();
      std::size_t remaining = pattern.size() - pos;
      std::size_t check = std::min(edge_len, remaining);
      for (std::size_t i = 0; i < check; ++i) {
        if (text_[child->start + i] != pattern[pos + i]) return nullptr;
      }
      pos += edge_len;
      curr = child;
    }
    return curr;
  }

  void collect_leaves(Node* node, std::vector<std::size_t>& results) const {
    if (!node) return;
    if (node->suffix_idx >= 0) {
      results.push_back(static_cast<std::size_t>(node->suffix_idx));
    }
    for (auto& [ch, child] : node->children) {
      collect_leaves(child, results);
    }
  }

  void longest_repeated_dfs(Node* node, std::string path, std::string& best) const {
    if (!node) return;
    bool is_repeated = (node->children.size() >= 2) ||
                       (node->suffix_idx >= 0 && node->children.size() >= 1);
    if (node != root_ && is_repeated) {
      if (path.size() > best.size()) best = path;
    }
    for (auto& [ch, child] : node->children) {
      std::size_t len = child->edge_length();
      std::string edge(text_.begin() + static_cast<std::ptrdiff_t>(child->start),
                       text_.begin() + static_cast<std::ptrdiff_t>(child->start + len));
      longest_repeated_dfs(child, path + edge, best);
    }
  }

  std::size_t count_nodes(Node* node) const {
    if (!node) return 0;
    std::size_t count = 1;
    for (auto& [ch, child] : node->children) {
      count += count_nodes(child);
    }
    return count;
  }
};

}  // namespace dsa
