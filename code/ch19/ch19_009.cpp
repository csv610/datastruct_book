template <typename T>
class sliding_window_counter {
public:
    explicit sliding_window_counter(size_t window_size, double epsilon = 0.01)
        : window_(window_size), epsilon_(epsilon) {}

    void add(const T& item, size_t timestamp) {
        // Add to bucket 0
        buckets_[0].push_back({item, 1});
        total_ += 1;

        // Merge buckets with same size if needed
        compact();

        // Remove expired elements
        while (!buckets_.empty() && buckets_.front().start_time +
               buckets_.front().count <= timestamp - window_) {
            total_ -= buckets_.front().count;
            buckets_.pop_front();
        }
    }

    size_t count() const { return total_; }

    // Approximate count of item in current window
    size_t approximate_count(const T& item) const {
        size_t result = 0;
        for (const auto& bucket : buckets_) {
            for (const auto& [val, cnt] : bucket.items) {
                if (val == item) result += cnt;
            }
        }
        return result;
    }

private:
    struct bucket {
        std::deque<std::pair<T, int>> items;
        size_t count = 0;
        size_t start_time = 0;
    };

    void compact() {
        // Merge consecutive buckets of same size
        for (size_t i = 0; i + 1 < buckets_.size(); ++i) {
            while (buckets_[i].count == buckets_[i+1].count &&
                   buckets_[i].count > 0) {
                // Merge bucket i+1 into bucket i
                for (auto& item : buckets_[i+1].items)
                    buckets_[i].items.push_back(item);
                buckets_[i].count += buckets_[i+1].count;
                buckets_.erase(buckets_.begin() + i + 1);
            }
        }
    }

    size_t window_;
    double epsilon_;
    std::deque<bucket> buckets_;
    size_t total_ = 0;
};
