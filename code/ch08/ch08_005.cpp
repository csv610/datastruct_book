template <typename K, typename V, typename Hash = std::hash<K>>
class hash_table_chaining {
public:
    explicit hash_table_chaining(size_t initial_buckets = 16)
        : buckets_(initial_buckets), table_(initial_buckets) {}

    void insert(const K& key, const V& value) {
        auto& chain = table_[bucket(key)];
        for (auto& [k, v] : chain) {
            if (k == key) {
                v = value;  // update existing
                return;
            }
        }
        chain.push_back({key, value});
        ++size_;
        check_load();
    }

    std::optional<V> find(const K& key) const {
        const auto& chain = table_[bucket(key)];
        for (const auto& [k, v] : chain) {
            if (k == key) return v;
        }
        return std::nullopt;
    }

    void erase(const K& key) {
        auto& chain = table_[bucket(key)];
        auto it = std::find_if(chain.begin(), chain.end(),
            [&](const auto& p) { return p.first == key; });
        if (it != chain.end()) {
            chain.erase(it);
            --size_;
        }
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

private:
    size_t bucket(const K& key) const {
        return Hash{}(key) % buckets_;
    }

    void check_load() {
        // Rehash when load factor exceeds 1.0
        if (size_ > buckets_) {
            buckets_ *= 2;
            std::vector<std::list<std::pair<K, V>>> new_table(buckets_);
            for (const auto& chain : table_) {
                for (const auto& pair : chain) {
                    size_t b = Hash{}(pair.first) % buckets_;
                    new_table[b].push_back(pair);
                }
            }
            table_ = std::move(new_table);
        }
    }

    size_t buckets_;
    size_t size_ = 0;
    std::vector<std::list<std::pair<K, V>>> table_;
};
