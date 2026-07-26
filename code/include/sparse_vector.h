#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T>
class SparseVector {
 public:
  struct Entry {
    std::size_t index;
    T value;
  };

  SparseVector() = default;
  explicit SparseVector(std::size_t size) : size_(size) {}

  std::size_t size() const { return size_; }
  std::size_t nnz() const { return entries_.size(); }
  bool empty() const { return entries_.empty(); }

  void set(std::size_t idx, const T& value) {
    if (idx >= size_) throw std::out_of_range("SparseVector::set");
    auto it = std::find_if(entries_.begin(), entries_.end(),
                           [idx](const Entry& e) { return e.index == idx; });
    if (value == T{}) {
      if (it != entries_.end()) entries_.erase(it);
    } else {
      if (it != entries_.end()) {
        it->value = value;
      } else {
        entries_.push_back({idx, value});
        std::sort(entries_.begin(), entries_.end(),
                  [](const Entry& a, const Entry& b) { return a.index < b.index; });
      }
    }
  }

  T get(std::size_t idx) const {
    if (idx >= size_) throw std::out_of_range("SparseVector::get");
    for (const auto& e : entries_) {
      if (e.index == idx) return e.value;
      if (e.index > idx) break;
    }
    return T{};
  }

  std::vector<Entry> to_entries() const { return entries_; }

  std::vector<T> to_dense() const {
    std::vector<T> dense(size_, T{});
    for (const auto& e : entries_) dense[e.index] = e.value;
    return dense;
  }

  T dot(const SparseVector& other) const {
    if (size_ != other.size_) throw std::invalid_argument("dimension mismatch");
    T result{};
    std::size_t i = 0, j = 0;
    while (i < entries_.size() && j < other.entries_.size()) {
      if (entries_[i].index == other.entries_[j].index) {
        result += entries_[i].value * other.entries_[j].value;
        ++i; ++j;
      } else if (entries_[i].index < other.entries_[j].index) {
        ++i;
      } else {
        ++j;
      }
    }
    return result;
  }

  SparseVector operator+(const SparseVector& other) const {
    if (size_ != other.size_) throw std::invalid_argument("dimension mismatch");
    SparseVector result(size_);
    std::size_t i = 0, j = 0;
    while (i < entries_.size() || j < other.entries_.size()) {
      if (j == other.entries_.size() || (i < entries_.size() && entries_[i].index < other.entries_[j].index)) {
        result.entries_.push_back(entries_[i++]);
      } else if (i == entries_.size() || entries_[i].index > other.entries_[j].index) {
        result.entries_.push_back(other.entries_[j++]);
      } else {
        T sum = entries_[i].value + other.entries_[j].value;
        if (sum != T{}) result.entries_.push_back({entries_[i].index, sum});
        ++i; ++j;
      }
    }
    return result;
  }

  SparseVector operator-(const SparseVector& other) const {
    if (size_ != other.size_) throw std::invalid_argument("dimension mismatch");
    SparseVector result(size_);
    std::size_t i = 0, j = 0;
    while (i < entries_.size() || j < other.entries_.size()) {
      if (j == other.entries_.size() || (i < entries_.size() && entries_[i].index < other.entries_[j].index)) {
        result.entries_.push_back(entries_[i++]);
      } else if (i == entries_.size() || entries_[i].index > other.entries_[j].index) {
        result.entries_.push_back({other.entries_[j].index, -other.entries_[j].value});
        ++j;
      } else {
        T diff = entries_[i].value - other.entries_[j].value;
        if (diff != T{}) result.entries_.push_back({entries_[i].index, diff});
        ++i; ++j;
      }
    }
    return result;
  }

  SparseVector scale(const T& scalar) const {
    SparseVector result(size_);
    for (const auto& e : entries_) {
      result.entries_.push_back({e.index, e.value * scalar});
    }
    return result;
  }

  T norm_squared() const {
    T sum{};
    for (const auto& e : entries_) sum += e.value * e.value;
    return sum;
  }

 private:
  std::size_t size_ = 0;
  std::vector<Entry> entries_;
};

}  // namespace dsa
