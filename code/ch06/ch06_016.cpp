template <typename T>
class min_stack {
public:
    void push(const T& value) {
        data_.push(value);
        if (min_.empty() || value <= min_.top()) {
            min_.push(value);
        }
    }

    void pop() {
        if (data_.empty())
            throw std::underflow_error("empty");
        if (data_.top() == min_.top()) {
            min_.pop();
        }
        data_.pop();
    }

    const T& top() const { return data_.top(); }

    const T& get_min() const {
        if (min_.empty())
            throw std::underflow_error("empty");
        return min_.top();
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }

private:
    linked_stack<T> data_;
    linked_stack<T> min_;
};
