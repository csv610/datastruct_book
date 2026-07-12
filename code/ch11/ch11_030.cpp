class bstar_tree {
    static constexpr int MAX_KEYS = 6;
    static constexpr int MIN_KEYS = 4;  // ceil(2*6/3) - 1

    struct node {
        int n;                       // current key count
        bool is_leaf;
        int keys[MAX_KEYS];
        node* children[MAX_KEYS + 1];
        node* parent;

        node(bool leaf)
            : n(0), is_leaf(leaf), parent(nullptr) {
            for (int i = 0; i <= MAX_KEYS; ++i)
                children[i] = nullptr;
        }
    };

    node* root;

    // Try to redistribute keys between 'src' and
    // 'sibling' through their common parent.
    // Returns true on success.
    bool redistribute(node* src, node* sibling,
                      node* parent) {
        int total = src->n + sibling->n;
        if (total > 2 * MAX_KEYS)   // would still overflow
            return false;

        int idx = 0;
        while (idx <= parent->n &&
               parent->children[idx] != src)
            ++idx;

        int target = total / 2;
        int move;

        if (idx > 0) {
            // src is on the right; move keys leftward
            move = src->n - target;
            if (move <= 0) return false;

            for (int i = sibling->n; i >= 1; --i) {
                sibling->keys[i] = sibling->keys[i - 1];
                sibling->children[i + 1] =
                    sibling->children[i];
            }
            sibling->children[1] =
                sibling->children[0];
            sibling->keys[0] = parent->keys[idx - 1];
            sibling->children[0] =
                src->children[0];

            for (int i = 0; i < move - 1; ++i) {
                sibling->keys[i + 1] = src->keys[i];
                sibling->children[i + 2] =
                    src->children[i + 1];
            }
            sibling->children[move + 1] =
                src->children[move];

            parent->keys[idx - 1] =
                src->keys[move - 1];

            int j = 0;
            for (int i = move; i < src->n; ++i)
                src->keys[j++] = src->keys[i];
            j = 0;
            for (int i = move; i <= src->n; ++i)
                src->children[j++] = src->children[i];
        } else {
            // src is on the left; move keys rightward
            move = src->n - target;
            if (move <= 0) return false;

            for (int i = 0; i < move; ++i) {
                sibling->children[sibling->n + 2 - i] =
                    sibling->children[sibling->n + 1 - i];
                sibling->keys[sibling->n + 1 - i] =
                    sibling->keys[sibling->n - i];
            }
            sibling->children[0] =
                src->children[src->n];
            sibling->keys[0] = parent->keys[0];
            parent->keys[0] =
                src->keys[src->n - move];

            sibling->n += move + 1;
            src->n -= move;
        }
        return true;
    }

    // Standard split fallback
    void split(node* full, int child_idx) {
        node* new_node = new node(full->is_leaf);
        int mid = full->n / 2;

        new_node->n = full->n - mid - 1;
        for (int i = 0; i < new_node->n; ++i) {
            new_node->keys[i] =
                full->keys[mid + 1 + i];
            new_node->children[i] =
                full->children[mid + 1 + i];
        }
        new_node->children[new_node->n] =
            full->children[full->n];
        full->n = mid;

        int sep = full->keys[mid];
        node* p = full->parent;

        if (!p) {
            root = new node(false);
            root->keys[0] = sep;
            root->children[0] = full;
            root->children[1] = new_node;
            root->n = 1;
            full->parent = root;
            new_node->parent = root;
            return;
        }

        for (int i = p->n; i > child_idx; --i) {
            p->keys[i] = p->keys[i - 1];
            p->children[i + 1] = p->children[i];
        }
        p->keys[child_idx] = sep;
        p->children[child_idx + 1] = new_node;
        new_node->parent = p;
        ++p->n;

        if (p->n > MAX_KEYS)
            split(p, child_idx);
    }

    void insert_nonfull(node* x, int key) {
        int i = x->n - 1;
        if (x->is_leaf) {
            while (i >= 0 && key < x->keys[i]) {
                x->keys[i + 1] = x->keys[i];
                --i;
            }
            x->keys[i + 1] = key;
            ++x->n;
            return;
        }
        while (i >= 0 && key < x->keys[i])
            --i;
        ++i;
        if (x->children[i]->n == MAX_KEYS) {
            // Try redistribution first
            node* sib = nullptr;
            if (i > 0)
                sib = x->children[i - 1];
            else if (i < x->n)
                sib = x->children[i + 1];

            if (sib && sib->n < MAX_KEYS &&
                redistribute(x->children[i],
                             sib, x))
                ;   // redistribution succeeded
            else
                split(x->children[i], i);

            if (key > x->keys[i])
                ++i;
        }
        insert_nonfull(x->children[i], key);
    }

public:
    bstar_tree() : root(new node(true)) {}

    void insert(int key) {
        node* r = root;
        if (r->n == MAX_KEYS) {
            split(r, 0);
            insert_nonfull(root, key);
        } else {
            insert_nonfull(r, key);
        }
    }
};
