#pragma once
#include <functional>
#include <string>
#include <vector>

namespace dsa {

// Merkle tree: hash tree for verifying data integrity.
// Each leaf is the hash of a data block.
// Each internal node is the hash of its two children's hashes.
template <typename T>
class merkle_tree {
public:
    merkle_tree() = default;

    explicit merkle_tree(const std::vector<T>& data,
                         std::function<std::string(const T&)> hasher = {},
                         std::function<std::string(const std::string&, const std::string&)> combine = {})
        : hasher_(std::move(hasher)),
          combine_(std::move(combine)) {
        if (!hasher_) hasher_ = [](const T& v) { return std::to_string(v); };
        if (!combine_) combine_ = [](const std::string& a, const std::string& b) { return a + b; };
        build(data);
    }

    // Get the root hash
    std::string root_hash() const {
        return tree_.empty() ? "" : tree_[0];
    }

    // Verify a data block at index i with expected hash
    bool verify(int i, const std::string& expected_hash) const {
        if (i < 0 || i >= leaf_count_) return false;
        int node_idx = leaf_offset_ + i;
        return tree_[node_idx] == expected_hash;
    }

    // Get hash of a specific leaf
    std::string leaf_hash(int i) const {
        if (i < 0 || i >= leaf_count_) return "";
        return tree_[leaf_offset_ + i];
    }

    // Get the proof (sibling hashes) for a leaf at index i
    std::vector<std::string> proof(int i) const {
        std::vector<std::string> result;
        if (i < 0 || i >= leaf_count_) return result;
        int idx = leaf_offset_ + i;
        while (idx > 0) {
            int sibling = (idx % 2 == 0) ? idx - 1 : idx + 1;
            if (sibling < static_cast<int>(tree_.size()))
                result.push_back(tree_[sibling]);
            idx = (idx - 1) / 2;
        }
        return result;
    }

    // Verify a proof
    static bool verify_proof(const std::string& leaf_hash,
                             const std::vector<std::string>& proof,
                             const std::string& expected_root,
                             std::function<std::string(const std::string&, const std::string&)> combine = {}) {
        if (!combine) combine = [](const std::string& a, const std::string& b) { return a + b; };
        std::string h = leaf_hash;
        for (const auto& sibling : proof) {
            h = combine(h, sibling);
        }
        return h == expected_root;
    }

    bool empty() const { return leaf_count_ == 0; }
    int leaf_count() const { return leaf_count_; }

private:
    std::function<std::string(const T&)> hasher_;
    std::function<std::string(const std::string&, const std::string&)> combine_;
    std::vector<std::string> tree_;
    int leaf_offset_ = 0;
    int leaf_count_ = 0;

    void build(const std::vector<T>& data) {
        leaf_count_ = static_cast<int>(data.size());
        if (leaf_count_ == 0) return;

        // Compute leaf hashes
        std::vector<std::string> leaves(leaf_count_);
        for (int i = 0; i < leaf_count_; ++i)
            leaves[i] = hasher_(data[i]);

        // Build tree
        int n = 1;
        while (n < leaf_count_) n *= 2;
        leaf_offset_ = n - 1;
        tree_.resize(2 * n - 1);

        // Fill leaves
        for (int i = 0; i < leaf_count_; ++i)
            tree_[leaf_offset_ + i] = leaves[i];
        for (int i = leaf_count_; i < n; ++i)
            tree_[leaf_offset_ + i] = "";

        // Build internal nodes
        for (int i = leaf_offset_ - 1; i >= 0; --i) {
            std::string left = tree_[2 * i + 1];
            std::string right = tree_[2 * i + 2];
            if (left.empty()) tree_[i] = right;
            else if (right.empty()) tree_[i] = left;
            else tree_[i] = combine_(left, right);
        }
    }
};

}  // namespace dsa
