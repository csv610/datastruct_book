template <typename T>
class deque_circular {
public:
    explicit deque_circular(int cap = 4)
        : cap_(cap), data_(new T[cap]), front_(0), sz_(0) {}

    ~deque_circular() { delete[] data_; }

    void push_front(const T& value) {
        if (sz_ == cap_) grow();
        front_ = (front_ - 1 + cap_) % cap_;
        data_[front_] = value;
        ++sz_;
    }

    void push_back(const T& value) {
        if (sz_ == cap_) grow();
        int rear = (front_ + sz_) % cap_;
        data_[rear] = value;
        ++sz_;
    }

    T pop_front() {
        if (sz_ == 0) throw std::underflow_error("empty");
        T val = data_[front_];
        front_ = (front_ + 1) % cap_;
        --sz_;
        return val;
    }

    T pop_back() {
        if (sz_ == 0) throw std::underflow_error("empty");
        int rear = (front_ + sz_ - 1) % cap_;
        T val = data_[rear];
        --sz_;
        return val;
    }

    T& operator[](int idx) {
        return data_[(front_ + idx) % cap_];
    }

    bool empty() const { return sz_ == 0; }
    int size() const { return sz_; }

private:
    void grow() {
        int new_cap = cap_ * 2;
        T* new_data = new T[new_cap];
        for (int i = 0; i < sz_; ++i) {
            new_data[i] = data_[(front_ + i) % cap_];
        }
        delete[] data_;
        data_ = new_data;
        cap_ = new_cap;
        front_ = 0;
    }

    int cap_;
    T* data_;
    int front_;
    int sz_;
};
