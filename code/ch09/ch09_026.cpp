void inorder_iterative(node* root) {
    std::stack<node*> st;
    node* curr = root;
    while (curr || !st.empty()) {
        while (curr) { st.push(curr); curr = curr->left; }
        curr = st.top(); st.pop();
        visit(curr);
        curr = curr->right;
    }
}
