#pragma once
#include <cstddef>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace dsa {

enum class oa_status { EMPTY, ACTIVE, DELETED };

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class open_addressing_hash {
public:
    explicit open_addressing_hash(std::size_t initial_cap = 16)
        : cap_(initial_cap), size_(0), table_(initial_cap) {
        for (auto& cell : table_) cell.status = oa_status::EMPTY;
    }

    bool insert(const Key& k, const Value& v) {
        if (size_ >= cap_ / 2) rehash(cap_ * 2);
        std::size_t idx = probe_start(k);
        std::size_t first_deleted = cap_;
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (idx + i) % cap_;
            if (table_[pos].status == oa_status::EMPTY) {
                std::size_t target = (first_deleted < cap_) ? first_deleted : pos;
                table_[target] = {k, v, oa_status::ACTIVE};
                ++size_;
                return true;
            }
            if (table_[pos].status == oa_status::DELETED && first_deleted == cap_)
                first_deleted = pos;
            if (table_[pos].status == oa_status::ACTIVE && table_[pos].key == k) {
                table_[pos].value = v;
                return true;
            }
        }
        return false;
    }

    bool erase(const Key& k) {
        std::size_t idx = probe_start(k);
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (idx + i) % cap_;
            if (table_[pos].status == oa_status::EMPTY) return false;
            if (table_[pos].status == oa_status::ACTIVE && table_[pos].key == k) {
                table_[pos].status = oa_status::DELETED;
                --size_;
                return true;
            }
        }
        return false;
    }

    Value* find(const Key& k) {
        std::size_t idx = probe_start(k);
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (idx + i) % cap_;
            if (table_[pos].status == oa_status::EMPTY) return nullptr;
            if (table_[pos].status == oa_status::ACTIVE && table_[pos].key == k)
                return &table_[pos].value;
        }
        return nullptr;
    }

    bool contains(const Key& k) const { return const_cast<open_addressing_hash*>(this)->find(k); }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Linear probing
    // Quadratic probing: idx + i*i
    // Double hashing: idx + i * hash2(k)

private:
    struct cell {
        Key key{};
        Value value{};
        oa_status status = oa_status::EMPTY;
    };

    std::size_t probe_start(const Key& k) const {
        return Hash{}(k) % cap_;
    }

    void rehash(std::size_t new_cap) {
        std::vector<cell> old = std::move(table_);
        cap_ = new_cap;
        size_ = 0;
        table_.assign(cap_, {});
        for (auto& cell : table_) cell.status = oa_status::EMPTY;
        for (auto& c : old)
            if (c.status == oa_status::ACTIVE)
                insert(c.key, c.value);
    }

    std::size_t cap_;
    std::size_t size_;
    std::vector<cell> table;
    std::vector<cell> table_;
};

// ---- Quadratic Probing Variant ----
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class quadratic_probing_hash {
public:
    explicit quadratic_probing_hash(std::size_t initial_cap = 16)
        : cap_(initial_cap), size_(0), keys_(cap_), vals_(cap_), occupied_(cap_, false) {}

    bool insert(const Key& k, const Value& v) {
        if (size_ >= cap_ / 2) rehash(cap_ * 2);
        std::size_t base = Hash{}(k) % cap_;
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (base + i * i) % cap_;
            if (!occupied_[pos]) {
                keys_[pos] = k; vals_[pos] = v; occupied_[pos] = true;
                ++size_;
                return true;
            }
            if (keys_[pos] == k) { vals_[pos] = v; return true; }
        }
        return false;
    }

    Value* find(const Key& k) {
        std::size_t base = Hash{}(k) % cap_;
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (base + i * i) % cap_;
            if (!occupied_[pos]) return nullptr;
            if (keys_[pos] == k) return &vals_[pos];
        }
        return nullptr;
    }

    bool contains(const Key& k) const { return const_cast<quadratic_probing_hash*>(this)->find(k); }
    std::size_t size() const { return size_; }

private:
    void rehash(std::size_t new_cap) {
        auto old_keys = std::move(keys_);
        auto old_vals = std::move(vals_);
        auto old_occ = std::move(occupied_);
        cap_ = new_cap; size_ = 0;
        keys_.assign(cap_, {}); vals_.assign(cap_, {}); occupied_.assign(cap_, false);
        for (std::size_t i = 0; i < old_occ.size(); ++i)
            if (old_occ[i]) insert(old_keys[i], old_vals[i]);
    }
    std::size_t cap_, size_;
    std::vector<Key> keys_;
    std::vector<Value> vals_;
    std::vector<bool> occupied_;
};

// ---- Double Hashing Variant ----
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class double_hashing_hash {
public:
    explicit double_hashing_hash(std::size_t initial_cap = 16)
        : cap_(initial_cap), size_(0), keys_(cap_), vals_(cap_), occupied_(cap_, false) {}

    bool insert(const Key& k, const Value& v) {
        if (size_ >= cap_ / 2) rehash(next_prime(cap_ * 2));
        std::size_t h1 = Hash{}(k) % cap_;
        std::size_t h2 = hash2(k) % cap_;
        if (h2 == 0) h2 = 1;
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (h1 + i * h2) % cap_;
            if (!occupied_[pos]) {
                keys_[pos] = k; vals_[pos] = v; occupied_[pos] = true;
                ++size_; return true;
            }
            if (keys_[pos] == k) { vals_[pos] = v; return true; }
        }
        return false;
    }

    Value* find(const Key& k) {
        std::size_t h1 = Hash{}(k) % cap_;
        std::size_t h2 = hash2(k) % cap_;
        if (h2 == 0) h2 = 1;
        for (std::size_t i = 0; i < cap_; ++i) {
            std::size_t pos = (h1 + i * h2) % cap_;
            if (!occupied_[pos]) return nullptr;
            if (keys_[pos] == k) return &vals_[pos];
        }
        return nullptr;
    }

    bool contains(const Key& k) const { return const_cast<double_hashing_hash*>(this)->find(k); }
    std::size_t size() const { return size_; }

private:
    static std::size_t hash2(const Key& k) {
        std::size_t h = std::hash<Key>{}(k);
        return 7 - (h % 7);
    }

    static bool is_prime(std::size_t n) {
        if (n < 2) return false;
        for (std::size_t i = 2; i * i <= n; ++i)
            if (n % i == 0) return false;
        return true;
    }
    static std::size_t next_prime(std::size_t n) {
        while (!is_prime(n)) ++n;
        return n;
    }

    void rehash(std::size_t new_cap) {
        auto ok = std::move(keys_); auto ov = std::move(vals_); auto oc = std::move(occupied_);
        cap_ = new_cap; size_ = 0;
        keys_.assign(cap_, {}); vals_.assign(cap_, {}); occupied_.assign(cap_, false);
        for (std::size_t i = 0; i < oc.size(); ++i)
            if (oc[i]) insert(ok[i], ov[i]);
    }
    std::size_t cap_, size_;
    std::vector<Key> keys_;
    std::vector<Value> vals_;
    std::vector<bool> occupied_;
};

}  // namespace dsa
