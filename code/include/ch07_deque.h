#pragma once
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ds {

template <typename T>
class Deque {
 public:
  Deque() : data_(kBlockSize), begin_(0), end_(0), size_(0) {}

  explicit Deque(std::size_t count, const T& value = T{})
      : data_(kBlockSize), begin_(0), end_(0), size_(0) {
    for (std::size_t i = 0; i < count; ++i) push_back(value);
  }

  template <typename It>
  Deque(It first, It last) : data_(kBlockSize), begin_(0), end_(0), size_(0) {
    for (; first != last; ++first) push_back(*first);
  }

  Deque(const Deque& other)
      : data_(other.size_), begin_(0), end_(other.size_), size_(other.size_) {
    std::uninitialized_copy(other.data_.begin(),
                           other.data_.begin() + other.size_,
                           data_.begin());
  }

  Deque& operator=(const Deque& other) {
    if (this != &other) {
      clear();
      if (other.size_ > capacity()) {
        grow(other.size_);
      }
      std::uninitialized_copy(other.data_.begin(),
                             other.data_.begin() + other.size_,
                             data_.begin());
      begin_ = 0;
      end_ = other.size_;
      size_ = other.size_;
    }
    return *this;
  }

  Deque(Deque&& other) noexcept
      : data_(std::move(other.data_)),
        begin_(other.begin_),
        end_(other.end_),
        size_(other.size_) {
    other.begin_ = 0;
    other.end_ = 0;
    other.size_ = 0;
  }

  Deque& operator=(Deque&& other) noexcept {
    if (this != &other) {
      clear();
      data_ = std::move(other.data_);
      begin_ = other.begin_;
      end_ = other.end_;
      size_ = other.size_;
      other.begin_ = 0;
      other.end_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  ~Deque() { clear(); }

  T& operator[](std::size_t idx) { return data_[mod(begin_ + idx)]; }
  const T& operator[](std::size_t idx) const { return data_[mod(begin_ + idx)]; }

  T& at(std::size_t idx) {
    if (idx >= size_) throw std::out_of_range("Deque::at");
    return data_[mod(begin_ + idx)];
  }
  const T& at(std::size_t idx) const {
    if (idx >= size_) throw std::out_of_range("Deque::at");
    return data_[mod(begin_ + idx)];
  }

  T& front() { return data_[begin_]; }
  const T& front() const { return data_[begin_]; }
  T& back() { return data_[mod(end_ - 1)]; }
  const T& back() const { return data_[mod(end_ - 1)]; }

  bool empty() const { return size_ == 0; }
  std::size_t size() const { return size_; }
  std::size_t capacity() const { return data_.size(); }

  void push_back(const T& value) {
    if (size_ == capacity()) grow();
    data_[end_] = value;
    end_ = mod(end_ + 1);
    ++size_;
  }

  void push_back(T&& value) {
    if (size_ == capacity()) grow();
    data_[end_] = std::move(value);
    end_ = mod(end_ + 1);
    ++size_;
  }

  void push_front(const T& value) {
    if (size_ == capacity()) grow();
    begin_ = mod(begin_ - 1);
    data_[begin_] = value;
    ++size_;
  }

  void push_front(T&& value) {
    if (size_ == capacity()) grow();
    begin_ = mod(begin_ - 1);
    data_[begin_] = std::move(value);
    ++size_;
  }

  void pop_back() {
    if (empty()) throw std::runtime_error("pop_back on empty Deque");
    end_ = mod(end_ - 1);
    data_[end_].~T();
    --size_;
  }

  void pop_front() {
    if (empty()) throw std::runtime_error("pop_front on empty Deque");
    data_[begin_].~T();
    begin_ = mod(begin_ + 1);
    --size_;
  }

  void insert(std::size_t idx, const T& value) {
    if (idx > size_) throw std::out_of_range("Deque::insert");
    if (idx == 0) { push_front(value); return; }
    if (idx == size_) { push_back(value); return; }
    if (size_ == capacity()) grow();
    if (idx <= size_ / 2) {
      push_front(data_[mod(begin_)]);
      for (std::size_t i = 0; i < idx - 1; ++i) {
        data_[mod(begin_ + i)] = data_[mod(begin_ + i + 2)];
      }
      data_[mod(begin_ + idx)] = value;
    } else {
      push_back(data_[mod(end_ - 1)]);
      for (std::size_t i = size_ - 1; i > idx; --i) {
        data_[mod(begin_ + i)] = data_[mod(begin_ + i)];
      }
      data_[mod(begin_ + idx)] = value;
    }
  }

  void erase(std::size_t idx) {
    if (idx >= size_) throw std::out_of_range("Deque::erase");
    if (idx == 0) { pop_front(); return; }
    if (idx == size_ - 1) { pop_back(); return; }
    if (idx < size_ / 2) {
      for (std::size_t i = idx; i > 0; --i) {
        data_[mod(begin_ + i)] = std::move(data_[mod(begin_ + i - 1)]);
      }
      data_[begin_].~T();
      begin_ = mod(begin_ + 1);
    } else {
      for (std::size_t i = idx; i < size_ - 1; ++i) {
        data_[mod(begin_ + i)] = std::move(data_[mod(begin_ + i + 1)]);
      }
      end_ = mod(end_ - 1);
      data_[end_].~T();
    }
    --size_;
  }

  void clear() {
    for (std::size_t i = 0; i < size_; ++i) {
      data_[mod(begin_ + i)].~T();
    }
    begin_ = 0;
    end_ = 0;
    size_ = 0;
  }

 private:
  static constexpr std::size_t kBlockSize = 8;

  std::size_t mod(std::size_t idx) const { return idx % data_.size(); }

  void grow(std::size_t min_capacity = 0) {
    std::size_t new_cap = std::max(min_capacity, data_.size() * 2);
    std::vector<T> new_data(new_cap);
    for (std::size_t i = 0; i < size_; ++i) {
      new_data[i] = std::move(data_[mod(begin_ + i)]);
    }
    data_ = std::move(new_data);
    begin_ = 0;
    end_ = size_;
  }

  std::vector<T> data_;
  std::size_t begin_;
  std::size_t end_;
  std::size_t size_;
};

}  // namespace ds
