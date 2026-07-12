template <typename K, typename V>
class scapegoat_tree {
    struct node {
        K key;
        V value;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;
        node(const K& k, const V& v) : key(k), value(v) {}
    };

    using node_ptr = std::unique_ptr<node>;
    node_ptr root_;
    int n_ = 0;
    static constexpr double ALPHA = 2.0 / 3.0;

    static int size(const node* n) {
        if (!n) return 0;
        return 1 + size(n->left.get()) + size(n->right.get());
    }

    static node* find_scapegoat(node* n, const K& key) {
        if (!n) return nullptr;
        int left_sz = size(n->left.get());
        int right_sz = size(n->right.get());
        int nsz = 1 + left_sz + right_sz;
        if (left_sz > ALPHA * nsz || right_sz > ALPHA * nsz)
            return n;
        if (key < n->key) return find_scapegoat(n->left.get(), key);
        if (key > n->key) return find_scapegoat(n->right.get(), key);
        return nullptr;
    }

    static void flatten(node* n, std::vector<node*>& out) {
        if (!n) return;
        flatten(n->left.get(), out);
        out.push_back(n);
        flatten(n->right.get(), out);
    }

    static node_ptr build_balanced(std::vector<node*>& nodes,
                                    int lo, int hi) {
        if (lo > hi) return nullptr;
        int mid = (lo + hi) / 2;
        auto n = node_ptr(nodes[mid]);
        n->left = build_balanced(nodes, lo, mid - 1);
        n->right = build_balanced(nodes, mid + 1, hi);
        return n;
    }

    node_ptr rebuild(node_ptr root, const K& key) {
        std::vector<node*> nodes;
        flatten(root.get(), nodes);
        for (auto* nd : nodes) { nd->left.reset(); nd->right.reset(); }
        return build_balanced(nodes, 0, nodes.size() - 1);
    }

    node_ptr insert(node_ptr n, const K& key, const V& value,
                    node*& scapegoat) {
        if (!n) { ++n_; return std::make_unique<node>(key, value); }
        if (key < n->key)
            n->left = insert(std::move(n->left), key, value, scapegoat);
        else if (key > n->key)
            n->right = insert(std::move(n->right), key, value, scapegoat);
        else { n->value = value; return n; }
        if (!scapegoat) {
            int left_sz = size(n->left.get());
            int right_sz = size(n->right.get());
            int nsz = 1 + left_sz + right_sz;
            if (left_sz > ALPHA * nsz || right_sz > ALPHA * nsz)
                scapegoat = n.get();
        }
        return n;
    }

public:
    void insert(const K& key, const V& value) {
        node* scapegoat = nullptr;
        root_ = insert(std::move(root_), key, value, scapegoat);
        if (scapegoat) root_ = rebuild(std::move(root_), scapegoat->key);
    }

    std::optional<V> find(const K& key) const {
        auto* n = root_.get();
        while (n) {
            if (key < n->key) n = n->left.get();
            else if (key > n->key) n = n->right.get();
            else return n->value;
        }
        return std::nullopt;
    }
};
