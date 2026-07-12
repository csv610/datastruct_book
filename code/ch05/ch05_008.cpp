template <typename T>
class circular_buffer {
public:
    explicit circular_buffer(size_t capacity)
        : buf_(capacity), head_(0), tail_(0), size_(0) {}

    void push(const T& val) {
        if (size_ == buf_.size())
            throw std::overflow_error("buffer full");
        buf_[tail_] = val;
        tail_ = (tail_ + 1) % buf_.size();
        ++size_;
    }

    T pop() {
        if (size_ == 0)
            throw std::underflow_error("buffer empty");
        T val = buf_[head_];
        head_ = (head_ + 1) % buf_.size();
        --size_;
        return val;
    }

    const T& front() const { return buf_[head_]; }
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }

private:
    std::vector<T> buf_;
    size_t head_, tail_, size_;
};
