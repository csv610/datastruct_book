#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace dsa {

class BitVector {
 public:
  explicit BitVector(std::size_t size = 0)
      : size_(size), blocks_((size + kBitsPerBlock - 1) / kBitsPerBlock, 0) {}

  bool get(std::size_t idx) const {
    if (idx >= size_) throw std::out_of_range("BitVector::get");
    return (blocks_[idx / kBitsPerBlock] >> (idx % kBitsPerBlock)) & 1;
  }

  void set(std::size_t idx, bool value = true) {
    if (idx >= size_) throw std::out_of_range("BitVector::set");
    if (value) {
      blocks_[idx / kBitsPerBlock] |= (1ULL << (idx % kBitsPerBlock));
    } else {
      blocks_[idx / kBitsPerBlock] &= ~(1ULL << (idx % kBitsPerBlock));
    }
  }

  void flip(std::size_t idx) {
    if (idx >= size_) throw std::out_of_range("BitVector::flip");
    blocks_[idx / kBitsPerBlock] ^= (1ULL << (idx % kBitsPerBlock));
  }

  void reset(std::size_t idx) { set(idx, false); }

  std::size_t size() const { return size_; }

  std::size_t count() const {
    std::size_t result = 0;
    for (auto block : blocks_) result += __builtin_popcountll(block);
    return result;
  }

  bool any() const { return count() > 0; }
  bool none() const { return count() == 0; }
  bool all() const { return count() == size_; }

  std::size_t find_first() const {
    for (std::size_t i = 0; i < blocks_.size(); ++i) {
      if (blocks_[i] != 0) {
        return i * kBitsPerBlock + __builtin_ctzll(blocks_[i]);
      }
    }
    return size_;
  }

  std::size_t find_next(std::size_t from) const {
    if (from >= size_) return size_;
    std::size_t block_idx = from / kBitsPerBlock;
    std::size_t bit_idx = from % kBitsPerBlock;
    std::uint64_t mask = (bit_idx + 1 < kBitsPerBlock) ? ~((1ULL << (bit_idx + 1)) - 1) : 0;
    if (blocks_[block_idx] & mask) {
      return block_idx * kBitsPerBlock + __builtin_ctzll(blocks_[block_idx] & mask);
    }
    for (std::size_t i = block_idx + 1; i < blocks_.size(); ++i) {
      if (blocks_[i] != 0) {
        return i * kBitsPerBlock + __builtin_ctzll(blocks_[i]);
      }
    }
    return size_;
  }

  BitVector operator&(const BitVector& other) const {
    BitVector result(std::min(size_, other.size_));
    for (std::size_t i = 0; i < result.blocks_.size(); ++i) {
      result.blocks_[i] = blocks_[i] & other.blocks_[i];
    }
    return result;
  }

  BitVector operator|(const BitVector& other) const {
    BitVector result(std::max(size_, other.size_));
    std::size_t min_blocks = std::min(blocks_.size(), other.blocks_.size());
    for (std::size_t i = 0; i < min_blocks; ++i) {
      result.blocks_[i] = blocks_[i] | other.blocks_[i];
    }
    for (std::size_t i = min_blocks; i < blocks_.size(); ++i) {
      result.blocks_[i] = blocks_[i];
    }
    for (std::size_t i = min_blocks; i < other.blocks_.size(); ++i) {
      result.blocks_[i] = other.blocks_[i];
    }
    return result;
  }

  BitVector operator^(const BitVector& other) const {
    BitVector result(std::max(size_, other.size_));
    std::size_t min_blocks = std::min(blocks_.size(), other.blocks_.size());
    for (std::size_t i = 0; i < min_blocks; ++i) {
      result.blocks_[i] = blocks_[i] ^ other.blocks_[i];
    }
    for (std::size_t i = min_blocks; i < blocks_.size(); ++i) {
      result.blocks_[i] = blocks_[i];
    }
    for (std::size_t i = min_blocks; i < other.blocks_.size(); ++i) {
      result.blocks_[i] = other.blocks_[i];
    }
    return result;
  }

  BitVector operator~() const {
    BitVector result(size_);
    for (std::size_t i = 0; i < blocks_.size(); ++i) {
      result.blocks_[i] = ~blocks_[i];
    }
    if (size_ % kBitsPerBlock != 0) {
      std::uint64_t mask = (1ULL << (size_ % kBitsPerBlock)) - 1;
      result.blocks_.back() &= mask;
    }
    return result;
  }

  bool operator==(const BitVector& other) const {
    if (size_ != other.size_) return false;
    return blocks_ == other.blocks_;
  }

  bool operator!=(const BitVector& other) const { return !(*this == other); }

 private:
  static constexpr std::size_t kBitsPerBlock = 64;
  std::size_t size_ = 0;
  std::vector<std::uint64_t> blocks_;
};

}  // namespace dsa
