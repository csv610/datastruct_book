template <typename T>
class array_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    explicit array_list(size_type capacity)
        : capacity_(capacity), size_(0), data_(std::make_unique<T[]>(capacity)) {}

    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    size_type capacity() const noexcept { return capacity_; }

    reference operator[](size_type index) {
        check_index(index);
        return data_[index];
    }

    const_reference operator[](size_type index) const {
        check_index(index);
        return data_[index];
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) throw std::overflow_error("list is full");
        data_[size_++] = value;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        --size_;
    }

    void insert(size_type index, const T& value) {
        if (size_ >= capacity_) throw std::overflow_error("list is full");
        if (index > size_) throw std::out_of_range("index out of range");
        for (size_type i = size_; i > index; --i) {
            data_[i] = data_[i - 1];
        }
        data_[index] = value;
        ++size_;
    }

    void erase(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        for (size_type i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
    }

    void clear() noexcept { size_ = 0; }

private:
    void check_index(size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
    }

    size_type capacity_;
    size_type size_;
    std::unique_ptr<T[]> data_;
};
