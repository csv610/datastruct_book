struct interval {
    int low;
    int high;
};

struct it_node {
    interval ival;
    int max_high;
    it_node* left  = nullptr;
    it_node* right = nullptr;

    explicit it_node(interval i)
        : ival(i), max_high(i.high) {}
};

class interval_tree {
public:
    ~interval_tree() { destroy(root_); }

    void insert(interval i) { root_ = insert(root_, i); }

    std::vector<interval> search(interval q) const {
        std::vector<interval> result;
        search(root_, q, result);
        return result;
    }

private:
    it_node* root_ = nullptr;

    it_node* insert(it_node* n, interval i) {
        if (!n) return new it_node(i);
        if (i.low < n->ival.low)
            n->left = insert(n->left, i);
        else
            n->right = insert(n->right, i);
        n->max_high = std::max(n->max_high, i.high);
        return n;
    }

    void search(it_node* n, const interval& q,
                std::vector<interval>& result) const {
        if (!n) return;

        // Prune left: if q.low > max_high, nothing overlaps
        if (q.low <= n->max_high) {
            search(n->left, q, result);
        }

        // Check current node for overlap
        if (n->ival.low <= q.high && q.low <= n->ival.high) {
            result.push_back(n->ival);
        }

        // Prune right: if q.high <= node.low, nothing overlaps
        if (q.high > n->ival.low) {
            search(n->right, q, result);
        }
    }

    void destroy(it_node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
};
