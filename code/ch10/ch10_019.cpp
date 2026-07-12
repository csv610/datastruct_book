template <typename T, int D = 4, typename Compare = std::less<T>>
class d_ary_heap {
    std::vector<T> data_;
    Compare comp_;

    int parent(int i) const { return (i - 1) / D; }
    int child(int i, int k) const { return D * i + k + 1; }

    void sift_up(int i) {
        while (i > 0) {
            int p = parent(i);
            if (!comp_(data_[p], data_[i])) break;
            std::swap(data_[i], data_[p]);
            i = p;
        }
    }

    void sift_down(int i) {
        int n = static_cast<int>(data_.size());
        while (true) {
            int smallest = i;
            for (int k = 0; k < D; ++k) {
                int c = child(i, k);
                if (c < n && comp_(data_[smallest], data_[c]))
                    smallest = c;
            }
            if (smallest == i) break;
            std::swap(data_[i], data_[smallest]);
            i = smallest;
        }
    }

public:
    d_ary_heap() = default;

    void push(const T& value) {
        data_.push_back(value);
        sift_up(static_cast<int>(data_.size()) - 1);
    }

    void push(T&& value) {
        data_.push_back(std::move(value));
        sift_up(static_cast<int>(data_.size()) - 1);
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

    // Decrease-key: O(log_d n)
    void decrease_key(int i, const T& new_key) {
        data_[i] = new_key;
        sift_up(i);
    }

    bool empty() const noexcept { return data_.empty(); }
    int size() const noexcept { return static_cast<int>(data_.size()); }
};
