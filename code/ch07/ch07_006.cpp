class thread_pool {
public:
    explicit thread_pool(size_t num_threads = std::thread::hardware_concurrency()) {
        workers_.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { worker_loop(); });
        }
    }

    ~thread_pool() {
        {
            std::scoped_lock lock(mutex_);
            done_ = true;
        }
        cv_.notify_all();
        for (auto& t : workers_) t.join();
    }

    void enqueue(std::function<void()> task) {
        std::scoped_lock lock(mutex_);
        tasks_.push(std::move(task));
        cv_.notify_one();
    }

private:
    void worker_loop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock lock(mutex_);
                cv_.wait(lock, [this] { return done_ || !tasks_.empty(); });
                if (done_ && tasks_.empty()) return;
                task = tasks_.front(); tasks_.pop();
            }
            task();  // execute outside the lock
        }
    }

    linked_queue<std::function<void()>> tasks_;
    std::vector<std::thread> workers_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool done_ = false;
};
