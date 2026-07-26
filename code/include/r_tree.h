#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>

namespace dsa {

// R-tree: spatial index for multi-dimensional objects (Guttman 1984).
// Simplified implementation: brute-force with bounding box prefilter.
// Template parameter D = dimensions.
template <int D, typename T = double>
class r_tree {
public:
    struct point {
        std::array<T, D> coords;
        int id;
    };

    struct rect {
        std::array<T, D> lo, hi;
    };

    r_tree() = default;

    void insert(point p) {
        points_.push_back(p);
    }

    std::vector<point> range_query(const rect& q) const {
        std::vector<point> result;
        for (const auto& p : points_) {
            if (contains(q, p.coords))
                result.push_back(p);
        }
        return result;
    }

    point nearest(const std::array<T, D>& target) const {
        T best_dist = std::numeric_limits<T>::max();
        int best_id = -1;
        for (const auto& p : points_) {
            T d = dist_sq(p.coords, target);
            if (d < best_dist) { best_dist = d; best_id = p.id; }
        }
        return points_[best_id];
    }

    bool empty() const { return points_.empty(); }
    std::size_t size() const { return points_.size(); }

private:
    std::vector<point> points_;

    T dist_sq(const std::array<T, D>& a, const std::array<T, D>& b) const {
        T d = T{};
        for (int i = 0; i < D; ++i) d += (a[i] - b[i]) * (a[i] - b[i]);
        return d;
    }

    bool contains(const rect& r, const std::array<T, D>& p) const {
        for (int i = 0; i < D; ++i)
            if (p[i] < r.lo[i] || p[i] > r.hi[i]) return false;
        return true;
    }
};

}  // namespace dsa
