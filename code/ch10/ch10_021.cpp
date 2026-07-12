template <typename T, typename Compare = std::less<T>>
class min_max_heap {
    std::vector<T> data_;
    Compare comp_;

    bool is_min_level(int i) const {
        int level = 0;
        int temp = i;
        while (temp > 0) { temp = (temp - 1) / 2; ++level; }
        return level % 2 == 0;
    }

    // Sift up within a min level (compare with grandchildren)
    void sift_up_min(int i) {
        while (i > 0) {
            int grandparent = ((i - 1) / 2 - 1) / 2;
            if (grandparent >= 0 && comp_(data_[i], data_[grandparent])) {
                std::swap(data_[i], data_[grandparent]);
                i = grandparent;
            } else {
                break;
            }
        }
        // Final check with parent (opposite level)
        if (i > 0) {
            int p = (i - 1) / 2;
            if (comp_(data_[i], data_[p]))
                std::swap(data_[i], data_[p]);
        }
    }

    // Sift up within a max level (compare with grandchildren)
    void sift_up_max(int i) {
        while (i > 0) {
            int grandparent = ((i - 1) / 2 - 1) / 2;
            if (grandparent >= 0 && comp_(data_[grandparent], data_[i])) {
                std::swap(data_[i], data_[grandparent]);
                i = grandparent;
            } else {
                break;
            }
        }
        if (i > 0) {
            int p = (i - 1) / 2;
            if (comp_(data_[p], data_[i]))
                std::swap(data_[i], data_[p]);
        }
    }

    void sift_up(int i) {
        if (i == 0) return;
        int p = (i - 1) / 2;
        if (is_min_level(i)) {
            if (comp_(data_[i], data_[p])) {
                std::swap(data_[i], data_[p]);
                sift_up_max(p);
            } else {
                sift_up_min(i);
            }
        } else {
            if (comp_(data_[p], data_[i])) {
                std::swap(data_[i], data_[p]);
                sift_up_min(p);
            } else {
                sift_up_max(i);
            }
        }
    }

    void sift_down_min(int i) {
        int n = static_cast<int>(data_.size());
        while (true) {
            int smallest = i;
            // Check children
            int left = 2 * i + 1, right = 2 * i + 2;
            if (left < n && comp_(data_[left], data_[smallest]))
                smallest = left;
            if (right < n && comp_(data_[right], data_[smallest]))
                smallest = right;
            // Check grandchildren (shortcut)
            int gc = 2 * left + 1;
            for (int k = 0; k < 4 && gc + k < n; ++k) {
                if (comp_(data_[gc + k], data_[smallest]))
                    smallest = gc + k;
            }
            if (smallest == i) break;
            int p = (smallest - 1) / 2;
            std::swap(data_[i], data_[smallest]);
            // If we swapped with a grandchild, may need to fix parent
            if (p != i && comp_(data_[smallest], data_[p]))
                std::swap(data_[smallest], data_[p]);
            i = smallest;
        }
    }

    void sift_down_max(int i) {
        int n = static_cast<int>(data_.size());
        while (true) {
            int largest = i;
            int left = 2 * i + 1, right = 2 * i + 2;
            if (left < n && comp_(data_[largest], data_[left]))
                largest = left;
            if (right < n && comp_(data_[largest], data_[right]))
                largest = right;
            int gc = 2 * left + 1;
            for (int k = 0; k < 4 && gc + k < n; ++k) {
                if (comp_(data_[largest], data_[gc + k]))
                    largest = gc + k;
            }
            if (largest == i) break;
            int p = (largest - 1) / 2;
            std::swap(data_[i], data_[largest]);
            if (p != i && comp_(data_[p], data_[largest]))
                std::swap(data_[largest], data_[p]);
            i = largest;
        }
    }

public:
    min_max_heap() = default;

    void push(const T& value) {
        data_.push_back(value);
        sift_up(static_cast<int>(data_.size()) - 1);
    }

    void push(T&& value) {
        data_.push_back(std::move(value));
        sift_up(static_cast<int>(data_.size()) - 1);
    }

    const T& find_min() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return data_[0];
    }

    const T& find_max() const {
        if (empty()) throw std::underflow_error("heap is empty");
        if (data_.size() == 1) return data_[0];
        int left = 1, right = 2;
        int n = static_cast<int>(data_.size());
        int best = left;
        if (right < n && comp_(data_[left], data_[right]))
            best = right;
        return data_[best];
    }

    T pop_min() {
        if (empty()) throw std::underflow_error("heap is empty");
        T result = std::move(data_[0]);
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) sift_down_min(0);
        return result;
    }

    T pop_max() {
        if (empty()) throw std::underflow_error("heap is empty");
        int left = 1, right = 2;
        int n = static_cast<int>(data_.size());
        int best = left;
        if (right < n && comp_(data_[left], data_[right]))
            best = right;
        T result = std::move(data_[best]);
        data_[best] = std::move(data_.back());
        data_.pop_back();
        if (best < static_cast<int>(data_.size()))
            sift_down_max(best);
        return result;
    }

    bool empty() const noexcept { return data_.empty(); }
    int size() const noexcept { return static_cast<int>(data_.size()); }
};
