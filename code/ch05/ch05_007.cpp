template <typename T>
class sorted_array {
public:
    void insert(const T& value) {
        auto pos = std::lower_bound(data_.begin(), data_.end(), value);
        data_.insert(pos, value);  // O(n) shift
    }

    bool contains(const T& value) const {
        return std::binary_search(data_.begin(), data_.end(), value);  // O(log n)
    }

    void erase(const T& value) {
        auto pos = std::lower_bound(data_.begin(), data_.end(), value);
        if (pos != data_.end() && *pos == value)
            data_.erase(pos);  // O(n) shift
    }

    // Range query: all elements in [lo, hi]
    auto range(const T& lo, const T& hi) const {
        auto first = std::lower_bound(data_.begin(), data_.end(), lo);
        auto last = std::upper_bound(data_.begin(), data_.end(), hi);
        return std::pair(first, last);  // O(log n) to find, O(k) to iterate
    }

    const T& operator[](size_t i) const { return data_[i]; }
    size_t size() const noexcept { return data_.size(); }

private:
    std::vector<T> data_;
};
