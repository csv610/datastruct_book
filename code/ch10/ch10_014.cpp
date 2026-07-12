template <typename T>
class pairing_heap {
    struct node {
        T key;
        node *left = nullptr, *next = nullptr, *prev = nullptr;
    };

    node* root_ = nullptr;
    int size_ = 0;

public:
    // Merge two pairing heaps: O(1)
    static pairing_heap merge(pairing_heap&& a, pairing_heap&& b) {
        if (!a.root_) return std::move(b);
        if (!b.root_) return std::move(a);
        link_roots(a.root_, b.root_);
        if (b.root_->key < a.root_->key)
            a.root_ = b.root_;
        a.size_ += b.size_;
        b.root_ = nullptr;
        return std::move(a);
    }

    // Insert: O(1) amortized
    node* insert(const T& key) {
        auto* n = new node{key};
        if (!root_) {
            root_ = n;
        } else {
            link_roots(n, root_);
            if (key < root_->key) root_ = n;
        }
        ++size_;
        return n;
    }

    // Decrease-key: O(log n) amortized
    void decrease_key(node* x, const T& new_key) {
        x->key = new_key;
        if (x == root_) return;
        // Cut x from its parent
        if (x->prev) x->prev->next = x->next;
        if (x->next) x->next->prev = x->prev;
        if (x == x->prev->next)  // was first child
            x->prev->next = x->next;
        x->next = x->prev = nullptr;
        // Merge cut subtree with root
        link_roots(x, root_);
        if (x->key < root_->key) root_ = x;
    }

    // Extract-min: O(log n) amortized
    T extract_min() {
        T result = root_->key;
        // Collect children
        node* first_child = root_->left;
        // Two-pass pairing
        std::vector<node*> trees;
        node* curr = first_child;
        while (curr) {
            node* next = curr->next;
            curr->next = curr->prev = nullptr;
            trees.push_back(curr);
            curr = next;
        }
        // Pair from right to left
        root_ = nullptr;
        for (int i = static_cast<int>(trees.size()) - 1; i >= 0; --i) {
            if (!root_) {
                root_ = trees[i];
            } else {
                link_roots(trees[i], root_);
                if (trees[i]->key < root_->key) root_ = trees[i];
            }
        }
        --size_;
        return result;
    }

    bool empty() const { return size_ == 0; }

private:
    static void link_roots(node* a, node* b) {
        // Make a a child of b (or vice versa)
        a->next = b->left;
        if (b->left) b->left->prev = a;
        b->left = a;
        a->prev = b;
    }
};
