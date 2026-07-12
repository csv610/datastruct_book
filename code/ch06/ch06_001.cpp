template <typename T>
class array_stack {
public:
    array_stack() = default;

    void push(const T& value) {
        if (size_ >= data_.size()) {
            data_.push_back(value);
        } else {
            data_[size_] = value;
        }
        ++size_;
    }

    void push(T&& value) {
        if (size_ >= data_.size()) {
            data_.push_back(std::move(value));
        } else {
            data_[size_] = std::move(value);
        }
        ++size_;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if (size_ >= data_.size()) {
            data_.emplace_back(std::forward<Args>(args)...);
        } else {
            data_[size_] = T(std::forward<Args>(args)...);
        }
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("stack is empty");
        --size_;
    }

    T& top() {
        if (empty()) throw std::underflow_error("stack is empty");
        return data_[size_ - 1];
    }
    const T& top() const {
        if (empty()) throw std::underflow_error("stack is empty");
        return data_[size_ - 1];
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    std::vector<T> data_;
    size_t size_ = 0;
};
