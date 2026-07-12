template <typename T>
class diagonal_matrix {
public:
    explicit diagonal_matrix(size_t n) : n_(n), data_(std::make_unique<T[]>(n)) {}

    T& operator()(size_t i, size_t j) {
        if (i != j) throw std::logic_error("non-diagonal access");
        return data_[i];
    }
    const T& operator()(size_t i, size_t j) const {
        if (i != j) return zero_;
        return data_[i];
    }

private:
    size_t n_;
    std::unique_ptr<T[]> data_;
    static constexpr T zero_ = T(0);
};
