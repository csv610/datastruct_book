class ternary_search_tree {
public:
    void insert(std::string_view key) {
        root_ = insert(root_, key, 0);
    }

    bool search(std::string_view key) const {
        return search(root_.get(), key, 0);
    }

    void autocomplete(std::string_view prefix,
                      std::vector<std::string>& results) const {
        const node* n = root_.get();
        for (char c : prefix) {
            if (!n) return;
            if (c < n->split_char)      n = n->low.get();
            else if (c > n->split_char) n = n->high.get();
            else                         n = n->equal.get();
        }
        collect(n, std::string(prefix), results);
    }

    void range_search(std::string_view lo, std::string_view hi,
                      std::vector<std::string>& results) const {
        range_collect(root_.get(), "", lo, hi, results);
    }

private:
    struct node {
        char split_char;
        std::unique_ptr<node> low;
        std::unique_ptr<node> equal;
        std::unique_ptr<node> high;
        bool is_end = false;
        explicit node(char c) : split_char(c) {}
    };

    std::unique_ptr<node> root_;

    std::unique_ptr<node> insert(std::unique_ptr<node> n,
                                  std::string_view key, size_t i) {
        if (i >= key.size()) return n;
        char c = key[i];
        if (!n) n = std::make_unique<node>(c);
        if (c < n->split_char)
            n->low = insert(std::move(n->low), key, i);
        else if (c > n->split_char)
            n->high = insert(std::move(n->high), key, i);
        else if (i + 1 < key.size())
            n->equal = insert(std::move(n->equal), key, i + 1);
        else
            n->is_end = true;
        return n;
    }

    bool search(const node* n, std::string_view key, size_t i) const {
        if (!n) return false;
        if (i >= key.size()) return n->is_end;
        char c = key[i];
        if (c < n->split_char)
            return search(n->low.get(), key, i);
        if (c > n->split_char)
            return search(n->high.get(), key, i);
        if (i + 1 < key.size())
            return search(n->equal.get(), key, i + 1);
        return n->is_end;
    }

    void collect(const node* n, std::string prefix,
                 std::vector<std::string>& results) const {
        if (!n) return;
        collect(n->low.get(), prefix, results);
        if (n->is_end) results.push_back(prefix);
        std::string next = prefix + n->split_char;
        collect(n->equal.get(), next, results);
        collect(n->high.get(), prefix, results);
    }

    void range_collect(const node* n, std::string prefix,
                       std::string_view lo, std::string_view hi,
                       std::vector<std::string>& results) const {
        if (!n) return;
        range_collect(n->low.get(), prefix, lo, hi, results);
        std::string next = prefix + n->split_char;
        if (n->is_end && next >= lo && next <= hi)
            results.push_back(next);
        range_collect(n->equal.get(), next, lo, hi, results);
        range_collect(n->high.get(), prefix, lo, hi, results);
    }
};
