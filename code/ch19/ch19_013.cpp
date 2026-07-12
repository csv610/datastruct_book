#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <stdexcept>

class merkle_tree {
public:
    using hash_fn = std::function<std::string(const std::string&)>;

    merkle_tree(std::vector<std::string> data, hash_fn h)
        : hash_(std::move(h)), data_(std::move(data)) {
        size_t n = data_.size();
        if (n == 0)
            throw std::invalid_argument("empty data");
        // Round up to next power of 2
        size_t leaves = 1;
        while (leaves < n) leaves *= 2;
        tree_.resize(2 * leaves, "");
        // Compute leaf hashes
        for (size_t i = 0; i < leaves; ++i) {
            if (i < n)
                tree_[leaves + i] = hash_(data_[i]);
            else
                tree_[leaves + i] = hash_("");  // padding
        }
        // Build internal nodes bottom-up
        for (size_t i = leaves - 1; i >= 1; --i)
            tree_[i] = hash_(tree_[2 * i] + tree_[2 * i + 1]);
        leaves_ = leaves;
    }

    std::string get_root_hash() const {
        return tree_[1];
    }

    // Merkle proof: list of sibling hashes from leaf to root
    std::vector<std::pair<std::string, bool>>
    get_proof(size_t index) const {
        std::vector<std::pair<std::string, bool>> proof;
        size_t pos = leaves_ + index;
        while (pos > 1) {
            bool is_left = (pos % 2 == 1);
            size_t sibling = is_left ? pos - 1 : pos + 1;
            proof.emplace_back(tree_[sibling], is_left);
            pos /= 2;
        }
        return proof;
    }

    // Verify that data_block is at index using the proof
    static bool verify_proof(
        hash_fn h,
        const std::string& data_block,
        size_t index,
        const std::string& expected_root,
        const std::vector<std::pair<std::string, bool>>& proof)
    {
        std::string current = h(data_block);
        for (const auto& [sibling_hash, is_left] : proof) {
            if (is_left)
                current = h(sibling_hash + current);
            else
                current = h(current + sibling_hash);
        }
        return current == expected_root;
    }

private:
    hash_fn hash_;
    std::vector<std::string> data_;
    std::vector<std::string> tree_;
    size_t leaves_;
};
