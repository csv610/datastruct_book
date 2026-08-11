#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace dsa {

class wavelet_tree {
public:
    explicit wavelet_tree(std::string text) : text_(std::move(text)) {
        if (!text_.empty()) root_ = build(text_, 0, 255);
    }

    std::size_t size() const noexcept { return text_.size(); }
    char access(std::size_t index) const { if (index >= size()) throw std::out_of_range("access"); return text_[index]; }

    std::size_t rank(char symbol, std::size_t end) const {
        if (end > size()) throw std::out_of_range("rank");
        return rank(root_.get(), static_cast<unsigned char>(symbol), end, 0, 255);
    }

    std::size_t select(char symbol, std::size_t occurrence) const {
        std::size_t lo = 0, hi = size();
        while (lo < hi) {
            const auto mid = lo + (hi - lo) / 2;
            if (rank(symbol, mid + 1) > occurrence) hi = mid;
            else lo = mid + 1;
        }
        if (lo == size() || rank(symbol, lo + 1) <= occurrence) throw std::out_of_range("select");
        return lo;
    }

    std::size_t range_count(std::size_t left, std::size_t right,
                            char low, char high) const {
        if (left > right || right > size()) throw std::out_of_range("range");
        return range_count(root_.get(), left, right,
                           static_cast<unsigned char>(low),
                           static_cast<unsigned char>(high), 0, 255);
    }

private:
    struct node {
        unsigned char lo, hi;
        std::vector<std::size_t> prefix_zero;
        std::unique_ptr<node> left, right;
    };

    static std::unique_ptr<node> build(const std::string& values, unsigned char lo, unsigned char hi) {
        auto result = std::make_unique<node>(); result->lo = lo; result->hi = hi;
        result->prefix_zero.push_back(0);
        if (lo == hi) { for (std::size_t i = 0; i < values.size(); ++i) result->prefix_zero.push_back(i + 1); return result; }
        const auto mid = static_cast<unsigned char>(lo + (hi - lo) / 2);
        std::string left_values, right_values;
        for (unsigned char c : values) {
            const bool zero = c <= mid;
            result->prefix_zero.push_back(result->prefix_zero.back() + zero);
            (zero ? left_values : right_values).push_back(static_cast<char>(c));
        }
        if (!left_values.empty()) result->left = build(left_values, lo, mid);
        if (!right_values.empty()) result->right = build(right_values, static_cast<unsigned char>(mid + 1), hi);
        return result;
    }

    static std::size_t rank(const node* n, unsigned char symbol, std::size_t end,
                            unsigned char lo, unsigned char hi) {
        if (!n || end == 0) return 0;
        if (lo == hi) return end;
        const auto mid = static_cast<unsigned char>(lo + (hi - lo) / 2);
        const auto zeros = n->prefix_zero[end];
        if (symbol <= mid) return rank(n->left.get(), symbol, zeros, lo, mid);
        return rank(n->right.get(), symbol, end - zeros, static_cast<unsigned char>(mid + 1), hi);
    }

    static std::size_t range_count(const node* n, std::size_t left, std::size_t right,
                                   unsigned char qlo, unsigned char qhi,
                                   unsigned char lo, unsigned char hi) {
        if (!n || right <= left || qhi < lo || hi < qlo) return 0;
        if (qlo <= lo && hi <= qhi) return right - left;
        const auto mid = static_cast<unsigned char>(lo + (hi - lo) / 2);
        const auto zl = n->prefix_zero[left], zr = n->prefix_zero[right];
        return range_count(n->left.get(), zl, zr, qlo, qhi, lo, mid) +
               range_count(n->right.get(), left - zl, right - zr, qlo, qhi,
                           static_cast<unsigned char>(mid + 1), hi);
    }

    std::string text_;
    std::unique_ptr<node> root_;
};

} // namespace dsa
