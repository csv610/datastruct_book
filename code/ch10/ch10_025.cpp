template <typename T, typename Compare = std::less<T>>
class skew_heap {
    struct node {
        T data;
        node *left = nullptr, *right = nullptr;
        explicit node(const T& value) : data(value) {}
    };

    node* root_ = nullptr;
    int size_ = 0;
    Compare comp_;

    // Merge two skew heaps: O(log n) amortized
    static node* merge(node* a, node* b, const Compare& comp) {
        if (!a) return b;
        if (!b) return a;

        // Ensure a has the smaller root
        if (comp(b->data, a->data)) std::swap(a, b);

        // Merge along the right spine of a
        node *curr = a, *prev = nullptr;
        while (curr) {
            if (comp(b->data, curr->data)) {
                // b should be above curr on the right spine
                // Swap spines: b takes curr's left subtree
                // as its right, and curr becomes b's right
                node* temp = curr->right;
                curr->right = b;
                b = curr;
                curr = temp;
            }
            prev = curr;
            if (curr) curr = curr->right;
        }
        // Swap left and right children on the merge path
        curr = a;
        while (curr && curr != b) {
            std::swap(curr->left, curr->right);
            curr = curr->right;
        }
        return b;
    }

public:
    skew_heap() = default;

    void push(const T& value) {
        auto* n = new node(value);
        root_ = merge(root_, n, comp_);
        ++size_;
    }

    void push(T&& value) {
        auto* n = new node(std::move(value));
        root_ = merge(root_, n, comp_);
        ++size_;
    }

    // Merge two skew heaps: O(log n) amortized
    static skew_heap merge(skew_heap&& a, skew_heap&& b) {
        skew_heap result;
        result.root_ = merge(a.root_, b.root_, a.comp_);
        result.size_ = a.size_ + b.size_;
        a.root_ = nullptr;
        b.root_ = nullptr;
        return result;
    }

    T pop() {
        if (empty()) throw std::underflow_error("heap is empty");
        T result = std::move(root_->data);
        node* old_root = root_;
        root_ = merge(root_->left, root_->right, comp_);
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

    ~skew_heap() {
        while (!empty()) pop();
    }
};
