#ifndef HASH_TABLE_CHAINING_H
#define HASH_TABLE_CHAINING_H

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename K, typename V, typename Hash = std::hash<K>>
    requires std::equality_comparable<K>
class hash_table_chaining {
public:
    using key_type    = K;
    using mapped_type = V;
    using value_type  = std::pair<const K, V>;
    using size_type   = std::size_t;

    explicit hash_table_chaining(size_type initial_buckets = 16)
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

    template <typename... Args>
    V& emplace(const K& key, Args&&... args) {
        auto& chain = table_[bucket(key)];
        for (auto& [k, v] : chain) {
            if (k == key) return v;
        }
        chain.push_back({key, V(std::forward<Args>(args)...)});
        ++size_;
        if (load_factor() > max_load_factor_)
            rehash(bucket_count_ * 2);
        return chain.back().second;
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

    size_type size()              const noexcept { return size_; }
    bool        empty()             const noexcept { return size_ == 0; }
    size_type bucket_count()      const noexcept { return bucket_count_; }
    double      load_factor()       const noexcept {
        return static_cast<double>(size_) / bucket_count_;
    }

    void set_max_load_factor(double mf) noexcept { max_load_factor_ = mf; }
    double max_load_factor() const noexcept { return max_load_factor_; }

    void reserve(size_type n) {
        if (n > bucket_count_) rehash(n * 2);
    }

    void clear() noexcept {
        for (auto& chain : table_) chain.clear();
        size_ = 0;
    }

    // ---- Forward iterator ----
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const K, V>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() : table_(nullptr), bucket_idx_(0) {}

        reference operator*() const { return *list_it_; }
        pointer operator->() const { return &(*list_it_); }

        iterator& operator++() {
            ++list_it_;
            advance_to_valid();
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return table_ == other.table_ && bucket_idx_ == other.bucket_idx_ && list_it_ == other.list_it_;
        }
        bool operator!=(const iterator& other) const { return !(*this == other); }

    private:
        friend class hash_table_chaining;
        using bucket_iterator = typename std::list<value_type>::iterator;

        hash_table_chaining* table_;
        size_type bucket_idx_;
        bucket_iterator list_it_;

        iterator(hash_table_chaining* t, size_type idx, bucket_iterator it)
            : table_(t), bucket_idx_(idx), list_it_(it) {}

        void advance_to_valid() {
            while (bucket_idx_ < table_->bucket_count_) {
                if (list_it_ != table_->table_[bucket_idx_].end()) return;
                ++bucket_idx_;
                if (bucket_idx_ < table_->bucket_count_)
                    list_it_ = table_->table_[bucket_idx_].begin();
            }
        }
    };

    iterator begin() {
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (!table_[i].empty())
                return iterator(this, i, table_[i].begin());
        }
        return end();
    }

    iterator end() {
        return iterator(this, bucket_count_,
                        bucket_count_ > 0 ? table_[bucket_count_ - 1].end() : table_[0].end());
    }

private:
    size_type bucket(const K& key) const {
        return Hash{}(key) % bucket_count_;
    }

    void rehash(size_type new_bucket_count) {
        std::vector<std::list<value_type>> new_table(new_bucket_count);
        for (auto& chain : table_) {
            for (auto& [k, v] : chain) {
                size_type b = Hash{}(k) % new_bucket_count;
                new_table[b].push_back({k, std::move(v)});
            }
        }
        table_        = std::move(new_table);
        bucket_count_ = new_bucket_count;
    }

    size_type bucket_count_;
    size_type size_;
    double      max_load_factor_ = 1.0;
    std::vector<std::list<value_type>> table_;
};

}  // namespace dsa

#endif
