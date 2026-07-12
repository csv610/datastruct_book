template <typename T>
class matrix {
public:
    matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(std::make_unique<T[]>(rows * cols)) {}

    T& operator()(size_t i, size_t j) {
        return data_[i * cols_ + j];
    }
    const T& operator()(size_t i, size_t j) const {
        return data_[i * cols_ + j];
    }

    size_t rows() const noexcept { return rows_; }
    size_t cols() const noexcept { return cols_; }
    size_t size() const noexcept { return rows_ * cols_; }

    matrix operator+(const matrix& other) const {
        matrix result(rows_, cols_);
        for (size_t k = 0; k < size(); ++k) {
            result.data_[k] = data_[k] + other.data_[k];
        }
        return result;
    }

    matrix operator*(const matrix& other) const {
        matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t k = 0; k < cols_; ++k) {
                T aik = (*this)(i, k);
                if (aik != T(0)) {  // skip zero multiplies
                    for (size_t j = 0; j < other.cols_; ++j) {
                        result(i, j) += aik * other(k, j);
                    }
                }
            }
        }
        return result;
    }

private:
    size_t rows_, cols_;
    std::unique_ptr<T[]> data_;
};
