#pragma once
#include <algorithm>
#include <memory>
#include <vector>

namespace dsa {

// Interval tree for stabbing queries (Brass Ch4 §4.1)
// Stores closed intervals [lo, hi]. Query(p) returns all intervals containing p.
template <typename T = int>
class interval_tree {
public:
    struct interval {
        T lo, hi;
        int id;
    };

    interval_tree() = default;

    explicit interval_tree(std::vector<interval> segs) : segs_(std::move(segs)) {
        if (!segs_.empty()) {
            std::sort(segs_.begin(), segs_.end(),
                      [](const interval& a, const interval& b) { return a.lo < b.lo; });
            root_ = build(0, static_cast<int>(segs_.size()) - 1);
        }
    }

    // Query: find all intervals containing point p
    std::vector<interval> query(T p) const {
        std::vector<interval> result;
        query(root_.get(), p, result);
        return result;
    }

    std::size_t size() const { return segs_.size(); }
    bool empty() const { return segs_.empty(); }

private:
    std::vector<interval> segs_;

    struct node {
        int idx;
        T max_end;
        std::unique_ptr<node> left, right;
    };

    std::unique_ptr<node> root_;

    std::unique_ptr<node> build(int lo, int hi) {
        if (lo > hi) return nullptr;
        int mid = lo + (hi - lo) / 2;
        auto n = std::make_unique<node>();
        n->idx = mid;
        n->left = build(lo, mid - 1);
        n->right = build(mid + 1, hi);
        n->max_end = segs_[mid].hi;
        if (n->left) n->max_end = std::max(n->max_end, n->left->max_end);
        if (n->right) n->max_end = std::max(n->max_end, n->right->max_end);
        return n;
    }

    void query(const node* n, T p, std::vector<interval>& result) const {
        if (!n) return;
        if (n->left && n->left->max_end >= p) query(n->left.get(), p, result);
        const auto& seg = segs_[n->idx];
        if (seg.lo <= p && p <= seg.hi) result.push_back(seg);
        if (seg.lo <= p && n->right) query(n->right.get(), p, result);
    }
};

}  // namespace dsa
