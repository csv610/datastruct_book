template <typename T>
class blocking_queue {
    std::queue<T> q_;
    size_t cap_;
    mutable std::mutex mtx_;
    std::condition_variable not_full_, not_empty_;
public:
    explicit blocking_queue(size_t cap) : cap_(cap) {}
    void push(T item) {
        std::unique_lock lock(mtx_);
        not_full_.wait(lock, [&]{ return q_.size() < cap_; });
        q_.push(std::move(item));
        not_empty_.notify_one();
    }
    T pop() {
        std::unique_lock lock(mtx_);
        not_empty_.wait(lock, [&]{ return !q_.size(); });
        T item = std::move(q_.front()); q_.pop();
        not_full_.notify_one();
        return item;
    }
};
