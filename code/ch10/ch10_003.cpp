template <typename T, typename Compare = std::less<T>>
class max_heap {
public:
    max_heap() = default;

    void push(const T& value) {
        data_.push_back(value);
        sift_up(data_.size() - 1);
    }

    void push(T&& value) {
        data_.push_back(std::move(value));
        sift_up(data_.size() - 1);
    }

    void pop() {
        if (empty()) throw std::underflow_error("heap is empty");
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) sift_down(0);
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return data_[0];
    }

    bool empty() const noexcept { return data_.empty(); }
    size_t size() const noexcept { return data_.size(); }

private:
    void sift_up(size_t i) {
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (!compare_(data_[parent], data_[i])) break;
            std::swap(data_[i], data_[parent]);
            i = parent;
        }
    }

    void sift_down(size_t i) {
        size_t n = data_.size();
        while (true) {
            size_t largest = i;
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;

            if (left < n && compare_(data_[largest], data_[left]))
                largest = left;
            if (right < n && compare_(data_[largest], data_[right]))
                largest = right;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }

    std::vector<T> data_;
    Compare compare_;  // less<T>  ->  max-heap
};
