template <typename T>
class doubly_linked_list {
public:
    doubly_linked_list() : size_(0) {
        head_ = std::make_unique<dnode<T>>(T());
        tail_ = head_.get();
    }

    void push_front(const T& value) {
        auto new_node = std::make_unique<dnode<T>>(value);
        new_node->prev = head_.get();
        new_node->next = std::move(head_->next);
        if (new_node->next) {
            new_node->next->prev = new_node.get();
        } else {
            tail_ = new_node.get();
        }
        head_->next = std::move(new_node);
        ++size_;
    }

    void push_back(const T& value) {
        auto new_node = std::make_unique<dnode<T>>(value);
        new_node->prev = tail_;
        tail_->next = std::move(new_node);
        tail_ = tail_->next.get();
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        dnode<T>* new_tail = tail_->prev;
        new_tail->next.reset();
        tail_ = new_tail;
        --size_;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }

private:
    std::unique_ptr<dnode<T>> head_;
    dnode<T>* tail_;
    size_type size_ = 0;
};
