#ifndef HASH_TABLE_CHAINING_H
#define HASH_TABLE_CHAINING_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <list>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename K, typename V, typename Hash = std::hash<K>>
    requires std::equality_comparable<K>
class hash_table_chaining {
public:
    using key_type    = K;
    using mapped_type = V;
    using value_type  = std::pair<const K, V>;

    explicit hash_table_chaining(std::size_t initial_buckets = 16)
        : bucket_count_(initial_buckets), size_(0),
          table_(initial_buckets) {}

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
        if (load_factor() > max_load_factor_)
            rehash(bucket_count_ * 2);
    }

    std::optional<V> find(const K& key) const {
        const auto& chain = table_[bucket(key)];
        for (const auto& [k, v] : chain)
            if (k == key) return v;
        return std::nullopt;
    }

    bool contains(const K& key) const {
        return find(key).has_value();
    }

    V& operator[](const K& key) {
        auto& chain = table_[bucket(key)];
        for (auto& [k, v] : chain)
            if (k == key) return v;
        chain.push_back({key, V{}});
        ++size_;
        if (load_factor() > max_load_factor_)
            rehash(bucket_count_ * 2);
        return chain.back().second;
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

    std::size_t size()              const noexcept { return size_; }
    bool        empty()             const noexcept { return size_ == 0; }
    std::size_t bucket_count()      const noexcept { return bucket_count_; }
    double      load_factor()       const noexcept {
        return static_cast<double>(size_) / bucket_count_;
    }

    void set_max_load_factor(double mf) noexcept { max_load_factor_ = mf; }
    double max_load_factor() const noexcept { return max_load_factor_; }

    void clear() noexcept {
        for (auto& chain : table_) chain.clear();
        size_ = 0;
    }

private:
    std::size_t bucket(const K& key) const {
        return Hash{}(key) % bucket_count_;
    }

    void rehash(std::size_t new_bucket_count) {
        std::vector<std::list<value_type>> new_table(new_bucket_count);
        for (auto& chain : table_) {
            for (auto& [k, v] : chain) {
                std::size_t b = Hash{}(k) % new_bucket_count;
                new_table[b].push_back({k, std::move(v)});
            }
        }
        table_        = std::move(new_table);
        bucket_count_ = new_bucket_count;
    }

    std::size_t bucket_count_;
    std::size_t size_;
    double      max_load_factor_ = 1.0;
    std::vector<std::list<value_type>> table_;
};

#endif
