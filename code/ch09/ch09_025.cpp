#include <string>
#include <sstream>
#include <memory>
#include <vector>

struct TreeNode {
    int val;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
    explicit TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ---------- Serialize ----------
void serialize(TreeNode* node, std::ostringstream& os) {
    if (!node) { os << "# "; return; }
    os << node->val << " ";
    serialize(node->left.get(),  os);
    serialize(node->right.get(), os);
}

std::string serialize(TreeNode* root) {
    std::ostringstream os;
    serialize(root, os);
    return os.str();
}

// ---------- Deserialize ----------
void deserialize_helper(std::istringstream& iss,
                        std::unique_ptr<TreeNode>& node) {
    std::string token;
    iss >> token;
    if (token == "#") return;
    node = std::make_unique<TreeNode>(std::stoi(token));
    deserialize_helper(iss, node->left);
    deserialize_helper(iss, node->right);
}

std::unique_ptr<TreeNode> deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::unique_ptr<TreeNode> root = nullptr;
    deserialize_helper(iss, root);
    return root;
}
