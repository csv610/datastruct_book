#pragma once
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <memory>
#include <random>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class skip_list {
public:
    static constexpr int MAX_LEVEL = 16;

    using key_type = Key;
    using size_type = std::size_t;

    struct node {
        Key key;
        std::vector<node*> forward;
        node(const Key& k, int level) : key(k), forward(level + 1, nullptr) {}
    };

    // ---- Forward iterator ----
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const Key*;
        using reference = const Key&;

        iterator() : node_(nullptr) {}
        explicit iterator(node* n) : node_(n) {}

        reference operator*() const { return node_->key; }
        pointer operator->() const { return &node_->key; }

        iterator& operator++() {
            node_ = node_->forward[0];
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return node_ != other.node_; }

    private:
        node* node_;
    };

public:
    skip_list() : head_(new node(Key{}, MAX_LEVEL)), level_(0), size_(0),
                  comp_(Compare{}), rng_(std::random_device{}()), coin_(0, 1) {}

    explicit skip_list(Compare cmp)
        : head_(new node(Key{}, MAX_LEVEL)), level_(0), size_(0),
          comp_(std::move(cmp)), rng_(std::random_device{}()), coin_(0, 1) {}

    ~skip_list() {
        node* curr = head_;
        while (curr) {
            node* next = curr->forward[0];
            delete curr;
            curr = next;
        }
    }

    skip_list(const skip_list&) = delete;
    skip_list& operator=(const skip_list&) = delete;

    bool insert(const Key& key) {
        std::vector<node*> update(MAX_LEVEL + 1);
        node* curr = head_;
        for (int i = level_; i >= 0; --i) {
            while (curr->forward[i] && comp_(curr->forward[i]->key, key))
                curr = curr->forward[i];
            update[i] = curr;
        }
        curr = curr->forward[0];
        if (curr && !comp_(key, curr->key) && !comp_(curr->key, key))
            return false;

        int new_level = random_level();
        if (new_level > level_) {
            for (int i = level_ + 1; i <= new_level; ++i)
                update[i] = head_;
            level_ = new_level;
        }
        node* n = new node(key, new_level);
        for (int i = 0; i <= new_level; ++i) {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
        ++size_;
        return true;
    }

    template <typename... Args>
    bool emplace(Args&&... args) {
        return insert(Key(std::forward<Args>(args)...));
    }

    bool erase(const Key& key) {
        std::vector<node*> update(MAX_LEVEL + 1);
        node* curr = head_;
        for (int i = level_; i >= 0; --i) {
            while (curr->forward[i] && comp_(curr->forward[i]->key, key))
                curr = curr->forward[i];
            update[i] = curr;
        }
        curr = curr->forward[0];
        if (!curr || comp_(key, curr->key) || comp_(curr->key, key))
            return false;

        for (int i = 0; i <= level_; ++i) {
            if (update[i]->forward[i] != curr) break;
            update[i]->forward[i] = curr->forward[i];
        }
        delete curr;
        while (level_ > 0 && !head_->forward[level_]) --level_;
        --size_;
        return true;
    }

    const Key* find(const Key& key) const {
        node* curr = head_;
        for (int i = level_; i >= 0; --i)
            while (curr->forward[i] && comp_(curr->forward[i]->key, key))
                curr = curr->forward[i];
        curr = curr->forward[0];
        if (curr && !comp_(key, curr->key) && !comp_(curr->key, key))
            return &curr->key;
        return nullptr;
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    Compare key_comp() const { return comp_; }

    std::vector<Key> to_sorted_vector() const {
        std::vector<Key> result;
        node* curr = head_->forward[0];
        while (curr) {
            result.push_back(curr->key);
            curr = curr->forward[0];
        }
        return result;
    }

    iterator begin() { return iterator(head_->forward[0]); }
    iterator end() { return iterator(nullptr); }

private:
    int random_level() {
        int lvl = 0;
        while (lvl < MAX_LEVEL && coin_(rng_) == 1) ++lvl;
        return lvl;
    }

    node* head_;
    int level_;
    size_type size_;
    Compare comp_;
    std::mt19937 rng_;
    std::uniform_int_distribution<int> coin_;
};

}  // namespace dsa
