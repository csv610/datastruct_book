template <typename T>
class message_queue {
public:
    void publish(const T& message) {
        std::scoped_lock lock(mutex_);
        queue_.push(message);
        cv_.notify_one();
    }

    T consume(std::chrono::milliseconds timeout = std::chrono::seconds(30)) {
        std::unique_lock lock(mutex_);
        if (!cv_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
            throw std::runtime_error("consume timeout -- no messages available");
        }
        T msg = queue_.front();
        queue_.pop();
        return msg;
    }

    size_t size() const {
        std::scoped_lock lock(mutex_);
        return queue_.size();
    }

private:
    linked_queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};
