template <typename K, typename V, typename Hash = std::hash<K>>
class hash_table_open {
public:
    explicit hash_table_open(size_t initial_capacity = 16)
        : capacity_(initial_capacity), size_(0),
          states_(initial_capacity, State::empty),
          keys_(initial_capacity), values_(initial_capacity) {}

    bool insert(const K& key, const V& value) {
        if (size_ >= capacity_ * 0.7) rehash();
        size_t i = 0;
        size_t pos;
        do {
            pos = probe(key, i);
            if (states_[pos] == State::empty || states_[pos] == State::deleted) {
                keys_[pos] = key;
                values_[pos] = value;
                states_[pos] = State::occupied;
                ++size_;
                return true;
            }
            if (states_[pos] == State::occupied && keys_[pos] == key) {
                values_[pos] = value;  // update
                return false;
            }
            ++i;
        } while (i < capacity_);
        rehash();
        return insert(key, value);
    }

    std::optional<V> find(const K& key) const {
        size_t i = 0;
        size_t pos;
        do {
            pos = probe(key, i);
            if (states_[pos] == State::empty) return std::nullopt;
            if (states_[pos] == State::occupied && keys_[pos] == key) {
                return values_[pos];
            }
            ++i;
        } while (i < capacity_);
        return std::nullopt;
    }

    void erase(const K& key) {
        size_t i = 0;
        size_t pos;
        do {
            pos = probe(key, i);
            if (states_[pos] == State::empty) return;
            if (states_[pos] == State::occupied && keys_[pos] == key) {
                states_[pos] = State::deleted;
                --size_;
                return;
            }
            ++i;
        } while (i < capacity_);
    }

private:
    enum class State { empty, occupied, deleted };

    size_t probe(const K& key, size_t i) const {
        size_t h1 = Hash{}(key) % capacity_;
        size_t h2 = 1 + (Hash{}(key) % (capacity_ - 1));  // second hash
        return (h1 + i * h2) % capacity_;
    }

    void rehash() {
        size_t old_cap = capacity_;
        capacity_ *= 2;
        std::vector<State> old_states = std::move(states_);
        std::vector<K> old_keys = std::move(keys_);
        std::vector<V> old_values = std::move(values_);

        states_ = std::vector<State>(capacity_, State::empty);
        keys_.resize(capacity_);
        values_.resize(capacity_);
        size_ = 0;

        for (size_t i = 0; i < old_cap; ++i) {
            if (old_states[i] == State::occupied) {
                insert(old_keys[i], old_values[i]);
            }
        }
    }

    size_t capacity_, size_;
    std::vector<State> states_;
    std::vector<K> keys_;
    std::vector<V> values_;
};
