void serialize(node* n, std::ostringstream& os) {
    if (!n) { os << "# "; return; }
    os << n->val << " ";
    serialize(n->left, os);
    serialize(n->right, os);
}
