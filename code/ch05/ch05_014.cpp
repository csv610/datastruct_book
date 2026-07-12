template <typename T>
class lower_triangular_matrix {
public:
    explicit lower_triangular_matrix(size_t n)
        : n_(n), data_(std::make_unique<T[]>(n * (n + 1) / 2)) {}

    T& operator()(size_t i, size_t j) {
        if (i < j) throw std::logic_error("below diagonal");
        return data_[i * (i + 1) / 2 + j];
    }
    const T& operator()(size_t i, size_t j) const {
        if (i < j) return zero_;
        return data_[i * (i + 1) / 2 + j];
    }

private:
    size_t n_;
    std::unique_ptr<T[]> data_;
    static constexpr T zero_ = T(0);
};
