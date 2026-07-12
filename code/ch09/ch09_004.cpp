template <typename Visitor>
void preorder_iterative(Visitor&& visit) const {
    if (!root_) return;
    std::stack<binary_node<T>*> s;
    s.push(root_.get());
    while (!s.empty()) {
        auto* node = s.top(); s.pop();
        visit(node->data);
        if (node->right) s.push(node->right.get());
        if (node->left) s.push(node->left.get());
    }
}

template <typename Visitor>
void inorder_iterative(Visitor&& visit) const {
    std::stack<binary_node<T>*> s;
    auto* current = root_.get();
    while (current || !s.empty()) {
        while (current) {
            s.push(current);
            current = current->left.get();
        }
        current = s.top(); s.pop();
        visit(current->data);
        current = current->right.get();
    }
}

template <typename Visitor>
void postorder_iterative(Visitor&& visit) const {
    if (!root_) return;
    std::stack<binary_node<T>*> s1, s2;
    s1.push(root_.get());
    while (!s1.empty()) {
        auto* node = s1.top(); s1.pop();
        s2.push(node);
        if (node->left) s1.push(node->left.get());
        if (node->right) s1.push(node->right.get());
    }
    while (!s2.empty()) {
        visit(s2.top()->data);
        s2.pop();
    }
}
