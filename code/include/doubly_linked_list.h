#pragma once
#include <cstddef>
#include <memory>
#include <utility>

namespace dsa {

// Doubly-linked list with sentinel node.
template <typename T>
class doubly_linked_list {
public:
    struct node {
        T data;
        node* prev = nullptr;
        node* next = nullptr;
        explicit node(const T& d) : data(d) {}
        explicit node(T&& d) : data(std::move(d)) {}
    };

    doubly_linked_list() { sentinel_.next = sentinel_.prev = &sentinel_; }

    ~doubly_linked_list() {
        clear();
    }

    doubly_linked_list(const doubly_linked_list&) = delete;
    doubly_linked_list& operator=(const doubly_linked_list&) = delete;

    doubly_linked_list(doubly_linked_list&& o) noexcept
        : sentinel_(std::move(o.sentinel_)), count_(o.count_) {
        o.count_ = 0;
        if (sentinel_.next) sentinel_.next->prev = &sentinel_;
        if (sentinel_.prev) sentinel_.prev->next = &sentinel_;
        o.sentinel_.next = o.sentinel_.prev = &o.sentinel_;
    }

    void push_front(const T& val) { insert(begin(), val); }
    void push_front(T&& val) { insert(begin(), std::move(val)); }
    void push_back(const T& val) { insert(end(), val); }
    void push_back(T&& val) { insert(end(), std::move(val)); }

    void pop_front() {
        if (empty()) return;
        erase_node(sentinel_.next);
    }
    void pop_back() {
        if (empty()) return;
        erase_node(sentinel_.prev);
    }

    T& front() { return sentinel_.next->data; }
    const T& front() const { return sentinel_.next->data; }
    T& back() { return sentinel_.prev->data; }
    const T& back() const { return sentinel_.prev->data; }

    bool empty() const { return sentinel_.next == &sentinel_; }
    std::size_t size() const { return count_; }

    void clear() {
        while (!empty()) pop_front();
    }

    void remove(const T& val) {
        node* cur = sentinel_.next;
        while (cur != &sentinel_) {
            node* nxt = cur->next;
            if (cur->data == val) erase_node(cur);
            cur = nxt;
        }
    }

    // Iterator support
    class iterator {
    public:
        iterator(node* n) : n_(n) {}
        T& operator*() { return n_->data; }
        iterator& operator++() { n_ = n_->next; return *this; }
        iterator operator++(int) { auto tmp = *this; n_ = n_->next; return tmp; }
        iterator& operator--() { n_ = n_->prev; return *this; }
        bool operator!=(const iterator& o) const { return n_ != o.n_; }
        bool operator==(const iterator& o) const { return n_ == o.n_; }
        node* get() const { return n_; }
    private:
        node* n_;
    };

    iterator begin() { return iterator(sentinel_.next); }
    iterator end() { return iterator(&sentinel_); }

    iterator insert(iterator pos, const T& val) {
        auto* n = new node(val);
        link_before(n, pos.get());
        ++count_;
        return iterator(n);
    }

    iterator insert(iterator pos, T&& val) {
        auto* n = new node(std::move(val));
        link_before(n, pos.get());
        ++count_;
        return iterator(n);
    }

    iterator erase(iterator pos) {
        node* nxt = pos.get()->next;
        erase_node(pos.get());
        return iterator(nxt);
    }

private:
    void link_before(node* n, node* next) {
        n->next = next;
        n->prev = next->prev;
        next->prev->next = n;
        next->prev = n;
    }

    void erase_node(node* n) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        delete n;
        --count_;
    }

    node sentinel_{T{}};
    std::size_t count_ = 0;
};

}  // namespace dsa
