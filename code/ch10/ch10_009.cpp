template <typename T>
struct leftist_node {
    T data;
    std::unique_ptr<leftist_node> left;
    std::unique_ptr<leftist_node> right;
    int npl = 0;  // null path length

    explicit leftist_node(const T& value) : data(value) {}
};

template <typename T>
std::unique_ptr<leftist_node<T>> merge(
    std::unique_ptr<leftist_node<T>> a,
    std::unique_ptr<leftist_node<T>> b) {
    if (!a) return b;
    if (!b) return a;

    // Ensure a has the smaller root
    if (a->data > b->data) std::swap(a, b);

    // Recursively merge with right child
    a->right = merge(std::move(a->right), std::move(b));

    // Maintain leftist property
    if (!a->left || (a->left->npl < a->right->npl)) {
        std::swap(a->left, a->right);
    }

    // Update null path length
    a->npl = (a->right ? a->right->npl : 0) + 1;

    return a;
}
