template <typename T>
class linked_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    linked_list() = default;

    linked_list(std::initializer_list<T> init) {
        for (const auto& value : init) {
            push_back(value);
        }
    }

    linked_list(const linked_list& other) {
        node<T>* current = other.head_.get();
        node<T>** tail = &head_;
        while (current) {
            *tail = std::make_unique<node<T>>(current->data);
            tail = &(*tail)->next;
            current = current->next.get();
        }
        size_ = other.size_;
    }

    linked_list& operator=(const linked_list& other) {
        if (this != &other) {
            linked_list temp(other);
            std::swap(head_, temp.head_);
            std::swap(size_, temp.size_);
        }
        return *this;
    }

    linked_list(linked_list&&) = default;
    linked_list& operator=(linked_list&&) = default;

    reference front() {
        if (empty()) throw std::underflow_error("list is empty");
        return head_->data;
    }
    const_reference front() const {
        if (empty()) throw std::underflow_error("list is empty");
        return head_->data;
    }

    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void push_front(const T& value) {
        auto new_node = std::make_unique<node<T>>(value);
        new_node->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
    }

    void push_front(T&& value) {
        auto new_node = std::make_unique<node<T>>(std::move(value));
        new_node->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
    }

    void push_back(const T& value) {
        auto new_node = std::make_unique<node<T>>(value);
        node<T>** tail = get_tail_ptr();
        *tail = std::move(new_node);
        ++size_;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("list is empty");
        head_ = std::move(head_->next);
        --size_;
    }

    void clear() noexcept {
        head_.reset();
        size_ = 0;
    }

    reference operator[](size_type index) {
        node<T>* current = head_.get();
        for (size_type i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(node<T>* ptr) : ptr_(ptr) {}

        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &ptr_->data; }
        iterator& operator++() { ptr_ = ptr_->next.get(); return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

    private:
        node<T>* ptr_;
    };

    iterator begin() noexcept { return iterator(head_.get()); }
    iterator end() noexcept { return iterator(nullptr); }

private:
    node<T>** get_tail_ptr() {
        node<T>** p = &head_;
        while (*p) p = &(*p)->next;
        return p;
    }

    std::unique_ptr<node<T>> head_;
    size_type size_ = 0;
};
