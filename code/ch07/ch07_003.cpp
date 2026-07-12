template <typename T>
class linked_queue {
public:
    linked_queue() = default;

    void push(const T& value) {
        auto new_node = std::make_unique<node>(value);
        if (rear_) {
            rear_->next = std::move(new_node);
            rear_ = rear_->next.get();
        } else {
            front_ = std::move(new_node);
            rear_ = front_.get();
        }
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("queue is empty");
        front_ = std::move(front_->next);
        if (!front_) rear_ = nullptr;
        --size_;
    }

    T& front() {
        if (empty()) throw std::underflow_error("queue is empty");
        return front_->data;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        explicit node(const T& d) : data(d) {}
    };
    std::unique_ptr<node> front_;
    node* rear_ = nullptr;
    size_t size_ = 0;
};
