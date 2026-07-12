template <typename K, typename V>
void skip_list<K, V>::insert(const K& key, const V& value) {
    std::vector<skip_node<K, V>*> update(level_ + 1, nullptr);
    auto* cur = head_;

    for (int i = level_; i >= 0; --i) {
        while (cur->next[i] && cur->next[i]->key < key)
            cur = cur->next[i];
        update[i] = cur;
    }

    cur = cur->next[0];
    if (cur && cur->key == key) {
        cur->value = value;  // key already exists
        return;
    }

    int new_level = random_level();
    if (new_level > level_) {
        head_->next.resize(new_level + 1, nullptr);
        update.resize(new_level + 1, head_);
        level_ = new_level;
    }

    auto* node = new skip_node<K, V>(key, value, new_level);
    for (int i = 0; i <= new_level; ++i) {
        node->next[i] = update[i]->next[i];
        update[i]->next[i] = node;
    }
}
