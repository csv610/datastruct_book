template <typename T>
class fibonacci_heap {
    struct node {
        T key;
        node *left, *right, *parent, *child;
        int degree = 0;
        bool marked = false;  // for cascading cut
    };

    node* min_ = nullptr;
    int size_ = 0;

public:
    // Insert: O(1) amortized
    void insert(const T& key) {
        auto* n = new node{key, nullptr, nullptr, nullptr, nullptr, 0, false};
        if (!min_) {
            min_ = n;
            n->left = n->right = n;  // circular list
        } else {
            // Add to root list
            n->left = min_;
            n->right = min_->right;
            min_->right->left = n;
            min_->right = n;
            if (key < min_->key) min_ = n;
        }
        ++size_;
    }

    // Decrease-key: O(1) amortized
    void decrease_key(node* x, const T& new_key) {
        x->key = new_key;
        node* y = x->parent;
        if (y && x->key < y->key) {
            cut(x, y);       // cut x from parent, add to root list
            cascading_cut(y); // possibly cut grandparent, etc.
        }
        if (x->key < min_->key) min_ = x;
    }

    // Extract-min: O(log n) amortized
    T extract_min() {
        T result = min_->key;
        // Add all children of min to root list
        if (min_->child) {
            node* child = min_->child;
            do {
                node* next = child->right;
                child->left = min_;
                child->right = min_->right;
                min_->right->left = child;
                min_->right = child;
                child->parent = nullptr;
                child = next;
            } while (child != min_->child);
        }
        // Remove min from root list
        min_->left->right = min_->right;
        min_->right->left = min_->left;
        if (min_ == min_->right) {
            delete min_;
            min_ = nullptr;
        } else {
            node* old_min = min_;
            min_ = min_->right;  // temporary
            consolidate();
            delete old_min;
        }
        --size_;
        return result;
    }

private:
    void cut(node* x, node* y) {
        // Remove x from child list of y
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->degree == 1) y->child = nullptr;
        --y->degree;
        // Add x to root list
        x->left = min_;
        x->right = min_->right;
        min_->right->left = x;
        min_->right = x;
        x->parent = nullptr;
        x->marked = false;
    }

    void cascading_cut(node* y) {
        node* z = y->parent;
        if (z) {
            if (!y->marked) y->marked = true;
            else { cut(y, z); cascading_cut(z); }
        }
    }

    void consolidate() {
        int max_degree = 4 * static_cast<int>(std::log2(size_));
        std::vector<node*> A(max_degree + 1, nullptr);

        // Collect all roots
        std::vector<node*> roots;
        node* current = min_;
        do {
            roots.push_back(current);
            current = current->right;
        } while (current != min_);

        for (node* w : roots) {
            node* x = w;
            int d = x->degree;
            while (d < max_degree && A[d]) {
                node* y = A[d];
                if (x->key > y->key) std::swap(x, y);
                link(y, x);  // make y a child of x
                A[d] = nullptr;
                ++d;
            }
            A[d] = x;
        }

        // Rebuild root list
        min_ = nullptr;
        for (node* n : A) {
            if (n) {
                if (!min_) {
                    min_ = n;
                    n->left = n->right = n;
                } else {
                    n->left = min_;
                    n->right = min_->right;
                    min_->right->left = n;
                    min_->right = n;
                    if (n->key < min_->key) min_ = n;
                }
            }
        }
    }

    void link(node* y, node* x) {
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x;
        if (x->child) {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        } else {
            x->child = y;
            y->left = y->right = y;
        }
        ++x->degree;
        y->marked = false;
    }
};
