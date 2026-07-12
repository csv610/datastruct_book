#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

template <std::regular T>
class array_list {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;
    using iterator        = T*;
    using const_iterator  = const T*;

    static constexpr size_type default_capacity = 4;

    array_list() : size_(0), capacity_(default_capacity),
                   data_(std::make_unique<T[]>(default_capacity)) {}

    explicit array_list(size_type n) : size_(n), capacity_(n),
                                       data_(std::make_unique<T[]>(n)) {}

    array_list(std::initializer_list<T> init)
        : size_(init.size()), capacity_(init.size()),
          data_(std::make_unique<T[]>(init.size())) {
        std::copy(init.begin(), init.end(), data_.get());
    }

    array_list(const array_list& other)
        : size_(other.size_), capacity_(other.capacity_),
          data_(std::make_unique<T[]>(other.capacity_)) {
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
    }

    array_list& operator=(const array_list& other) {
        if (this != &other) {
            auto tmp = std::make_unique<T[]>(other.capacity_);
            std::copy(other.data_.get(), other.data_.get() + other.size_, tmp.get());
            size_     = other.size_;
            capacity_ = other.capacity_;
            data_     = std::move(tmp);
        }
        return *this;
    }

    array_list(array_list&&) = default;
    array_list& operator=(array_list&&) = default;

    reference operator[](size_type i) {
        check_index(i);
        return data_[i];
    }

    const_reference operator[](size_type i) const {
        check_index(i);
        return data_[i];
    }

    reference at(size_type i) {
        if (i >= size_) throw std::out_of_range("index out of range");
        return data_[i];
    }

    const_reference at(size_type i) const {
        if (i >= size_) throw std::out_of_range("index out of range");
        return data_[i];
    }

    reference front() {
        if (empty()) throw std::underflow_error("list is empty");
        return data_[0];
    }

    const_reference front() const {
        if (empty()) throw std::underflow_error("list is empty");
        return data_[0];
    }

    reference back() {
        if (empty()) throw std::underflow_error("list is empty");
        return data_[size_ - 1];
    }

    const_reference back() const {
        if (empty()) throw std::underflow_error("list is empty");
        return data_[size_ - 1];
    }

    size_type size()      const noexcept { return size_; }
    size_type capacity()  const noexcept { return capacity_; }
    bool      empty()     const noexcept { return size_ == 0; }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_)
            grow(new_capacity);
    }

    void push_back(const T& value) {
        if (size_ >= capacity_)
            grow(2 * capacity_);
        data_[size_++] = value;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_)
            grow(2 * capacity_);
        data_[size_++] = std::move(value);
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ >= capacity_)
            grow(2 * capacity_);
        data_[size_] = T(std::forward<Args>(args)...);
        return data_[size_++];
    }

    void pop_back() {
        if (empty())
            throw std::underflow_error("list is empty");
        --size_;
    }

    void insert(size_type index, const T& value) {
        if (index > size_)
            throw std::out_of_range("index out of range");
        if (size_ >= capacity_)
            grow(2 * capacity_);
        for (size_type i = size_; i > index; --i)
            data_[i] = std::move(data_[i - 1]);
        data_[index] = value;
        ++size_;
    }

    void erase(size_type index) {
        if (index >= size_)
            throw std::out_of_range("index out of range");
        for (size_type i = index; i < size_ - 1; ++i)
            data_[i] = std::move(data_[i + 1]);
        --size_;
    }

    void clear() noexcept { size_ = 0; }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            auto tmp = std::make_unique<T[]>(size_);
            std::move(data_.get(), data_.get() + size_, tmp.get());
            capacity_ = size_;
            data_     = std::move(tmp);
        }
    }

    iterator       begin()       noexcept { return data_.get(); }
    const_iterator begin() const noexcept { return data_.get(); }
    iterator       end()         noexcept { return data_.get() + size_; }
    const_iterator end()   const noexcept { return data_.get() + size_; }

private:
    void check_index(size_type i) const {
        if (i >= size_)
            throw std::out_of_range("index out of range");
    }

    void grow(size_type new_capacity) {
        auto tmp = std::make_unique<T[]>(new_capacity);
        std::move(data_.get(), data_.get() + size_, tmp.get());
        capacity_ = new_capacity;
        data_     = std::move(tmp);
    }

    size_type              size_;
    size_type              capacity_;
    std::unique_ptr<T[]>   data_;
};

#endif
