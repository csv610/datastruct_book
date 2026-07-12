// ch05: Matrices — matrix, diagonal_matrix, triangular_matrix, csr_matrix
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T>
class matrix {
    std::size_t rows_, cols_;
    T* data_;
public:
    matrix(std::size_t r, std::size_t c)
        : rows_(r), cols_(c), data_(new T[r * c]) {}
    ~matrix() { delete[] data_; }

    matrix(const matrix& other)
        : rows_(other.rows_), cols_(other.cols_),
          data_(new T[rows_ * cols_]) {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) data_[i] = other.data_[i];
    }
    matrix& operator=(const matrix& other) {
        if (this != &other) {
            delete[] data_;
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = new T[rows_ * cols_];
            for (std::size_t i = 0; i < rows_ * cols_; ++i) data_[i] = other.data_[i];
        }
        return *this;
    }

    T& operator()(std::size_t i, std::size_t j) { return data_[i * cols_ + j]; }
    const T& operator()(std::size_t i, std::size_t j) const { return data_[i * cols_ + j]; }

    matrix operator+(const matrix& other) const {
        matrix result(rows_, cols_);
        for (std::size_t i = 0; i < rows_ * cols_; ++i)
            result.data_[i] = data_[i] + other.data_[i];
        return result;
    }
    matrix operator*(const matrix& other) const {
        matrix result(rows_, other.cols_);
        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t k = 0; k < cols_; ++k)
                for (std::size_t j = 0; j < other.cols_; ++j)
                    result(i, j) += (*this)(i, k) * other(k, j);
        return result;
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
};

template <typename T>
class diagonal_matrix {
    std::size_t n_;
    T* diag_;
public:
    diagonal_matrix(std::size_t n) : n_(n), diag_(new T[n]) {}
    ~diagonal_matrix() { delete[] diag_; }

    T get(std::size_t i, std::size_t j) const {
        if (i == j) return diag_[i];
        return T{};
    }
    void set(std::size_t i, std::size_t j, const T& v) {
        if (i == j) diag_[i] = v;
        else if (v != T{}) throw std::invalid_argument("non-zero off-diagonal");
    }
};

template <typename T>
class lower_triangular_matrix {
    std::size_t n_;
    T* data_;
public:
    lower_triangular_matrix(std::size_t n) : n_(n), data_(new T[n * (n + 1) / 2]) {}
    ~lower_triangular_matrix() { delete[] data_; }

    T get(std::size_t i, std::size_t j) const {
        if (i < j) return T{};
        return data_[i * (i + 1) / 2 + j];
    }
    void set(std::size_t i, std::size_t j, const T& v) {
        if (i < j && v != T{}) throw std::invalid_argument("non-zero upper");
        data_[i * (i + 1) / 2 + j] = v;
    }
};

template <typename T>
class upper_triangular_matrix {
    std::size_t n_;
    T* data_;
public:
    upper_triangular_matrix(std::size_t n) : n_(n), data_(new T[n * (n + 1) / 2]) {}
    ~upper_triangular_matrix() { delete[] data_; }

    T get(std::size_t i, std::size_t j) const {
        if (i > j) return T{};
        return data_[j * (j + 1) / 2 + i];
    }
    void set(std::size_t i, std::size_t j, const T& v) {
        if (i > j && v != T{}) throw std::invalid_argument("non-zero lower");
        data_[j * (j + 1) / 2 + i] = v;
    }
};

template <typename T>
class csr_matrix {
    std::size_t rows_, cols_;
    std::vector<T> values_;
    std::vector<std::size_t> col_indices_;
    std::vector<std::size_t> row_ptr_;
public:
    csr_matrix(std::size_t r, std::size_t c)
        : rows_(r), cols_(c), row_ptr_(r + 1, 0) {}

    void add_entry(std::size_t i, std::size_t j, const T& v) {
        values_.push_back(v);
        col_indices_.push_back(j);
        ++row_ptr_[i + 1];
    }

    void finalize() {
        for (std::size_t i = 1; i <= rows_; ++i)
            row_ptr_[i] += row_ptr_[i - 1];
    }

    T get(std::size_t i, std::size_t j) const {
        for (std::size_t k = row_ptr_[i]; k < row_ptr_[i + 1]; ++k)
            if (col_indices_[k] == j) return values_[k];
        return T{};
    }
};

int main() {
    // matrix
    matrix<int> m1(2, 3), m2(2, 3);
    m1(0, 0) = 1; m1(0, 1) = 2; m1(0, 2) = 3;
    m1(1, 0) = 4; m1(1, 1) = 5; m1(1, 2) = 6;
    m2(0, 0) = 6; m2(0, 1) = 5; m2(0, 2) = 4;
    m2(1, 0) = 3; m2(1, 1) = 2; m2(1, 2) = 1;
    auto m3 = m1 + m2;
    std::cout << "matrix[0][0]=" << m3(0, 0) << " (expect 7)\n";

    // diagonal
    diagonal_matrix<int> dm(3);
    dm.set(0, 0, 1); dm.set(1, 1, 2); dm.set(2, 2, 3);
    std::cout << "diagonal[1][1]=" << dm.get(1, 1) << " (expect 2)\n";

    // triangular
    lower_triangular_matrix<int> lt(3);
    lt.set(0, 0, 1); lt.set(1, 0, 2); lt.set(1, 1, 3);
    lt.set(2, 0, 4); lt.set(2, 1, 5); lt.set(2, 2, 6);
    std::cout << "lower_tri[2][1]=" << lt.get(2, 1) << " (expect 5)\n";

    // CSR
    csr_matrix<int> csr(3, 3);
    csr.add_entry(0, 0, 1); csr.add_entry(0, 2, 2);
    csr.add_entry(1, 1, 3); csr.add_entry(2, 0, 4);
    csr.finalize();
    std::cout << "csr[0][2]=" << csr.get(0, 2) << " (expect 2)\n";

    std::cout << "All ch05 tests passed.\n";
    return 0;
}
