template <typename K, typename V>
class bplus_tree {
    static constexpr int MAX_KEYS = 3;

    struct node {
        std::vector<K> keys;
        std::vector<V> values;           // only meaningful for leaves
        std::vector<std::unique_ptr<node>> children;
        node* next = nullptr;            // leaf linked list
        bool is_leaf;

        node(bool leaf) : is_leaf(leaf) {}
    };

    std::unique_ptr<node> root_;

    std::optional<V> search(const node* n, const K& key) const {
        if (!n) return std::nullopt;
        if (n->is_leaf) {
            for (size_t i = 0; i < n->keys.size(); ++i)
                if (n->keys[i] == key) return n->values[i];
            return std::nullopt;
        }
        size_t i = 0;
        while (i < n->keys.size() && key >= n->keys[i]) ++i;
        return search(n->children[i].get(), key);
    }

    void split_child(node* parent, int child_index) {
        auto& child = parent->children[child_index];
        auto right = std::make_unique<node>(child->is_leaf);
        size_t mid = child->keys.size() / 2;

        if (child->is_leaf) {
            right->keys.assign(child->keys.begin() + mid, child->keys.end());
            right->values.assign(child->values.begin() + mid, child->values.end());
            right->next = child->next;
            child->next = right.get();
            child->keys.resize(mid);
            child->values.resize(mid);
            parent->keys.insert(parent->keys.begin() + child_index,
                                right->keys[0]);
        } else {
            right->keys.assign(child->keys.begin() + mid + 1, child->keys.end());
            child->keys.resize(mid);
            K up_key = child->keys[mid];
            child->keys.pop_back();
            for (size_t i = mid + 1; i < child->children.size(); ++i)
                right->children.push_back(std::move(child->children[i]));
            child->children.resize(mid + 1);
            parent->keys.insert(parent->keys.begin() + child_index, up_key);
        }
        parent->children.insert(parent->children.begin() + child_index + 1,
                                std::move(right));
    }

    void insert_non_full(node* n, const K& key, const V& value) {
        if (n->is_leaf) {
            auto it = std::lower_bound(n->keys.begin(), n->keys.end(), key);
            size_t pos = it - n->keys.begin();
            n->keys.insert(it, key);
            n->values.insert(n->values.begin() + pos, value);
            return;
        }
        size_t i = 0;
        while (i < n->keys.size() && key >= n->keys[i]) ++i;
        if (n->children[i]->keys.size() == 2 * MAX_KEYS) {
            split_child(n, i);
            if (key >= n->keys[i]) ++i;
        }
        insert_non_full(n->children[i].get(), key, value);
    }

public:
    void insert(const K& key, const V& value) {
        if (!root_) {
            root_ = std::make_unique<node>(true);
            root_->keys.push_back(key);
            root_->values.push_back(value);
            return;
        }
        if (root_->keys.size() == 2 * MAX_KEYS) {
            auto new_root = std::make_unique<node>(false);
            new_root->children.push_back(std::move(root_));
            split_child(new_root.get(), 0);
            root_ = std::move(new_root);
        }
        insert_non_full(root_.get(), key, value);
    }

    std::optional<V> find(const K& key) const {
        return search(root_.get(), key);
    }

    std::vector<std::pair<K, V>> range_query(const K& lo, const K& hi) const {
        std::vector<std::pair<K, V>> result;
        const node* n = root_.get();
        while (n && !n->is_leaf) n = n->children[0].get();
        while (n) {
            for (size_t i = 0; i < n->keys.size(); ++i) {
                if (n->keys[i] > hi) return result;
                if (n->keys[i] >= lo)
                    result.emplace_back(n->keys[i], n->values[i]);
            }
            n = n->next;
        }
        return result;
    }
};
