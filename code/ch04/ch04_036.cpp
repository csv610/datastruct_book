template <typename K, typename V>
class lru_cache {
    struct entry { K key; V value; };
    std::list<entry> order_;  // front = most recent
    std::unordered_map<K, typename std::list<entry>::iterator> map_;
    size_t capacity_;
public:
    lru_cache(size_t cap) : capacity_(cap) {}

    std::optional<V> get(const K& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return std::nullopt;
        order_.splice(order_.begin(), order_, it->second);
        return it->second->value;
    }

    void put(const K& key, const V& value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->value = value;
            order_.splice(order_.begin(), order_, it->second);
            return;
        }
        if (order_.size() == capacity_) {
            map_.erase(order_.back().key);
            order_.pop_back();
        }
        order_.push_front({key, value});
        map_[key] = order_.begin();
    }
};
