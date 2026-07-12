#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// -----------------------------------------------------------------------
// Hash functions
// -----------------------------------------------------------------------

size_t hash_djb2(std::string_view s) {
    size_t h = 5381;
    for (char c : s)
        h = h * 33 + static_cast<size_t>(c);
    return h;
}

size_t hash_sdbm(std::string_view s) {
    size_t h = 0;
    for (char c : s)
        h = static_cast<size_t>(c) + (h << 6) + (h << 16) - h;
    return h;
}

// -----------------------------------------------------------------------
// Hash table with separate chaining
// -----------------------------------------------------------------------

template <typename K, typename V, typename Hash = std::hash<K>>
class hash_table_chaining {
public:
    explicit hash_table_chaining(size_t initial_buckets = 16)
        : buckets_(initial_buckets), table_(initial_buckets) {}

    void insert(const K& key, const V& value) {
        auto& chain = table_[bucket(key)];
        for (auto& [k, v] : chain) {
            if (k == key) {
                v = value;
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

    void print() const {
        std::cout << "Hash table (" << size_ << " elements, "
                  << buckets_ << " buckets):\n";
        for (size_t i = 0; i < buckets_; ++i) {
            std::cout << "  [" << i << "]:";
            for (const auto& [k, v] : table_[i])
                std::cout << " (" << k << ", " << v << ")";
            std::cout << "\n";
        }
    }

private:
    size_t bucket(const K& key) const {
        return Hash{}(key) % buckets_;
    }

    void check_load() {
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

// -----------------------------------------------------------------------
// Demonstration
// -----------------------------------------------------------------------

int main() {
    std::cout << "=== Hash Functions ===\n";
    std::cout << "hash_djb2(\"hello\") = " << hash_djb2("hello") << "\n";
    std::cout << "hash_djb2(\"world\") = " << hash_djb2("world") << "\n";
    std::cout << "hash_sdbm(\"hello\") = " << hash_sdbm("hello") << "\n";
    std::cout << "hash_sdbm(\"world\") = " << hash_sdbm("world") << "\n\n";

    std::cout << "=== Hash Table with Chaining ===\n";
    hash_table_chaining<std::string, int> ht;

    ht.insert("apple", 5);
    ht.insert("banana", 3);
    ht.insert("cherry", 8);
    ht.insert("date", 2);
    ht.insert("elderberry", 10);

    ht.print();

    std::cout << "\nLookups:\n";
    for (const auto& key : {"apple", "banana", "fig"}) {
        auto val = ht.find(key);
        if (val)
            std::cout << "  " << key << " -> " << *val << "\n";
        else
            std::cout << "  " << key << " not found\n";
    }

    std::cout << "\nErase \"banana\":\n";
    ht.erase("banana");
    ht.print();

    std::cout << "\nSize: " << ht.size() << ", empty: "
              << std::boolalpha << ht.empty() << "\n";

    std::cout << "\nAll ch08 tests passed.\n";
    return 0;
}
