template <typename V>
class tst {
public:
    void insert(std::string_view key, const V& value) {
        root_ = insert(root_, key, 0, value);
    }

    std::optional<V> find(std::string_view key) const {
        auto* n = find(root_.get(), key, 0);
        if (n && n->is_end) return n->value;
        return std::nullopt;
    }

private:
    struct node {
        char c;
        std::unique_ptr<node> left, mid, right;
        V value;
        bool is_end = false;
        node(char ch) : c(ch) {}
    };

    std::unique_ptr<node> insert(std::unique_ptr<node> n,
                                  std::string_view key, size_t i,
                                  const V& value) {
        char c = key[i];
        if (!n) n = std::make_unique<node>(c);
        if (c < n->c) n->left = insert(std::move(n->left), key, i, value);
        else if (c > n->c) n->right = insert(std::move(n->right), key, i, value);
        else if (i + 1 < key.size()) n->mid = insert(std::move(n->mid), key, i+1, value);
        else { n->value = value; n->is_end = true; }
        return n;
    }

    node* find(node* n, std::string_view key, size_t i) const {
        if (!n) return nullptr;
        char c = key[i];
        if (c < n->c) return find(n->left.get(), key, i);
        if (c > n->c) return find(n->right.get(), key, i);
        if (i + 1 < key.size()) return find(n->mid.get(), key, i+1);
        return n;
    }

    std::unique_ptr<node> root_;
};
