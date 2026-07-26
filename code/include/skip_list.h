#pragma once
#include <cstdlib>
#include <functional>
#include <memory>
#include <random>
#include <vector>

namespace dsa {

template <typename Key, typename Compare = std::less<Key>>
class skip_list {
public:
    static constexpr int MAX_LEVEL = 16;

    skip_list() : head_(new node(Key{}, MAX_LEVEL)), level_(0), size_(0),
                  rng_(std::random_device{}()), coin_(0, 1) {}

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

    bool contains(const Key& key) const {
        node* curr = head_;
        for (int i = level_; i >= 0; --i)
            while (curr->forward[i] && comp_(curr->forward[i]->key, key))
                curr = curr->forward[i];
        curr = curr->forward[0];
        return curr && !comp_(key, curr->key) && !comp_(curr->key, key);
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    std::vector<Key> to_sorted_vector() const {
        std::vector<Key> result;
        node* curr = head_->forward[0];
        while (curr) {
            result.push_back(curr->key);
            curr = curr->forward[0];
        }
        return result;
    }

private:
    struct node {
        Key key;
        std::vector<node*> forward;
        node(const Key& k, int level) : key(k), forward(level + 1, nullptr) {}
    };

    int random_level() {
        int lvl = 0;
        while (lvl < MAX_LEVEL && coin_(rng_) == 1) ++lvl;
        return lvl;
    }

    node* head_;
    int level_;
    std::size_t size_;
    Compare comp_;
    std::mt19937 rng_;
    std::uniform_int_distribution<int> coin_;
};

}  // namespace dsa
