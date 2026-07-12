template <typename T>
class binary_tree {
public:
    using node_ptr = std::unique_ptr<binary_node<T>>;

    binary_tree() = default;

    // Construct from root
    explicit binary_tree(node_ptr root) : root_(std::move(root)) {}

    bool empty() const noexcept { return root_ == nullptr; }

    // Preorder traversal
    template <typename Visitor>
    void preorder(Visitor&& visit) const {
        preorder(root_.get(), visit);
    }

    // Inorder traversal
    template <typename Visitor>
    void inorder(Visitor&& visit) const {
        inorder(root_.get(), visit);
    }

    // Postorder traversal
    template <typename Visitor>
    void postorder(Visitor&& visit) const {
        postorder(root_.get(), visit);
    }

    // Level-order traversal (breadth-first)
    template <typename Visitor>
    void level_order(Visitor&& visit) const {
        if (!root_) return;
        std::queue<binary_node<T>*> q;
        q.push(root_.get());
        while (!q.empty()) {
            auto* node = q.front(); q.pop();
            visit(node->data);
            if (node->left) q.push(node->left.get());
            if (node->right) q.push(node->right.get());
        }
    }

    size_t height() const { return height(root_.get()); }
    size_t size() const { return size(root_.get()); }

private:
    static void preorder(binary_node<T>* node, auto& visit) {
        if (!node) return;
        visit(node->data);
        preorder(node->left.get(), visit);
        preorder(node->right.get(), visit);
    }

    static void inorder(binary_node<T>* node, auto& visit) {
        if (!node) return;
        inorder(node->left.get(), visit);
        visit(node->data);
        inorder(node->right.get(), visit);
    }

    static void postorder(binary_node<T>* node, auto& visit) {
        if (!node) return;
        postorder(node->left.get(), visit);
        postorder(node->right.get(), visit);
        visit(node->data);
    }

    static size_t height(binary_node<T>* node) {
        if (!node) return 0;
        return 1 + std::max(height(node->left.get()), height(node->right.get()));
    }

    static size_t size(binary_node<T>* node) {
        if (!node) return 0;
        return 1 + size(node->left.get()) + size(node->right.get());
    }

    node_ptr root_;
};
