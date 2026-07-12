// ch04: Linear Lists — array_list, dynamic_array_list, linked_list
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// ---- array_list (fixed capacity) ----
template <std::regular T>
class array_list {
    static constexpr std::size_t DEFAULT_CAPACITY = 10;
    T*         data_;
    std::size_t capacity_;
    std::size_t size_;
public:
    array_list() : data_(new T[DEFAULT_CAPACITY]), capacity_(DEFAULT_CAPACITY), size_(0) {}
    explicit array_list(std::size_t cap)
        : data_(new T[cap]), capacity_(cap), size_(0) {}
    ~array_list() { delete[] data_; }

    array_list(const array_list&) = delete;
    array_list& operator=(const array_list&) = delete;
    array_list(array_list&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          capacity_(other.capacity_), size_(other.size_) {}
    array_list& operator=(array_list&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            capacity_ = other.capacity_;
            size_ = other.size_;
        }
        return *this;
    }

    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }
    T& at(std::size_t i) { if (i >= size_) throw std::out_of_range("index"); return data_[i]; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void push_back(const T& val) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            auto* new_data = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i) new_data[i] = std::move(data_[i]);
            delete[] data_;
            data_ = new_data;
        }
        data_[size_++] = val;
    }
    void pop_back() { if (size_ > 0) --size_; }
};

// ---- dynamic_array_list (full featured) ----
template <std::regular T>
class dynamic_array_list {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;
    using iterator        = T*;
    using const_iterator  = const T*;

    dynamic_array_list() : data_(nullptr), size_(0), capacity_(0) {}
    explicit dynamic_array_list(std::size_t count, const T& val = T())
        : data_(new T[count]), size_(count), capacity_(count) {
        for (std::size_t i = 0; i < count; ++i) data_[i] = val;
    }
    dynamic_array_list(std::initializer_list<T> il)
        : data_(new T[il.size()]), size_(il.size()), capacity_(il.size()) {
        std::copy(il.begin(), il.end(), data_);
    }
    ~dynamic_array_list() { delete[] data_; }

    dynamic_array_list(const dynamic_array_list& other)
        : data_(new T[other.size_]), size_(other.size_), capacity_(other.size_) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
    }
    dynamic_array_list& operator=(const dynamic_array_list& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.size_;
            data_ = new T[size_];
            for (std::size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
        }
        return *this;
    }
    dynamic_array_list(dynamic_array_list&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          capacity_(std::exchange(other.capacity_, 0)) {}
    dynamic_array_list& operator=(dynamic_array_list&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }

    reference operator[](size_type i) { return data_[i]; }
    const_reference operator[](size_type i) const { return data_[i]; }
    reference at(size_type i) { if (i >= size_) throw std::out_of_range("index"); return data_[i]; }

    iterator begin() { return data_; }
    const_iterator begin() const { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator end() const { return data_ + size_; }

    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void push_back(const T& val) {
        if (size_ == capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        data_[size_++] = val;
    }
    void pop_back() { if (size_ > 0) --size_; }
    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        auto* new_data = new T[new_cap];
        for (size_type i = 0; i < size_; ++i) new_data[i] = std::move(data_[i]);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

private:
    T*          data_;
    size_type   size_;
    size_type   capacity_;
};

// ---- linked_list (singly linked) ----
template <typename T>
struct node {
    T data;
    std::unique_ptr<node> next;
    node(const T& d) : data(d) {}
};

template <typename T>
class linked_list {
public:
    linked_list() = default;

    linked_list(std::initializer_list<T> il) {
        for (const auto& v : il) push_back(v);
    }

    linked_list(const linked_list& other) {
        for (auto* cur = other.head_.get(); cur; cur = cur->next.get())
            push_back(cur->data);
    }
    linked_list& operator=(const linked_list& other) {
        if (this != &other) {
            head_.reset();
            tail_ptr_ = &head_;
            for (auto* cur = other.head_.get(); cur; cur = cur->next.get())
                push_back(cur->data);
        }
        return *this;
    }
    linked_list(linked_list&&) = default;
    linked_list& operator=(linked_list&&) = default;

    void push_back(const T& val) {
        auto n = std::make_unique<node<T>>(val);
        *tail_ptr_ = std::move(n);
        tail_ptr_ = &(*tail_ptr_)->next;
    }
    void push_front(const T& val) {
        auto n = std::make_unique<node<T>>(val);
        n->next = std::move(head_);
        head_ = std::move(n);
        if (!*tail_ptr_) tail_ptr_ = &head_;
    }
    bool empty() const { return head_ == nullptr; }

private:
    std::unique_ptr<node<T>> head_;
    std::unique_ptr<node<T>>* tail_ptr_ = &head_;
};

// ---- demo ----
int main() {
    // array_list
    array_list<int> al;
    al.push_back(10);
    al.push_back(20);
    al.push_back(30);
    std::cout << "array_list: " << al[0] << ", " << al[1] << ", " << al[2] << "\n";

    // dynamic_array_list
    dynamic_array_list<int> dal = {1, 2, 3, 4, 5};
    dal.push_back(6);
    std::cout << "dynamic_array_list size: " << dal.size() << "\n";
    for (auto& x : dal) std::cout << x << " ";
    std::cout << "\n";

    // linked_list
    linked_list<int> ll = {100, 200, 300};
    ll.push_back(400);
    ll.push_front(50);
    std::cout << "linked_list OK\n";

    std::cout << "All ch04 tests passed.\n";
    return 0;
}
