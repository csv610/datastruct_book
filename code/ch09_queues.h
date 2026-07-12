#ifndef QUEUES_H
#define QUEUES_H

#include <concepts>
#include <memory>
#include <stdexcept>
#include <utility>

// ---- Circular queue (array-based) ----
template <std::regular T>
class circular_queue {
public:
    explicit circular_queue(std::size_t initial_capacity = 8)
        : data_(std::make_unique<T[]>(initial_capacity))
        , capacity_(initial_capacity), front_(0), size_(0) {}

    void push(const T& value) {
        if (size_ == capacity_) grow();
        data_[(front_ + size_) % capacity_] = value;
        ++size_;
    }

    void push(T&& value) {
        if (size_ == capacity_) grow();
        data_[(front_ + size_) % capacity_] = std::move(value);
        ++size_;
    }

    void pop() {
        if (empty())
            throw std::underflow_error("queue is empty");
        front_ = (front_ + 1) % capacity_;
        --size_;
    }

    T& front() {
        if (empty())
            throw std::underflow_error("queue is empty");
        return data_[front_];
    }

    const T& front() const {
        if (empty())
            throw std::underflow_error("queue is empty");
        return data_[front_];
    }

    T& back() {
        if (empty())
            throw std::underflow_error("queue is empty");
        return data_[(front_ + size_ - 1) % capacity_];
    }

    const T& back() const {
        if (empty())
            throw std::underflow_error("queue is empty");
        return data_[(front_ + size_ - 1) % capacity_];
    }

    bool     empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

private:
    void grow() {
        std::size_t new_cap = capacity_ * 2;
        auto new_data = std::make_unique<T[]>(new_cap);
        for (std::size_t i = 0; i < size_; ++i)
            new_data[i] = std::move(data_[(front_ + i) % capacity_]);
        front_    = 0;
        capacity_ = new_cap;
        data_     = std::move(new_data);
    }

    std::unique_ptr<T[]> data_;
    std::size_t capacity_, front_, size_;
};

// ---- Linked queue ----
template <std::regular T>
class linked_queue {
    struct qnode {
        T data;
        std::unique_ptr<qnode> next;
        explicit qnode(const T& d) : data(d) {}
        explicit qnode(T&& d)      : data(std::move(d)) {}
    };

public:
    linked_queue() = default;

    void push(const T& value) {
        auto n = std::make_unique<qnode>(value);
        if (rear_) {
            rear_->next = std::move(n);
            rear_ = rear_->next.get();
        } else {
            front_ = std::move(n);
            rear_  = front_.get();
        }
        ++size_;
    }

    void push(T&& value) {
        auto n = std::make_unique<qnode>(std::move(value));
        if (rear_) {
            rear_->next = std::move(n);
            rear_ = rear_->next.get();
        } else {
            front_ = std::move(n);
            rear_  = front_.get();
        }
        ++size_;
    }

    void pop() {
        if (empty())
            throw std::underflow_error("queue is empty");
        front_ = std::move(front_->next);
        if (!front_) rear_ = nullptr;
        --size_;
    }

    T& front() {
        if (empty())
            throw std::underflow_error("queue is empty");
        return front_->data;
    }

    const T& front() const {
        if (empty())
            throw std::underflow_error("queue is empty");
        return front_->data;
    }

    T& back() {
        if (empty())
            throw std::underflow_error("queue is empty");
        return rear_->data;
    }

    bool     empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

private:
    std::unique_ptr<qnode> front_;
    qnode*                 rear_ = nullptr;
    std::size_t            size_ = 0;
};

#endif
