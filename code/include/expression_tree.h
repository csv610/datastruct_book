#pragma once
#include <cctype>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace dsa {

struct expr_node {
    std::string value;
    std::unique_ptr<expr_node> left;
    std::unique_ptr<expr_node> right;

    explicit expr_node(std::string val) : value(std::move(val)) {}
};

inline bool is_operator(const std::string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

// Build expression tree from postfix expression
inline std::unique_ptr<expr_node> build_expr_tree(
    const std::vector<std::string>& postfix) {

    std::stack<std::unique_ptr<expr_node>> stk;
    for (const auto& token : postfix) {
        if (is_operator(token)) {
            auto right = std::move(stk.top()); stk.pop();
            auto left = std::move(stk.top()); stk.pop();
            auto node = std::make_unique<expr_node>(token);
            node->left = std::move(left);
            node->right = std::move(right);
            stk.push(std::move(node));
        } else {
            stk.push(std::make_unique<expr_node>(token));
        }
    }
    return stk.empty() ? nullptr : std::move(stk.top());
}

// Inorder traversal (infix)
inline void inorder_traversal(const expr_node* node, std::vector<std::string>& out) {
    if (!node) return;
    bool need_parens = node->left && is_operator(node->value);
    if (need_parens) out.push_back("(");
    inorder_traversal(node->left.get(), out);
    out.push_back(node->value);
    inorder_traversal(node->right.get(), out);
    if (need_parens) out.push_back(")");
}

// Postorder traversal (postfix)
inline void postorder_traversal(const expr_node* node, std::vector<std::string>& out) {
    if (!node) return;
    postorder_traversal(node->left.get(), out);
    postorder_traversal(node->right.get(), out);
    out.push_back(node->value);
}

// Evaluate expression tree
inline double eval_expr_tree(const expr_node* node) {
    if (!node) return 0.0;
    if (!node->left && !node->right)
        return std::stod(node->value);
    double l = eval_expr_tree(node->left.get());
    double r = eval_expr_tree(node->right.get());
    if (node->value == "+") return l + r;
    if (node->value == "-") return l - r;
    if (node->value == "*") return l * r;
    if (node->value == "/") return l / r;
    return 0.0;
}

}  // namespace dsa
