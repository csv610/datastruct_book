#include <vector>
#include <list>
#include <utility>
#include <optional>
#include <functional>

template <typename K, typename V,
          typename Hash = std::hash<K>>
class ExtendibleHash {
public:
    explicit ExtendibleHash(size_t bucket_size = 4)
        : bucket_cap_(bucket_size), global_depth_(0),
          directory_(1, new Bucket(bucket_size)) {}

    ~ExtendibleHash() {
        for (auto* b : directory_) delete b;
    }

    void insert(const K& key, const V& value) {
        size_t idx = hash_to_index(key);
        Bucket* bkt = directory_[idx];
        if (bkt->size() < bucket_cap_) {
            bkt->insert(key, value);
            return;
        }
        // Bucket full -- split it
        if (bkt->local_depth_ == global_depth_)
            double_directory();
        split_bucket(idx);
        insert(key, value);  // retry after split
    }

    std::optional<V> find(const K& key) const {
        size_t idx = hash_to_index(key);
        return directory_[idx]->find(key);
    }

    void erase(const K& key) {
        size_t idx = hash_to_index(key);
        directory_[idx]->erase(key);
    }

private:
    struct Bucket {
        int local_depth_;
        size_t cap_;
        std::list<std::pair<K, V>> entries_;

        explicit Bucket(size_t cap, int depth = 0)
            : local_depth_(depth), cap_(cap) {}

        bool full() const { return entries_.size() >= cap_; }
        size_t size() const { return entries_.size(); }

        void insert(const K& k, const V& v) {
            entries_.push_back({k, v});
        }
        std::optional<V> find(const K& k) const {
            for (auto& [ek, ev] : entries_)
                if (ek == k) return ev;
            return std::nullopt;
        }
        void erase(const K& k) {
            entries_.remove_if(
                [&](auto& p) { return p.first == k; });
        }
    };

    size_t hash_to_index(const K& key) const {
        size_t h = Hash{}(key);
        return h & ((1ULL << global_depth_) - 1);
    }

    void double_directory() {
        size_t old_size = 1ULL << global_depth_;
        size_t new_size = old_size * 2;
        directory_.resize(new_size);
        for (size_t i = 0; i < old_size; ++i)
            directory_[i + old_size] = directory_[i];
        ++global_depth_;
    }

    void split_bucket(size_t idx) {
        Bucket* old_bkt = directory_[idx];
        int old_depth = old_bkt->local_depth_;
        int new_depth = old_depth + 1;

        Bucket* b0 = new Bucket(bucket_cap_, new_depth);
        Bucket* b1 = new Bucket(bucket_cap_, new_depth);
        old_bkt->local_depth_ = new_depth;

        // Redistribute entries by one extra hash bit
        for (auto& [k, v] : old_bkt->entries_) {
            size_t h = Hash{}(k);
            size_t bit = (h >> old_depth) & 1;
            if (bit == 0) b0->insert(k, v);
            else          b1->insert(k, v);
        }

        // Update directory pointers
        size_t step = 1ULL << new_depth;
        for (size_t i = idx & (step - 1);
             i < (1ULL << global_depth_); i += step) {
            if (i == idx)                    directory_[i] = b0;
            else if (i == idx + step)        directory_[i] = b1;
        }
        // Fix duplicate pointers from the doubling
        size_t mask = step - 1;
        for (size_t i = 0; i < (1ULL << global_depth_); ++i) {
            if ((i & mask) == (idx & mask))
                directory_[i] = (i == idx) ? b0 : b1;
            else if ((i & mask) == ((idx + step / 2)
                                    & mask))
                directory_[i] = (i == idx + step / 2)
                                    ? b1 : directory_[i];
        }
        delete old_bkt;
    }

    size_t bucket_cap_;
    int global_depth_;
    std::vector<Bucket*> directory_;
};
