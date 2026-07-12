template <typename T>
class ring_buffer {
    std::vector<T> buf_;
    size_t head_ = 0, tail_ = 0, size_ = 0;
public:
    explicit ring_buffer(size_t cap) : buf_(cap) {}
    void push(const T& v) {
        if (size_ == buf_.size()) throw std::overflow_error("full");
        buf_[tail_] = v;
        tail_ = (tail_ + 1) % buf_.size();
        ++size_;
    }
    T pop() {
        if (size_ == 0) throw std::underflow_error("empty");
        T v = buf_[head_];
        head_ = (head_ + 1) % buf_.size();
        --size_;
        return v;
    }
};
