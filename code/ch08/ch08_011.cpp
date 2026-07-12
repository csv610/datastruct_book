template <typename K, typename V>
V* skip_list<K, V>::find(const K& key) {
    auto* cur = head_;
    for (int i = level_; i >= 0; --i) {
        while (cur->next[i] && cur->next[i]->key < key)
            cur = cur->next[i];
    }
    cur = cur->next[0];
    if (cur && cur->key == key) return &cur->value;
    return nullptr;
}
