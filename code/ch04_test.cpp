#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <string>
#include <functional>
#include <chrono>

// ch04: Linear Lists - Array List Implementation

template <typename T>
class array_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    
    explicit array_list(size_type capacity = 10) 
        : capacity_(capacity), size_(0), data_(std::make_unique<T[]>(capacity)) {}
    
    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    size_type capacity() const noexcept { return capacity_; }
    
    reference operator[](size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        return data_[index];
    }
    const_reference operator[](size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
        return data_[index];
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) resize(capacity_ * 2);
        data_[size_++] = value;
    }
    
    void push_back(T&& value) {
        if (size_ >= capacity_) resize(capacity_ * 2);
        data_[size_++] = std::move(value);
    }
    
    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        --size_;
    }
    
    void insert(size_type index, const T& value) {
        if (index > size_) throw std::out_of_range("index out of range");
        if (size_ >= capacity_) resize(capacity_ * 2);
        for (size_type i = size_; i > index; --i) data_[i] = std::move(data_[i - 1]);
        data_[index] = value;
        ++size_;
    }
    
    void erase(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        for (size_type i = index; i < size_ - 1; ++i) data_[i] = std::move(data_[i + 1]);
        --size_;
    }
    
    void clear() noexcept { size_ = 0; }
    
private:
    void resize(size_type new_cap) {
        auto new_data = std::make_unique<T[]>(new_cap);
        for (size_type i = 0; i < size_; ++i) new_data[i] = std::move(data_[i]);
        data_ = std::move(new_data);
        capacity_ = new_cap;
    }
    
    size_type capacity_;
    size_type size_;
    std::unique_ptr<T[]> data_;
};

// Singly linked list
template <typename T>
class linked_list {
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(const T& d) : data(d), next(nullptr) {}
        Node(T&& d) : data(std::move(d)), next(nullptr) {}
    };
    
    std::unique_ptr<Node> head_;
    std::size_t size_ = 0;
    
public:
    linked_list() = default;
    
    void push_front(const T& value) {
        auto n = std::make_unique<Node>(value);
        n->next = std::move(head_);
        head_ = std::move(n);
        ++size_;
    }
    
    void pop_front() {
        if (!head_) throw std::underflow_error("empty");
        head_ = std::move(head_->next);
        --size_;
    }
    
    T& front() { return head_->data; }
    const T& front() const { return head_->data; }
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    // Iterator
    class iterator {
        Node* ptr_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        iterator(Node* p = nullptr) : ptr_(p) {}
        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &ptr_->data; }
        iterator& operator++() { ptr_ = ptr_->next.get(); return *this; }
        iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& o) const { return ptr_ == o.ptr_; }
        bool operator!=(const iterator& o) const { return ptr_ != o.ptr_; }
    };
    
    iterator begin() { return iterator(head_.get()); }
    iterator end() { return iterator(nullptr); }
};

// Doubly linked list
template <typename T>
class doubly_linked_list {
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node* prev = nullptr;
        Node(const T& d) : data(d) {}
        Node(T&& d) : data(std::move(d)) {}
    };
    
    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
    
public:
    doubly_linked_list() = default;
    
    void push_front(const T& value) {
        auto n = std::make_unique<Node>(value);
        if (head_) {
            head_->prev = n.get();
            n->next = std::move(head_);
        } else {
            tail_ = n.get();
        }
        head_ = std::move(n);
        ++size_;
    }
    
    void push_back(const T& value) {
        auto n = std::make_unique<Node>(value);
        if (tail_) {
            tail_->next = std::move(n);
            tail_->next->prev = tail_;
            tail_ = tail_->next.get();
        } else {
            head_ = std::move(n);
            tail_ = head_.get();
        }
        ++size_;
    }
    
    void pop_front() {
        if (!head_) throw std::underflow_error("empty");
        head_ = std::move(head_->next);
        if (head_) head_->prev = nullptr;
        else tail_ = nullptr;
        --size_;
    }
    
    T& front() { return head_->data; }
    T& back() { return tail_->data; }
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};

