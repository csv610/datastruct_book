class rope {
public:
    struct node {
        size_t weight;                    // characters in left subtree
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;
        std::string leaf_data;            // non-empty only for leaves

        explicit node(std::string s)
            : weight(s.size()), leaf_data(std::move(s)) {}

        node(std::unique_ptr<node> l, std::unique_ptr<node> r)
            : weight(total_len(l.get())),
              left(std::move(l)), right(std::move(r)) {}

        static size_t total_len(const node* n) {
            if (!n) return 0;
            if (!n->left && !n->right) return n->leaf_data.size();
            return n->weight + total_len(n->right.get());
        }
    };

    rope() = default;
    explicit rope(std::string s)
        : root_(std::make_unique<node>(std::move(s))) {}

    size_t size() const { return node::total_len(root_.get()); }
    bool empty() const { return !root_; }

    char index(size_t i) const {
        if (i >= size()) throw std::out_of_range("rope index");
        return index(root_.get(), i);
    }

    void concatenate(rope&& other) {
        root_ = std::make_unique<node>(
            std::move(root_), std::move(other.root_));
    }

    std::pair<rope, rope> split(size_t pos) {
        if (pos > size()) throw std::out_of_range("rope split");
        auto [l, r] = split(std::move(root_), pos);
        return {rope(std::move(l)), rope(std::move(r))};
    }

    std::string to_string() const {
        std::string result;
        result.reserve(size());
        flatten(root_.get(), result);
        return result;
    }

private:
    std::unique_ptr<node> root_;
    explicit rope(std::unique_ptr<node> n) : root_(std::move(n)) {}

    static char index(const node* n, size_t i) {
        if (!n->left && !n->right) return n->leaf_data[i];
        if (i < n->weight) return index(n->left.get(), i);
        return index(n->right.get(), i - n->weight);
    }

    static void flatten(const node* n, std::string& out) {
        if (!n) return;
        if (!n->left && !n->right) { out += n->leaf_data; return; }
        flatten(n->left.get(), out);
        flatten(n->right.get(), out);
    }

    static std::pair<std::unique_ptr<node>, std::unique_ptr<node>>
    split(std::unique_ptr<node> n, size_t pos) {
        if (!n) return {nullptr, nullptr};
        if (!n->left && !n->right) {
            std::string l_str = n->leaf_data.substr(0, pos);
            std::string r_str = n->leaf_data.substr(pos);
            return {
                l_str.empty() ? nullptr
                    : std::make_unique<node>(std::move(l_str)),
                r_str.empty() ? nullptr
                    : std::make_unique<node>(std::move(r_str))
            };
        }
        if (pos <= n->weight) {
            auto [ll, lr] = split(std::move(n->left), pos);
            auto new_right = std::make_unique<node>(
                std::move(lr), std::move(n->right));
            return {std::move(ll), std::move(new_right)};
        } else {
            auto [rl, rr] = split(std::move(n->right), pos - n->weight);
            auto new_left = std::make_unique<node>(
                std::move(n->left), std::move(rl));
            return {std::move(new_left), std::move(rr)};
        }
    }
};
