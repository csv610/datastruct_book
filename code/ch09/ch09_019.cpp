ThreadedNode* inorder_successor(ThreadedNode* node) {
    if (node->right_thread)
        return node->right;  // thread points to successor

    // Go to leftmost node in right subtree
    ThreadedNode* curr = node->right;
    while (!curr->left_thread)
        curr = curr->left;
    return curr;
}

// In-order traversal using threads -- no stack needed
void threaded_inorder(ThreadedNode* root) {
    if (!root) return;

    // Start at leftmost node
    ThreadedNode* curr = root;
    while (!curr->left_thread)
        curr = curr->left;

    while (curr) {
        visit(curr);
        curr = inorder_successor(curr);
    }
}
