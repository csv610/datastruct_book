template <typename T>
class dynamic_array_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;

    static constexpr size_type default_capacity = 4;

    dynamic_array_list() : size_(0), capacity_(default_capacity),
                           data_(std::make_unique<T[]>(default_capacity)) {}

    dynamic_array_list(std::initializer_list<T> init)
        : size_(init.size()), capacity_(init.size()),
          data_(std::make_unique<T[]>(init.size())) {
        std::copy(init.begin(), init.end(), data_.get());
    }

    dynamic_array_list(const dynamic_array_list& other)
        : size_(other.size_), capacity_(other.capacity_),
          data_(std::make_unique<T[]>(other.capacity_)) {
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
    }

    dynamic_array_list& operator=(const dynamic_array_list& other) {
        if (this != &other) {
            auto new_data = std::make_unique<T[]>(other.capacity_);
            std::copy(other.data_.get(), other.data_.get() + other.size_, new_data.get());
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = std::move(new_data);
        }
        return *this;
    }

    dynamic_array_list(dynamic_array_list&&) = default;
    dynamic_array_list& operator=(dynamic_array_list&&) = default;

    reference operator[](size_type index) {
        check_index(index);
        return data_[index];
    }
    const_reference operator[](size_type index) const {
        check_index(index);
        return data_[index];
    }

    reference at(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        return data_[index];
    }
    const_reference at(size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
        return data_[index];
    }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            grow(new_capacity);
        }
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) grow(2 * capacity_);
        data_[size_++] = value;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) grow(2 * capacity_);
        data_[size_++] = std::move(value);
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ >= capacity_) grow(2 * capacity_);
        data_[size_] = T(std::forward<Args>(args)...);
        return data_[size_++];
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        --size_;
    }

    void insert(size_type index, const T& value) {
        if (index > size_) throw std::out_of_range("index out of range");
        if (size_ >= capacity_) grow(2 * capacity_);
        for (size_type i = size_; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[index] = value;
        ++size_;
    }

    void erase(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        for (size_type i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        --size_;
    }

    void clear() noexcept { size_ = 0; }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            auto new_data = std::make_unique<T[]>(size_);
            std::move(data_.get(), data_.get() + size_, new_data.get());
            capacity_ = size_;
            data_ = std::move(new_data);
        }
    }

    iterator begin() noexcept { return data_.get(); }
    const_iterator begin() const noexcept { return data_.get(); }
    iterator end() noexcept { return data_.get() + size_; }
    const_iterator end() const noexcept { return data_.get() + size_; }

private:
    void check_index(size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
    }

    void grow(size_type new_capacity) {
        auto new_data = std::make_unique<T[]>(new_capacity);
        std::move(data_.get(), data_.get() + size_, new_data.get());
        capacity_ = new_capacity;
        data_ = std::move(new_data);
    }

    size_type size_;
    size_type capacity_;
    std::unique_ptr<T[]> data_;
};