// Rope data structure (simplified)
template <typename T>
class rope {
    struct Node {
        std::vector<T> data;
        std::unique_ptr<Node> left, right;
        std::size_t weight = 0;
        bool is_leaf = true;
        
        Node(std::vector<T>&& d) : data(std::move(d)), is_leaf(true) {
            weight = data.size();
        }
        Node(std::unique_ptr<Node>&& l, std::unique_ptr<Node>&& r) 
            : left(std::move(l)), right(std::move(r)), is_leaf(false) {
            weight = left ? left->weight : 0;
        }
    };
    
    std::unique_ptr<Node> root_;
    
    static std::unique_ptr<Node> build(const std::vector<T>& v, size_t l, size_t r) {
        if (r - l <= 16) {
            std::vector<T> chunk(v.begin() + l, v.begin() + r);
            return std::make_unique<Node>(std::move(chunk));
        }
        size_t m = l + (r - l) / 2;
        return std::make_unique<Node>(build(v, l, m), build(v, m, r));
    }
    
    static T& index(Node* n, size_t i) {
        if (n->is_leaf) return n->data[i];
        if (i < n->weight) return index(n->left.get(), i);
        return index(n->right.get(), i - n->weight);
    }
    
public:
    rope() = default;
    rope(const std::vector<T>& v) { if (!v.empty()) root_ = build(v, 0, v.size()); }
    
    T& operator[](size_t i) { return index(root_.get(), i); }
    const T& operator[](size_t i) const { return index(root_.get(), i); }
    size_t size() const { return root_ ? root_->weight + (root_->right ? root_->right->weight : 0) : 0; }
};

void test_array_list() {
    std::cout << "=== Testing array_list ===\n";
    array_list<int> al;
    for (int i = 0; i < 20; ++i) al.push_back(i);
    assert(al.size() == 20);
    assert(al[0] == 0);
    assert(al[19] == 19);
    al.insert(10, 99);
    assert(al[10] == 99);
    assert(al.size() == 21);
    al.erase(10);
    assert(al[10] == 10);
    assert(al.size() == 20);
    std::cout << "array_list: PASS\n";
}

void test_linked_list() {
    std::cout << "=== Testing linked_list ===\n";
    linked_list<int> ll;
    for (int i = 0; i < 10; ++i) ll.push_front(i);
    assert(ll.size() == 10);
    assert(ll.front() == 9);
    int sum = 0;
    for (int v : ll) sum += v;
    assert(sum == 45);  // 0+1+...+9
    std::cout << "linked_list: PASS\n";
}

void test_doubly_linked_list() {
    std::cout << "=== Testing doubly_linked_list ===\n";
    doubly_linked_list<int> dll;
    for (int i = 0; i < 5; ++i) dll.push_back(i);
    for (int i = 5; i < 10; ++i) dll.push_front(i);
    assert(dll.size() == 10);
    assert(dll.front() == 9);
    assert(dll.back() == 4);
    dll.pop_front();
    assert(dll.front() == 8);
    assert(dll.size() == 9);
    std::cout << "doubly_linked_list: PASS\n";
}

void test_rope() {
    std::cout << "=== Testing rope ===\n";
    std::vector<char> v(1000, 'a');
    for (size_t i = 0; i < v.size(); ++i) v[i] = 'a' + (i % 26);
    rope<char> r(v);
    assert(r.size() == 1000);
    assert(r[0] == 'a');
    assert(r[25] == 'z');
    assert(r[26] == 'a');
    assert(r[999] == 'd');
    std::cout << "rope: PASS\n";
}

int main() {
    std::cout << "=== Chapter 4: Linear Lists ===\n\n";
    test_array_list();
    test_linked_list();
    test_doubly_linked_list();
    test_rope();
    std::cout << "\n=== All tests passed ===\n";
    return 0;
}