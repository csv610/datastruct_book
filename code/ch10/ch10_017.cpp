template <typename T, typename Compare = std::less<T>>
class binomial_heap {
    struct node {
        T key;
        int degree = 0;
        node *parent = nullptr, *child = nullptr, *sibling = nullptr;
    };

    node* root_ = nullptr;
    int size_ = 0;
    Compare comp_;

    // Link tree y under tree x (same degree): O(1)
    static void merge_trees(node* y, node* x) {
        y->parent = x;
        y->sibling = x->child;
        x->child = y;
        ++x->degree;
    }

public:
    binomial_heap() = default;

    // Merge two binomial heaps: O(log n)
    static binomial_heap merge(binomial_heap&& a, binomial_heap&& b) {
        binomial_heap result;
        result.root_ = merge_root_lists(a.root_, b.root_);
        if (!result.root_) return result;
        // Link roots of equal degree
        node *prev = nullptr, *x = result.root_, *next = x->sibling;
        while (next) {
            if (x->degree != next->degree ||
                (next->sibling && next->sibling->degree == x->degree)) {
                prev = x;
                x = next;
            } else {
                if (comp_(next->key, x->key)) {
                    x->sibling = next->sibling;
                    merge_trees(x, next);
                    x = next;
                } else {
                    next->sibling = x->sibling;
                    merge_trees(next, x);
                }
            }
            next = x->sibling;
        }
        result.size_ = a.size_ + b.size_;
        a.root_ = nullptr;
        b.root_ = nullptr;
        return result;
    }

    // Insert: O(log n)
    void insert(const T& key) {
        binomial_heap temp;
        temp.root_ = new node{key};
        temp.size_ = 1;
        *this = merge(std::move(*this), std::move(temp));
    }

    // Extract-min: O(log n)
    T extract_min() {
        if (!root_) throw std::underflow_error("heap is empty");
        // Find the root with minimum key
        node *best_prev = nullptr, *best = root_;
        node *prev = nullptr, *curr = root_;
        while (curr) {
            if (comp_(curr->key, best->key)) {
                best = curr;
                best_prev = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }
        // Remove best from root list
        if (best_prev)
            best_prev->sibling = best->sibling;
        else
            root_ = best->sibling;
        // Reverse the child list to form a new heap
        binomial_heap child_heap;
        node* rev = nullptr;
        node* c = best->child;
        while (c) {
            node* next = c->sibling;
            c->sibling = rev;
            c->parent = nullptr;
            rev = c;
            c = next;
        }
        child_heap.root_ = rev;
        child_heap.size_ = (1 << best->degree) - 1;
        T result = best->key;
        delete best;
        --size_;
        *this = merge(std::move(*this), std::move(child_heap));
        return result;
    }

    // Decrease-key: O(log n)
    void decrease_key(node* x, const T& new_key) {
        x->key = new_key;
        node* y = x;
        while (y->parent && comp_(y->key, y->parent->key)) {
            std::swap(y->key, y->parent->key);
            y = y->parent;
        }
    }

    bool empty() const noexcept { return size_ == 0; }
    int size() const noexcept { return size_; }

private:
    static node* merge_root_lists(node* a, node* b) {
        if (!a) return b;
        if (!b) return a;
        node* result = nullptr;
        node** tail = &result;
        while (a && b) {
            if (a->degree <= b->degree) {
                *tail = a;
                a = a->sibling;
            } else {
                *tail = b;
                b = b->sibling;
            }
            tail = &((*tail)->sibling);
        }
        *tail = a ? a : b;
        return result;
    }
};
