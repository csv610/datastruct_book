std::unique_ptr<binary_node<char>> build_tree(
    std::span<const char> inorder,
    std::span<const char> preorder) {
    if (inorder.empty()) return nullptr;

    char root_val = preorder[0];
    auto root = std::make_unique<binary_node<char>>(root_val);

    // Find root in inorder
    size_t split = 0;
    while (split < inorder.size() && inorder[split] != root_val) ++split;

    root->left = build_tree(
        inorder.subspan(0, split),
        preorder.subspan(1, split));
    root->right = build_tree(
        inorder.subspan(split + 1),
        preorder.subspan(split + 1));

    return root;
}
