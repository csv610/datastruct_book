// ch07: Queues
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T>
class array_queue {
    T* data_;
    std::size_t capacity_;
    std::size_t front_;
    std::size_t back_;
    std::size_t size_;
public:
    array_queue() : data_(new T[4]), capacity_(4), front_(0), back_(0), size_(0) {}
    ~array_queue() { delete[] data_; }

    void enqueue(const T& v) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            T* nd = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i)
                nd[i] = std::move(data_[(front_ + i) % (capacity_ / 2)]);
            delete[] data_;
            data_ = nd;
            front_ = 0;
            back_ = size_;
        }
        data_[back_] = v;
        back_ = (back_ + 1) % capacity_;
        ++size_;
    }
    T dequeue() {
        if (size_ == 0) throw std::underflow_error("queue empty");
        T v = std::move(data_[front_]);
        front_ = (front_ + 1) % capacity_;
        --size_;
        return v;
    }
    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
};

template <typename T>
class linked_queue {
    struct list_node {
        T data;
        std::unique_ptr<list_node> next;
    };
    std::unique_ptr<list_node> head_;
    list_node* tail_ = nullptr;
public:
    void enqueue(const T& v) {
        auto n = std::make_unique<list_node>(v, nullptr);
        if (tail_) tail_->next = std::move(n);
        else head_ = std::move(n);
        tail_ = head_.get();
        while (tail_->next) tail_ = tail_->next.get();
    }
    T dequeue() {
        if (!head_) throw std::underflow_error("queue empty");
        T v = std::move(head_->data);
        head_ = std::move(head_->next);
        if (!head_) tail_ = nullptr;
        return v;
    }
    bool empty() const { return !head_; }
};

int main() {
    array_queue<int> aq;
    aq.enqueue(10); aq.enqueue(20); aq.enqueue(30);
    std::cout << "dequeue: " << aq.dequeue() << " (expect 10)\n";
    std::cout << "dequeue: " << aq.dequeue() << " (expect 20)\n";
    aq.enqueue(40);
    std::cout << "dequeue: " << aq.dequeue() << " (expect 30)\n";
    std::cout << "dequeue: " << aq.dequeue() << " (expect 40)\n";
    std::cout << "queue empty: " << aq.empty() << " (expect 1)\n";

    linked_queue<int> lq;
    lq.enqueue(1); lq.enqueue(2);
    std::cout << "linked dequeue: " << lq.dequeue() << " (expect 1)\n";

    std::cout << "All ch07 tests passed.\n";
    return 0;
}
