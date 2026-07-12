#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

class sparse_matrix {
public:
    using triplet = std::tuple<int, int, double>;

    sparse_matrix(int rows, int cols)
        : rows_(rows), cols_(cols),
          row_ptr_(rows + 1, 0) {}

    sparse_matrix(int rows, int cols, std::vector<triplet> entries)
        : rows_(rows), cols_(cols) {
        // Sort entries by (row, col)
        std::sort(entries.begin(), entries.end(),
            [](const triplet& a, const triplet& b) {
                if (std::get<0>(a) != std::get<0>(b))
                    return std::get<0>(a) < std::get<0>(b);
                return std::get<1>(a) < std::get<1>(b);
            });

        // Build CRS arrays
        row_ptr_.assign(rows + 1, 0);
        for (const auto& [r, c, v] : entries) {
            if (v != 0.0) {
                row_ptr_[r + 1]++;
                values_.push_back(v);
                col_index_.push_back(c);
            }
        }

        // Convert counts to prefix sums
        for (int i = 0; i < rows; ++i) {
            row_ptr_[i + 1] += row_ptr_[i];
        }
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }
    int nnz() const { return static_cast<int>(values_.size()); }

    double get(int r, int c) const {
        int start = row_ptr_[r];
        int end = row_ptr_[r + 1];
        auto it = std::lower_bound(
            col_index_.begin() + start,
            col_index_.begin() + end, c);
        if (it != col_index_.begin() + end && *it == c) {
            return values_[it - col_index_.begin()];
        }
        return 0.0;
    }

    // O(nnz) matrix addition
    sparse_matrix add(const sparse_matrix& B) const {
        if (rows_ != B.rows_ || cols_ != B.cols_)
            throw std::invalid_argument("dimension mismatch");

        sparse_matrix result(rows_, cols_);
        std::vector<triplet> entries;

        for (int i = 0; i < rows_; ++i) {
            int a_start = row_ptr_[i], a_end = row_ptr_[i + 1];
            int b_start = B.row_ptr_[i], b_end = B.row_ptr_[i + 1];
            int ai = a_start, bi = b_start;
            while (ai < a_end || bi < b_end) {
                int ac = (ai < a_end) ? col_index_[ai] : cols_;
                int bc = (bi < b_end) ? B.col_index_[bi] : cols_;
                if (ac == bc) {
                    double sum = values_[ai] + B.values_[bi];
                    if (sum != 0.0) entries.emplace_back(i, ac, sum);
                    ++ai; ++bi;
                } else if (ac < bc) {
                    entries.emplace_back(i, ac, values_[ai]);
                    ++ai;
                } else {
                    entries.emplace_back(i, bc, B.values_[bi]);
                    ++bi;
                }
            }
        }
        return sparse_matrix(rows_, cols_, entries);
    }

    // O(nnz(A) * n_cols(B)) sparse matrix multiply
    sparse_matrix multiply(const sparse_matrix& B) const {
        if (cols_ != B.rows_)
            throw std::invalid_argument("dimension mismatch");

        std::vector<triplet> entries;
        for (int i = 0; i < rows_; ++i) {
            std::vector<double> tmp(B.cols_, 0.0);
            for (int ai = row_ptr_[i]; ai < row_ptr_[i + 1]; ++ai) {
                int k = col_index_[ai];
                double a_val = values_[ai];
                for (int bj = B.row_ptr_[k]; bj < B.row_ptr_[k + 1]; ++bj) {
                    tmp[B.col_index_[bj]] += a_val * B.values_[bj];
                }
            }
            for (int j = 0; j < B.cols_; ++j) {
                if (tmp[j] != 0.0) entries.emplace_back(i, j, tmp[j]);
            }
        }
        return sparse_matrix(rows_, B.cols_, entries);
    }

    // O(m + n + nnz) transpose
    sparse_matrix transpose() const {
        sparse_matrix result(cols_, rows_);
        result.row_ptr_.assign(cols_ + 1, 0);

        // Count entries per column (now row in result)
        for (int c : col_index_) {
            result.row_ptr_[c + 1]++;
        }
        for (int i = 0; i < cols_; ++i) {
            result.row_ptr_[i + 1] += result.row_ptr_[i];
        }

        result.values_.resize(nnz());
        result.col_index_.resize(nnz());
        std::vector<int> pos(result.row_ptr_.begin(), result.row_ptr_.end());

        for (int i = 0; i < rows_; ++i) {
            for (int j = row_ptr_[i]; j < row_ptr_[i + 1]; ++j) {
                int c = col_index_[j];
                int dest = pos[c]++;
                result.values_[dest] = values_[j];
                result.col_index_[dest] = i;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const sparse_matrix& m) {
        for (int i = 0; i < m.rows_; ++i) {
            for (int j = 0; j < m.cols_; ++j) {
                if (j > 0) os << " ";
                os << m.get(i, j);
            }
            os << "\n";
        }
        return os;
    }

private:
    int rows_, cols_;
    std::vector<double> values_;
    std::vector<int> col_index_;
    std::vector<int> row_ptr_;
};
