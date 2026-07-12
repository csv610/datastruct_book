template <typename V>
class trie {
public:
    void insert(std::string_view key, const V& value) {
        node* n = &root_;
        for (char c : key) {
            size_t idx = c - 'a';  // assume lowercase letters
            if (!n->children[idx]) {
                n->children[idx] = std::make_unique<node>();
            }
            n = n->children[idx].get();
        }
        n->value = value;
        n->is_end = true;
    }

    std::optional<V> find(std::string_view key) const {
        const node* n = &root_;
        for (char c : key) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return std::nullopt;
            n = n->children[idx].get();
        }
        if (n->is_end) return n->value;
        return std::nullopt;
    }

    bool starts_with(std::string_view prefix) const {
        const node* n = &root_;
        for (char c : prefix) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return false;
            n = n->children[idx].get();
        }
        return true;
    }

    std::vector<std::string> keys_with_prefix(std::string_view prefix) const {
        std::vector<std::string> result;
        const node* n = &root_;
        for (char c : prefix) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return result;
            n = n->children[idx].get();
        }
        collect_keys(n, std::string(prefix), result);
        return result;
    }

private:
    static constexpr size_t alphabet_size = 26;

    struct node {
        std::array<std::unique_ptr<node>, alphabet_size> children;
        V value;
        bool is_end = false;
    };

    void collect_keys(const node* n, std::string prefix,
                      std::vector<std::string>& result) const {
        if (n->is_end) result.push_back(prefix);
        for (size_t i = 0; i < alphabet_size; ++i) {
            if (n->children[i]) {
                collect_keys(n->children[i].get(),
                             prefix + char('a' + i), result);
            }
        }
    }

    node root_;
};
