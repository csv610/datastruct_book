class Buffer {
    size_t size_;
    int* data_;
public:
    // Constructor
    explicit Buffer(size_t size) : size_(size), data_(new int[size]) {}
    
    // Destructor
    ~Buffer() { delete[] data_; }
    
    // Copy constructor (expensive)
    Buffer(const Buffer& other) : size_(other.size_), data_(new int[other.size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
    
    // Move constructor (cheap)
    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }
    
    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }
};
