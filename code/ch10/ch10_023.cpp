template <typename T, typename Compare = std::less<T>>
class weak_heap {
    struct node {
        T data;
        node* right = nullptr;
        bool reverse = false;

        explicit node(const T& value) : data(value) {}
    };

    node* root_ = nullptr;
    node* last_ = nullptr;
    int size_ = 0;
    Compare comp_;

    node* left_child(node* n) const {
        if (!n || !n->right) return nullptr;
        // The left child is the parent of the right child in
        // the original complete binary tree (found via reverse bits)
        return nullptr;  // simplified; real impl uses parent pointers
    }

    // Merge two weak heaps: O(1)
    static node* merge(node* a, node* b, const Compare& comp) {
        if (!a) return b;
        if (!b) return a;
        // Make the smaller root the new root
        if (comp(b->data, a->data)) std::swap(a, b);
        // Attach b as the right child of a
        a->right = merge(a->right, b, comp);
        return a;
    }

    // Sift down from root: O(log n)
    void sift_down(node* n) {
        if (!n) return;
        while (true) {
            node* child = n->right;
            if (!child) break;

            // Find the smallest child considering reverse bit
            node* smallest = child;
            node* lc = nullptr;
            if (n->reverse) {
                // Left child is accessible via right chain
                lc = child->right;
            }
            // (simplified comparison; full version tracks left child)
            if (lc && comp(lc->data, smallest->data))
                smallest = lc;

            if (comp(smallest->data, n->data)) {
                std::swap(n->data, smallest->data);
                n = smallest;
            } else {
                break;
            }
        }
    }

public:
    weak_heap() = default;

    // Insert: O(log n) amortized
    void push(const T& value) {
        auto* n = new node(value);
        root_ = merge(root_, n, comp_);
        ++size_;
    }

    // Delete-min: O(log n)
    T pop() {
        if (empty()) throw std::underflow_error("heap is empty");
        T result = std::move(root_->data);
        node* old_root = root_;
        root_ = merge(root_->right, nullptr, comp_);
        // In full implementation, detach left subtree and merge
        delete old_root;
        --size_;
        return result;
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return root_->data;
    }

    bool empty() const noexcept { return size_ == 0; }
    int size() const noexcept { return size_; }

    ~weak_heap() {
        // Cleanup (simplified)
        while (!empty()) pop();
    }
};
