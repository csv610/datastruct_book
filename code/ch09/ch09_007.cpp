struct expr_node {
    std::string value;      // operator or operand
    std::unique_ptr<expr_node> left;
    std::unique_ptr<expr_node> right;
    
    bool is_operator() const {
        return value == "+" || value == "-" || value == "*" || value == "/";
    }
};

int evaluate(const expr_node* node) {
    if (!node->is_operator()) {
        return std::stoi(node->value);
    }
    int l = evaluate(node->left.get());
    int r = evaluate(node->right.get());
    if (node->value == "+") return l + r;
    if (node->value == "-") return l - r;
    if (node->value == "*") return l * r;
    return l / r;
}
