#pragma once
#include <functional>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

// ---- Indexed min-priority queue with decrease_key ----
template <typename Key = int, typename Compare = std::greater<Key>>
class indexed_min_pq {
public:
    explicit indexed_min_pq(int n)
        : keys_(n), pos_(n, -1), heap_(), size_(0), compare_() {}

    bool contains(int i) const { return pos_[i] != -1; }

    void push(int i, Key key) {
        keys_[i] = key;
        pos_[i] = static_cast<int>(heap_.size());
        heap_.push_back(i);
        ++size_;
        sift_up(pos_[i]);
    }

    Key top_key() const {
        if (empty()) throw std::underflow_error("pq is empty");
        return keys_[heap_[0]];
    }

    int pop() {
        if (empty()) throw std::underflow_error("pq is empty");
        int i = heap_[0];
        pos_[i] = -1;
        --size_;
        int last = heap_.back();
        heap_.pop_back();
        if (!heap_.empty()) {
            heap_[0] = last;
            pos_[last] = 0;
            sift_down(0);
        }
        return i;
    }

    void decrease_key(int i, Key key) {
        keys_[i] = key;
        sift_up(pos_[i]);
    }

    void increase_key(int i, Key key) {
        keys_[i] = key;
        sift_down(pos_[i]);
    }

    Key key_of(int i) const { return keys_[i]; }

    bool        empty() const noexcept { return size_ == 0; }
    std::size_t size()  const noexcept { return size_; }

private:
    void sift_up(std::size_t i) {
        while (i > 0) {
            std::size_t p = (i - 1) / 2;
            if (!compare_(keys_[heap_[p]], keys_[heap_[i]])) break;
            std::swap(heap_[i], heap_[p]);
            pos_[heap_[i]] = static_cast<int>(i);
            pos_[heap_[p]] = static_cast<int>(p);
            i = p;
        }
    }

    void sift_down(std::size_t i) {
        std::size_t n = heap_.size();
        while (true) {
            std::size_t best = i;
            std::size_t l = 2 * i + 1, r = 2 * i + 2;
            if (l < n && compare_(keys_[heap_[best]], keys_[heap_[l]]))
                best = l;
            if (r < n && compare_(keys_[heap_[best]], keys_[heap_[r]]))
                best = r;
            if (best == i) break;
            std::swap(heap_[i], heap_[best]);
            pos_[heap_[i]] = static_cast<int>(i);
            pos_[heap_[best]] = static_cast<int>(best);
            i = best;
        }
    }

    std::vector<Key>    keys_;
    std::vector<int>    pos_;
    std::vector<int>    heap_;
    std::size_t         size_;
    Compare             compare_;
};

// ---- Indexed max-priority queue ----
template <typename Key = int, typename Compare = std::less<Key>>
class indexed_max_pq {
public:
    explicit indexed_max_pq(int n)
        : keys_(n), pos_(n, -1), heap_(), size_(0), compare_() {}

    bool contains(int i) const { return pos_[i] != -1; }

    void push(int i, Key key) {
        keys_[i] = key;
        pos_[i] = static_cast<int>(heap_.size());
        heap_.push_back(i);
        ++size_;
        sift_up(pos_[i]);
    }

    Key top_key() const {
        if (empty()) throw std::underflow_error("pq is empty");
        return keys_[heap_[0]];
    }

    int pop() {
        if (empty()) throw std::underflow_error("pq is empty");
        int i = heap_[0];
        pos_[i] = -1;
        --size_;
        int last = heap_.back();
        heap_.pop_back();
        if (!heap_.empty()) {
            heap_[0] = last;
            pos_[last] = 0;
            sift_down(0);
        }
        return i;
    }

    void increase_key(int i, Key key) {
        keys_[i] = key;
        sift_up(pos_[i]);
    }

    void decrease_key(int i, Key key) {
        keys_[i] = key;
        sift_down(pos_[i]);
    }

    Key key_of(int i) const { return keys_[i]; }

    bool        empty() const noexcept { return size_ == 0; }
    std::size_t size()  const noexcept { return size_; }

private:
    void sift_up(std::size_t i) {
        while (i > 0) {
            std::size_t p = (i - 1) / 2;
            if (!compare_(keys_[heap_[p]], keys_[heap_[i]])) break;
            std::swap(heap_[i], heap_[p]);
            pos_[heap_[i]] = static_cast<int>(i);
            pos_[heap_[p]] = static_cast<int>(p);
            i = p;
        }
    }

    void sift_down(std::size_t i) {
        std::size_t n = heap_.size();
        while (true) {
            std::size_t best = i;
            std::size_t l = 2 * i + 1, r = 2 * i + 2;
            if (l < n && compare_(keys_[heap_[best]], keys_[heap_[l]]))
                best = l;
            if (r < n && compare_(keys_[heap_[best]], keys_[heap_[r]]))
                best = r;
            if (best == i) break;
            std::swap(heap_[i], heap_[best]);
            pos_[heap_[i]] = static_cast<int>(i);
            pos_[heap_[best]] = static_cast<int>(best);
            i = best;
        }
    }

    std::vector<Key>    keys_;
    std::vector<int>    pos_;
    std::vector<int>    heap_;
    std::size_t         size_;
    Compare             compare_;
};

}  // namespace dsa
