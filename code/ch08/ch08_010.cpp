template <typename K, typename V>
struct skip_node {
    K key;
    V value;
    std::vector<skip_node*> next;  // next[i] = successor at level i

    skip_node(const K& k, const V& v, int level)
        : key(k), value(v), next(level + 1, nullptr) {}
};
