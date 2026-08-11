#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <random>
#include <string_view>
#include <utility>
#include <vector>

namespace dsa {

class cuckoo_filter {
public:
    explicit cuckoo_filter(std::size_t capacity, std::size_t slots = 4,
                           std::size_t max_kicks = 500)
        : buckets_(std::max<std::size_t>(1, capacity)), slots_(std::max<std::size_t>(1, slots)), max_kicks_(max_kicks),
          table_(buckets_ * slots_, 0) {}

    bool insert(std::string_view key) {
        const auto fp = fingerprint(key);
        auto [a, b] = locations(key, fp);
        if (place(a, fp) || place(b, fp)) { ++size_; return true; }
        std::mt19937_64 rng(hash_key(key) ^ size_);
        std::size_t bucket = (rng() & 1) ? a : b;
        std::uint16_t current = fp;
        std::size_t last_bucket = bucket;
        std::size_t last_slot = 0;
        for (std::size_t i = 0; i < max_kicks_; ++i) {
            const std::size_t slot = rng() % slots_;
            last_bucket = bucket;
            last_slot = slot;
            std::swap(current, table_[bucket * slots_ + slot]);
            bucket = alternate(bucket, current);
            if (place(bucket, current)) { ++size_; return true; }
        }
        table_[last_bucket * slots_ + last_slot] = current;
        return false;
    }

    bool contains(std::string_view key) const {
        const auto fp = fingerprint(key);
        auto [a, b] = locations(key, fp);
        return has(a, fp) || has(b, fp);
    }

    bool erase(std::string_view key) {
        const auto fp = fingerprint(key);
        auto [a, b] = locations(key, fp);
        if (remove(a, fp) || remove(b, fp)) { --size_; return true; }
        return false;
    }

    std::size_t size() const noexcept { return size_; }
    double load_factor() const noexcept { return static_cast<double>(size_) / table_.size(); }

private:
    static std::uint64_t hash_key(std::string_view key) { return std::hash<std::string_view>{}(key); }
    std::uint16_t fingerprint(std::string_view key) const {
        auto fp = static_cast<std::uint16_t>(hash_key(key) & 0xFFFFu);
        return fp == 0 ? 1 : fp;
    }
    std::size_t bucket_hash(std::uint64_t h) const { return h % buckets_; }
    std::pair<std::size_t, std::size_t> locations(std::string_view key, std::uint16_t fp) const {
        const auto a = bucket_hash(hash_key(key));
        return {a, alternate(a, fp)};
    }
    std::size_t alternate(std::size_t bucket, std::uint16_t fp) const {
        return (bucket ^ (static_cast<std::size_t>(fp) * 0x9e3779b9u)) % buckets_;
    }
    bool place(std::size_t bucket, std::uint16_t fp) {
        for (std::size_t i = 0; i < slots_; ++i)
            if (table_[bucket * slots_ + i] == 0) { table_[bucket * slots_ + i] = fp; return true; }
        return false;
    }
    bool has(std::size_t bucket, std::uint16_t fp) const {
        for (std::size_t i = 0; i < slots_; ++i) if (table_[bucket * slots_ + i] == fp) return true;
        return false;
    }
    bool remove(std::size_t bucket, std::uint16_t fp) {
        for (std::size_t i = 0; i < slots_; ++i)
            if (table_[bucket * slots_ + i] == fp) { table_[bucket * slots_ + i] = 0; return true; }
        return false;
    }

    std::size_t buckets_, slots_, max_kicks_, size_ = 0;
    std::vector<std::uint16_t> table_;
};

} // namespace dsa
