// A fixed-capacity circular queue.
// Uses one wasted slot to distinguish full from empty.
// When (rear_ + 1) % cap == front_, the queue is full.
// When front_ == rear_, the queue is empty.
template <typename T>
class circular_queue_v2 {
public:
    explicit circular_queue_v2(int capacity)
        : cap_(capacity + 1), data_(new T[capacity + 1]),
          front_(0), rear_(0) {}

    ~circular_queue_v2() { delete[] data_; }

    // Disallow copying for simplicity.
    circular_queue_v2(const circular_queue_v2&) = delete;
    circular_queue_v2& operator=(const circular_queue_v2&) = delete;

    void enqueue(const T& value) {
        if (is_full()) {
            throw std::overflow_error("queue is full");
        }
        data_[rear_] = value;
        rear_ = (rear_ + 1) % cap_;
    }

    T dequeue() {
        if (is_empty()) {
            throw std::underflow_error("queue is empty");
        }
        T value = data_[front_];
        front_ = (front_ + 1) % cap_;
        return value;
    }

    T& front() {
        if (is_empty()) {
            throw std::underflow_error("queue is empty");
        }
        return data_[front_];
    }

    bool is_empty() const { return front_ == rear_; }

    bool is_full() const {
        return (rear_ + 1) % cap_ == front_;
    }

    int size() const {
        return (rear_ - front_ + cap_) % cap_;
    }

private:
    int cap_;
    T* data_;
    int front_;   // index of the first element
    int rear_;    // index of the NEXT insertion point
};

// Driver: exercise all operations.
int main() {
    circular_queue_v2<int> q(5);   // capacity = 5

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    std::cout << "front: " << q.front() << "\n";  // 10
    std::cout << "size:  " << q.size()   << "\n";  // 3

    q.dequeue();
    q.enqueue(40);
    q.enqueue(50);
    q.enqueue(60);   // now full (capacity = 5)
    std::cout << "full: " << q.is_full() << "\n";  // 1

    // Demonstrates wrap-around: indices wrap to 0.
    while (!q.is_empty()) {
        std::cout << q.dequeue() << " ";
    }
    std::cout << "\n";  // 20 30 40 50 60
    return 0;
}
