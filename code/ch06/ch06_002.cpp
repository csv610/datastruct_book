template <typename T>
class linked_stack {
public:
    linked_stack() = default;

    void push(const T& value) {
        head_ = std::make_unique<node<T>>(value, std::move(head_));
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("stack is empty");
        head_ = std::move(head_->next);
        --size_;
    }

    T& top() {
        if (empty()) throw std::underflow_error("stack is empty");
        return head_->data;
    }
    const T& top() const {
        if (empty()) throw std::underflow_error("stack is empty");
        return head_->data;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        node(const T& d, std::unique_ptr<node> nxt)
            : data(d), next(std::move(nxt)) {}
    };
    std::unique_ptr<node> head_;
    size_t size_ = 0;
};
