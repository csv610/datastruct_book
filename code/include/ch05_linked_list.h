#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <concepts>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
struct node {
    T                      data;
    std::unique_ptr<node>  next;

    explicit node(const T& d) : data(d), next(nullptr) {}
    explicit node(T&& d)      : data(std::move(d)), next(nullptr) {}
};

template <std::regular T>
class linked_list {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;

    linked_list() = default;

    linked_list(std::initializer_list<T> init) {
        for (const auto& v : init)
            push_back(v);
    }

    linked_list(const linked_list& other) : size_(0) {
        node<T>* src = other.head_.get();
        node<T>** dst = &head_;
        while (src) {
            *dst = std::make_unique<node<T>>(src->data);
            dst  = &(*dst)->next;
            src  = src->next.get();
            ++size_;
        }
    }

    linked_list& operator=(const linked_list& other) {
        if (this != &other) {
            linked_list tmp(other);
            std::swap(head_, tmp.head_);
            std::swap(size_, tmp.size_);
        }
        return *this;
    }

    linked_list(linked_list&&) = default;
    linked_list& operator=(linked_list&&) = default;

    reference front() {
        if (empty())
            throw std::underflow_error("list is empty");
        return head_->data;
    }

    const_reference front() const {
        if (empty())
            throw std::underflow_error("list is empty");
        return head_->data;
    }

    reference back() {
        if (empty())
            throw std::underflow_error("list is empty");
        auto* cur = head_.get();
        while (cur->next)
            cur = cur->next.get();
        return cur->data;
    }

    size_type size()  const noexcept { return size_; }
    bool      empty() const noexcept { return size_ == 0; }

    void push_front(const T& value) {
        auto n = std::make_unique<node<T>>(value);
        n->next = std::move(head_);
        head_   = std::move(n);
        ++size_;
    }

    void push_front(T&& value) {
        auto n = std::make_unique<node<T>>(std::move(value));
        n->next = std::move(head_);
        head_   = std::move(n);
        ++size_;
    }

    void push_back(const T& value) {
        auto n = std::make_unique<node<T>>(value);
        *get_tail_ptr() = std::move(n);
        ++size_;
    }

    void push_back(T&& value) {
        auto n = std::make_unique<node<T>>(std::move(value));
        *get_tail_ptr() = std::move(n);
        ++size_;
    }

    void pop_front() {
        if (empty())
            throw std::underflow_error("list is empty");
        head_ = std::move(head_->next);
        --size_;
    }

    void clear() noexcept {
        head_.reset();
        size_ = 0;
    }

    void reverse() noexcept {
        std::unique_ptr<node<T>> prev;
        auto cur = std::move(head_);
        while (cur) {
            auto next = std::move(cur->next);
            cur->next = std::move(prev);
            prev      = std::move(cur);
            cur       = std::move(next);
        }
        head_ = std::move(prev);
    }

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator() : ptr_(nullptr) {}
        explicit iterator(node<T>* p) : ptr_(p) {}

        reference operator*()  const { return ptr_->data; }
        pointer   operator->() const { return &ptr_->data; }

        iterator& operator++() {
            ptr_ = ptr_->next.get();
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ptr_ = ptr_->next.get();
            return tmp;
        }

        bool operator==(const iterator& o) const { return ptr_ == o.ptr_; }
        bool operator!=(const iterator& o) const { return ptr_ != o.ptr_; }

    private:
        node<T>* ptr_;
    };

    iterator begin() noexcept { return iterator(head_.get()); }
    iterator end()   noexcept { return iterator(nullptr); }

private:
    node<T>** get_tail_ptr() {
        auto p = &head_;
        while (*p)
            p = &(*p)->next;
        return p;
    }

    std::unique_ptr<node<T>> head_;
    size_type                size_ = 0;
};

#endif
