#pragma once
#include <algorithm>
#include <vector>

namespace dsa {

// 2D Orthogonal Range Tree for range reporting (Brass Ch4 §4.6)
// Supports querying all points in an axis-aligned rectangle [xlo,xhi] x [ylo,yhi].
// Uses a segment tree on x-coordinate where each node stores a sorted-by-y vector.
template <typename T = int>
class orthogonal_range_tree {
public:
    struct point {
        T x, y;
        int id;
    };

    orthogonal_range_tree() = default;

    explicit orthogonal_range_tree(std::vector<point> pts) : pts_(std::move(pts)) {
        if (pts_.empty()) return;
        std::sort(pts_.begin(), pts_.end(),
                  [](const point& a, const point& b) { return a.x < b.x; });
        // Build associated y-sorted vectors using merge technique
        int n = static_cast<int>(pts_.size());
        ytree_.assign(4 * n, {});
        build(1, 0, n - 1);
    }

    // Query: all points in [xlo, xhi] x [ylo, yhi]
    std::vector<point> query(T xlo, T xhi, T ylo, T yhi) const {
        std::vector<point> result;
        if (pts_.empty()) return result;
        query(1, 0, static_cast<int>(pts_.size()) - 1, xlo, xhi, ylo, yhi, result);
        return result;
    }

    std::size_t size() const { return pts_.size(); }
    bool empty() const { return pts_.empty(); }

private:
    std::vector<point> pts_;
    std::vector<std::vector<point>> ytree_;

    void build(int node, int lo, int hi) {
        if (lo == hi) {
            ytree_[node] = {pts_[lo]};
            return;
        }
        int mid = lo + (hi - lo) / 2;
        build(2 * node, lo, mid);
        build(2 * node + 1, mid + 1, hi);
        // Merge sorted-by-y arrays
        ytree_[node].resize(ytree_[2 * node].size() + ytree_[2 * node + 1].size());
        std::merge(ytree_[2 * node].begin(), ytree_[2 * node].end(),
                   ytree_[2 * node + 1].begin(), ytree_[2 * node + 1].end(),
                   ytree_[node].begin(),
                   [](const point& a, const point& b) { return a.y < b.y; });
    }

    void query(int node, int lo, int hi, T xlo, T xhi, T ylo, T yhi,
               std::vector<point>& result) const {
        if (hi < lo) return;
        if (xhi < pts_[lo].x || pts_[hi].x < xlo) return;
        if (xlo <= pts_[lo].x && pts_[hi].x <= xhi) {
            // All points in this x-range qualify on x; filter on y
            const auto& vec = ytree_[node];
            auto it_lo = std::lower_bound(vec.begin(), vec.end(), ylo,
                [](const point& p, T v) { return p.y < v; });
            auto it_hi = std::upper_bound(vec.begin(), vec.end(), yhi,
                [](T v, const point& p) { return v < p.y; });
            result.insert(result.end(), it_lo, it_hi);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        query(2 * node, lo, mid, xlo, xhi, ylo, yhi, result);
        query(2 * node + 1, mid + 1, hi, xlo, xhi, ylo, yhi, result);
    }
};

}  // namespace dsa
