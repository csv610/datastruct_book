#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

namespace dsa {

class SuffixArray {
 public:
  explicit SuffixArray(const std::string& s) : text_(s + '\0') {
    build_sa();
    build_lcp();
  }

  const std::vector<std::size_t>& sa() const { return sa_; }
  const std::vector<std::size_t>& lcp() const { return lcp_; }

  std::vector<std::size_t> search(const std::string& pattern) const {
    std::vector<std::size_t> results;
    std::size_t n = text_.size() - 1;
    std::size_t m = pattern.size();
    if (m == 0 || m > n) return results;

    std::size_t lo = 0, hi = sa_.size();
    while (lo < hi) {
      std::size_t mid = lo + (hi - lo) / 2;
      if (compare_suffix(sa_[mid], pattern, n) < 0) lo = mid + 1;
      else hi = mid;
    }
    std::size_t start = lo;
    lo = 0;
    hi = sa_.size();
    while (lo < hi) {
      std::size_t mid = lo + (hi - lo) / 2;
      if (compare_suffix(sa_[mid], pattern, n) <= 0) lo = mid + 1;
      else hi = mid;
    }
    std::size_t end = lo;

    for (std::size_t i = start; i < end; ++i) {
      results.push_back(sa_[i]);
    }
    std::sort(results.begin(), results.end());
    return results;
  }

  std::size_t count_occurrences(const std::string& pattern) const {
    return search(pattern).size();
  }

  std::string longest_common_substring(const SuffixArray& other) const {
    std::size_t n1 = text_.size() - 1;
    std::size_t n2 = other.text_.size() - 1;
    std::string combined = text_ + other.text_.substr(0, n2);
    SuffixArray combined_sa(combined);

    std::size_t max_len = 0;
    std::size_t max_pos = 0;
    for (std::size_t i = 1; i < combined_sa.sa().size(); ++i) {
      std::size_t a = combined_sa.sa()[i - 1];
      std::size_t b = combined_sa.sa()[i];
      bool a_first = a < n1;
      bool b_first = b < n1;
      if (a_first != b_first) {
        std::size_t lcp = combined_sa.lcp()[i];
        if (lcp > max_len) {
          max_len = lcp;
          max_pos = a_first ? a : b;
        }
      }
    }
    return combined.substr(max_pos, max_len);
  }

 private:
  void build_sa() {
    std::size_t n = text_.size();
    sa_.resize(n);
    rank_.resize(n);
    tmp_.resize(n);

    for (std::size_t i = 0; i < n; ++i) {
      sa_[i] = i;
      rank_[i] = text_[i];
    }

    for (std::size_t gap = 1; gap < n; gap *= 2) {
      auto cmp = [this, gap, n](std::size_t a, std::size_t b) {
        if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
        std::size_t ra = (a + gap < n) ? rank_[a + gap] : -1;
        std::size_t rb = (b + gap < n) ? rank_[b + gap] : -1;
        return ra < rb;
      };
      std::sort(sa_.begin(), sa_.end(), cmp);

      tmp_[sa_[0]] = 0;
      for (std::size_t i = 1; i < n; ++i) {
        tmp_[sa_[i]] = tmp_[sa_[i - 1]] + (cmp(sa_[i - 1], sa_[i]) ? 1 : 0);
      }
      rank_ = tmp_;
      if (rank_[sa_[n - 1]] == n - 1) break;
    }
  }

  void build_lcp() {
    std::size_t n = sa_.size();
    lcp_.assign(n, 0);
    std::vector<std::size_t> inv_sa(n);
    for (std::size_t i = 0; i < n; ++i) inv_sa[sa_[i]] = i;

    std::size_t k = 0;
    for (std::size_t i = 0; i < n; ++i) {
      if (inv_sa[i] == 0) { k = 0; continue; }
      std::size_t j = sa_[inv_sa[i] - 1];
      while (i + k < n && j + k < n && text_[i + k] == text_[j + k]) ++k;
      lcp_[inv_sa[i]] = k;
      if (k > 0) --k;
    }
  }

  int compare_suffix(std::size_t sa_idx, const std::string& pattern,
                     std::size_t n) const {
    std::size_t i = sa_idx;
    std::size_t j = 0;
    while (i < n && j < pattern.size() && text_[i] == pattern[j]) {
      ++i; ++j;
    }
    if (j == pattern.size()) return 0;
    if (i == n) return -1;
    return text_[i] < pattern[j] ? -1 : 1;
  }

  std::string text_;
  std::vector<std::size_t> sa_;
  std::vector<std::size_t> lcp_;
  std::vector<std::size_t> rank_;
  std::vector<std::size_t> tmp_;
};

}  // namespace dsa
