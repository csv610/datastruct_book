#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <numeric>
#include <vector>

namespace dsa {

// k-d tree for multi-dimensional point sets (Brass Ch4 §4.10)
// Supports nearest-neighbor and range queries in k dimensions.
template <int K, typename T = double>
class kd_tree {
public:
    struct point {
        std::array<T, K> coords;
        int id;
    };

    kd_tree() = default;

    explicit kd_tree(std::vector<point> pts) : points_(std::move(pts)) {
        if (!points_.empty()) {
            std::vector<int> indices(points_.size());
            std::iota(indices.begin(), indices.end(), 0);
            root_ = build(indices, 0, 0, static_cast<int>(indices.size()) - 1);
        }
    }

    // Nearest neighbor query: returns (point, squared_distance)
    std::pair<point, T> nearest(const std::array<T, K>& target) const {
        T best_dist = std::numeric_limits<T>::max();
        int best_id = -1;
        search_nearest(root_.get(), target, 0, best_dist, best_id);
        return {points_[best_id], best_dist};
    }

    // Range query: find all points within squared distance r of target
    std::vector<point> range_query(const std::array<T, K>& target, T radius) const {
        std::vector<point> result;
        range_search(root_.get(), target, 0, radius * radius, result);
        return result;
    }

    bool empty() const { return points_.empty(); }
    std::size_t size() const { return points_.size(); }

private:
    std::vector<point> points_;

    struct node {
        int point_idx = -1;
        int axis = 0;
        T split_val{};
        std::unique_ptr<node> left, right;
    };

    std::unique_ptr<node> root_;

    std::unique_ptr<node> build(std::vector<int>& indices, int depth, int lo, int hi) {
        if (lo > hi) return nullptr;
        int axis = depth % K;
        int mid = lo + (hi - lo) / 2;
        std::nth_element(indices.begin() + lo, indices.begin() + mid,
                          indices.begin() + hi + 1,
                          [&](int a, int b) {
                              return points_[a].coords[axis] < points_[b].coords[axis];
                          });
        auto n = std::make_unique<node>();
        n->axis = axis;
        n->point_idx = indices[mid];
        n->split_val = points_[indices[mid]].coords[axis];
        n->left = build(indices, depth + 1, lo, mid - 1);
        n->right = build(indices, depth + 1, mid + 1, hi);
        return n;
    }

    T dist_sq(const std::array<T, K>& a, const std::array<T, K>& b) const {
        T d = T{};
        for (int i = 0; i < K; ++i) d += (a[i] - b[i]) * (a[i] - b[i]);
        return d;
    }

    void search_nearest(node* n, const std::array<T, K>& target, int depth,
                        T& best_dist, int& best_id) const {
        if (!n) return;
        T d = dist_sq(points_[n->point_idx].coords, target);
        if (d < best_dist) {
            best_dist = d;
            best_id = n->point_idx;
        }
        int axis = n->axis;
        T diff = target[axis] - n->split_val;
        node* first = diff < 0 ? n->left.get() : n->right.get();
        node* second = diff < 0 ? n->right.get() : n->left.get();
        search_nearest(first, target, depth + 1, best_dist, best_id);
        if (diff * diff < best_dist) {
            search_nearest(second, target, depth + 1, best_dist, best_id);
        }
    }

    void range_search(node* n, const std::array<T, K>& target, int depth,
                      T radius_sq, std::vector<point>& result) const {
        if (!n) return;
        T d = dist_sq(points_[n->point_idx].coords, target);
        if (d <= radius_sq) result.push_back(points_[n->point_idx]);
        int axis = n->axis;
        T diff = target[axis] - n->split_val;
        node* first = diff < 0 ? n->left.get() : n->right.get();
        node* second = diff < 0 ? n->right.get() : n->left.get();
        range_search(first, target, depth + 1, radius_sq, result);
        if (diff * diff <= radius_sq) {
            range_search(second, target, depth + 1, radius_sq, result);
        }
    }
};

}  // namespace dsa
