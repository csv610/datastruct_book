template <typename T>
class csr_matrix {
public:
    csr_matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), row_ptr_(rows + 1, 0) {}

    void finalize() {
        // Convert from COO to CSR
        std::sort(coo_.begin(), coo_.end(),
            [](const auto& a, const auto& b) {
                return std::tie(a.row, a.col) < std::tie(b.row, b.col);
            });

        // Build CSR
        size_t nnz = coo_.size();
        values_.reserve(nnz);
        col_indices_.reserve(nnz);

        for (const auto& t : coo_) {
            values_.push_back(t.value);
            col_indices_.push_back(t.col);
        }

        // Build row_ptr
        size_t idx = 0;
        for (size_t i = 0; i < rows_; ++i) {
            row_ptr_[i] = idx;
            while (idx < nnz && coo_[idx].row == i) ++idx;
        }
        row_ptr_[rows_] = nnz;

        coo_.clear();  // free temporary storage
    }

    T operator()(size_t row, size_t col) const {
        for (size_t i = row_ptr_[row]; i < row_ptr_[row + 1]; ++i) {
            if (col_indices_[i] == col) return values_[i];
        }
        return T(0);
    }

    // Sparse matrix-vector multiplication: result = A * x
    std::vector<T> multiply(const std::vector<T>& x) const {
        std::vector<T> result(rows_, T(0));
        for (size_t i = 0; i < rows_; ++i) {
            T sum = T(0);
            for (size_t j = row_ptr_[i]; j < row_ptr_[i + 1]; ++j) {
                sum += values_[j] * x[col_indices_[j]];
            }
            result[i] = sum;
        }
        return result;
    }

    size_t rows() const noexcept { return rows_; }
    size_t cols() const noexcept { return cols_; }
    size_t nonzero_count() const noexcept { return values_.size(); }

private:
    size_t rows_, cols_;
    std::vector<T> values_;
    std::vector<size_t> col_indices_;
    std::vector<size_t> row_ptr_;
    std::vector<triple> coo_;  // temporary during construction
};
