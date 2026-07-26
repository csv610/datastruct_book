#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T>
class SparseMatrix {
 public:
  struct Triplet {
    std::size_t row;
    std::size_t col;
    T value;
  };

  SparseMatrix() = default;

  SparseMatrix(std::size_t rows, std::size_t cols)
      : rows_(rows), cols_(cols) {}

  explicit SparseMatrix(const std::vector<std::vector<T>>& dense) {
    rows_ = dense.size();
    cols_ = dense.empty() ? 0 : dense[0].size();
    for (std::size_t r = 0; r < rows_; ++r) {
      for (std::size_t c = 0; c < cols_; ++c) {
        if (dense[r][c] != T{}) {
          triplets_.push_back({r, c, dense[r][c]});
        }
      }
    }
    build_csr();
  }

  std::size_t rows() const { return rows_; }
  std::size_t cols() const { return cols_; }
  std::size_t nnz() const { return triplets_.size(); }

  void set(std::size_t r, std::size_t c, const T& value) {
    if (r >= rows_ || c >= cols_) throw std::out_of_range("SparseMatrix::set");
    if (value == T{}) {
      erase(r, c);
    } else {
      auto it = find_triplet(r, c);
      if (it != triplets_.end()) {
        it->value = value;
      } else {
        auto pos = lower_bound_triplet(r, c);
        triplets_.insert(pos, {r, c, value});
      }
      dirty_ = true;
    }
  }

  T get(std::size_t r, std::size_t c) const {
    if (r >= rows_ || c >= cols_) throw std::out_of_range("SparseMatrix::get");
    ensure_csr();
    for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
      if (col_idx_[i] == c) return values_[i];
    }
    return T{};
  }

  std::vector<T> multiply(const std::vector<T>& x) const {
    if (x.size() != cols_) throw std::invalid_argument("dimension mismatch");
    ensure_csr();
    std::vector<T> result(rows_, T{});
    for (std::size_t r = 0; r < rows_; ++r) {
      for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
        result[r] += values_[i] * x[col_idx_[i]];
      }
    }
    return result;
  }

  SparseMatrix transpose() const {
    ensure_csr();
    SparseMatrix result(cols_, rows_);
    result.row_ptr_.resize(cols_ + 1, 0);
    for (std::size_t i = 0; i < col_idx_.size(); ++i) {
      result.row_ptr_[col_idx_[i] + 1]++;
    }
    for (std::size_t c = 1; c <= cols_; ++c) {
      result.row_ptr_[c] += result.row_ptr_[c - 1];
    }
    auto saved_row_ptr = result.row_ptr_;
    result.col_idx_.resize(nnz());
    result.values_.resize(nnz());
    for (std::size_t r = 0; r < rows_; ++r) {
      for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
        std::size_t c = col_idx_[i];
        std::size_t pos = result.row_ptr_[c]++;
        result.col_idx_[pos] = r;
        result.values_[pos] = values_[i];
      }
    }
    result.row_ptr_ = saved_row_ptr;
    result.triplets_.reserve(nnz());
    for (std::size_t c = 0; c < cols_; ++c) {
      for (std::size_t i = saved_row_ptr[c]; i < saved_row_ptr[c + 1]; ++i) {
        result.triplets_.push_back({c, result.col_idx_[i], result.values_[i]});
      }
    }
    result.dirty_ = false;
    return result;
  }

  std::vector<Triplet> to_triplets() const { return triplets_; }

  std::vector<T> to_dense() const {
    ensure_csr();
    std::vector<T> dense(rows_ * cols_, T{});
    for (std::size_t r = 0; r < rows_; ++r) {
      for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
        dense[r * cols_ + col_idx_[i]] = values_[i];
      }
    }
    return dense;
  }

  static SparseMatrix add(const SparseMatrix& a, const SparseMatrix& b) {
    if (a.rows_ != b.rows_ || a.cols_ != b.cols_)
      throw std::invalid_argument("dimension mismatch");
    SparseMatrix result(a.rows_, a.cols_);
    for (const auto& t : a.triplets_) result.set(t.row, t.col, t.value);
    for (const auto& t : b.triplets_) {
      T existing = result.get(t.row, t.col);
      result.set(t.row, t.col, existing + t.value);
    }
    return result;
  }

 private:
  void ensure_csr() const {
    if (dirty_) {
      const_cast<SparseMatrix*>(this)->build_csr();
    }
  }

  void build_csr() {
    row_ptr_.assign(rows_ + 1, 0);
    col_idx_.clear();
    values_.clear();
    for (const auto& t : triplets_) {
      row_ptr_[t.row + 1]++;
    }
    for (std::size_t r = 1; r <= rows_; ++r) {
      row_ptr_[r] += row_ptr_[r - 1];
    }
    col_idx_.resize(triplets_.size());
    values_.resize(triplets_.size());
    std::vector<std::size_t> pos(rows_, 0);
    for (const auto& t : triplets_) {
      std::size_t p = row_ptr_[t.row] + pos[t.row]++;
      col_idx_[p] = t.col;
      values_[p] = t.value;
    }
    dirty_ = false;
  }

  typename std::vector<Triplet>::iterator lower_bound_triplet(
      std::size_t r, std::size_t c) {
    return std::lower_bound(
        triplets_.begin(), triplets_.end(), Triplet{r, c, T{}},
        [](const Triplet& a, const Triplet& b) {
          return a.row < b.row || (a.row == b.row && a.col < b.col);
        });
  }

  typename std::vector<Triplet>::iterator find_triplet(
      std::size_t r, std::size_t c) {
    return std::find_if(triplets_.begin(), triplets_.end(),
                        [r, c](const Triplet& t) {
                          return t.row == r && t.col == c;
                        });
  }

  void erase(std::size_t r, std::size_t c) {
    auto it = find_triplet(r, c);
    if (it != triplets_.end()) {
      triplets_.erase(it);
      dirty_ = true;
    }
  }

  std::size_t rows_ = 0;
  std::size_t cols_ = 0;
  std::vector<Triplet> triplets_;

  mutable std::vector<std::size_t> row_ptr_;
  mutable std::vector<std::size_t> col_idx_;
  mutable std::vector<T> values_;
  mutable bool dirty_ = true;
};

}  // namespace dsa
