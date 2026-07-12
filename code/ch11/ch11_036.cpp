class bst_iterator {
    std::stack<node*> st;
    void push_left(node* n) { while (n) { st.push(n); n = n->left; } }
public:
    bst_iterator(node* root) { push_left(root); }
    int next() {
        node* n = st.top(); st.pop();
        push_left(n->right);
        return n->val;
    }
    bool has_next() { return !st.empty(); }
};
