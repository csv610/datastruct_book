template <typename T>
class circular_queue {
public:
    explicit circular_queue(size_t initial_capacity = 8)
        : data_(std::make_unique<T[]>(initial_capacity))
        , capacity_(initial_capacity), front_(0), size_(0) {}

    void push(const T& value) {
        if (size_ == capacity_) grow();
        size_t pos = (front_ + size_) % capacity_;
        data_[pos] = value;
        ++size_;
    }

    void push(T&& value) {
        if (size_ == capacity_) grow();
        size_t pos = (front_ + size_) % capacity_;
        data_[pos] = std::move(value);
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("queue is empty");
        front_ = (front_ + 1) % capacity_;
        --size_;
    }

    T& front() {
        if (empty()) throw std::underflow_error("queue is empty");
        return data_[front_];
    }
    const T& front() const {
        if (empty()) throw std::underflow_error("queue is empty");
        return data_[front_];
    }

    T& back() {
        if (empty()) throw std::underflow_error("queue is empty");
        return data_[(front_ + size_ - 1) % capacity_];
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    void grow() {
        size_t new_capacity = capacity_ * 2;
        auto new_data = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[(front_ + i) % capacity_]);
        }
        front_ = 0;
        capacity_ = new_capacity;
        data_ = std::move(new_data);
    }

    std::unique_ptr<T[]> data_;
    size_t capacity_, front_, size_;
};
