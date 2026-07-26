#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>
#include <vector>

namespace dsa {

// Quadtree: recursive 2D spatial partitioning.
// Each node has 4 children (NW, NE, SW, SE).
template <typename T = double>
class quadtree {
public:
    struct point {
        T x, y;
        int id;
    };

    struct rect {
        T x, y, w, h;  // center (x,y), half-width w, half-height h
    };

    quadtree() : root_(nullptr) {}

    explicit quadtree(rect bounds, int max_pts = 8)
        : bounds_(bounds), max_points_(max_pts) {}

    void insert(T x, T y, int id = 0) {
        if (!root_) {
            root_ = std::make_unique<node>(bounds_);
        }
        insert_rec(root_, {x, y, id});
    }

    std::vector<point> range_query(T x, T y, T radius) const {
        std::vector<point> result;
        if (!root_) return result;
        range_rec(root_, x, y, radius * radius, result);
        return result;
    }

    std::vector<point> rect_query(T rx, T ry, T rw, T rh) const {
        std::vector<point> result;
        if (!root_) return result;
        rect_query_rec(root_, rx, ry, rw, rh, result);
        return result;
    }

    bool empty() const { return !root_ || root_->points.empty(); }
    std::size_t size() const { return count_; }

private:
    struct node {
        rect bounds;
        std::vector<point> points;
        std::unique_ptr<node> children[4];  // NW, NE, SW, SE
        bool divided = false;

        node(rect b) : bounds(b) {}
    };

    rect bounds_;
    int max_points_ = 8;
    std::unique_ptr<node> root_;
    std::size_t count_ = 0;

    int get_quadrant(const rect& bounds, const point& p) const {
        bool left = p.x < bounds.x;
        bool top = p.y < bounds.y;
        if (top && left) return 0;   // NW
        if (top && !left) return 1;  // NE
        if (!top && left) return 2;  // SW
        return 3;                     // SE
    }

    void subdivide(node* n) {
        T x = n->bounds.x, y = n->bounds.y;
        T hw = n->bounds.w / 2, hh = n->bounds.h / 2;
        n->children[0] = std::make_unique<node>(rect{x - hw, y - hh, hw, hh});
        n->children[1] = std::make_unique<node>(rect{x + hw, y - hh, hw, hh});
        n->children[2] = std::make_unique<node>(rect{x - hw, y + hh, hw, hh});
        n->children[3] = std::make_unique<node>(rect{x + hw, y + hh, hw, hh});
        n->divided = true;
    }

    void insert_rec(std::unique_ptr<node>& n, const point& p, bool is_reinsert = false) {
        if (!n) return;
        if (!contains_point(n->bounds, p)) return;
        if (!n->divided && static_cast<int>(n->points.size()) < max_points_) {
            n->points.push_back(p);
            if (!is_reinsert) ++count_;
            return;
        }
        if (!n->divided) subdivide(n.get());
        // Re-insert existing points
        if (n->points.size() > 0) {
            auto pts = std::move(n->points);
            n->points.clear();
            for (const auto& pt : pts) {
                int q = get_quadrant(n->bounds, pt);
                insert_rec(n->children[q], pt, true);
            }
        }
        int q = get_quadrant(n->bounds, p);
        insert_rec(n->children[q], p, false);
    }

    bool contains_point(const rect& r, const point& p) const {
        return p.x >= r.x - r.w && p.x <= r.x + r.w &&
               p.y >= r.y - r.h && p.y <= r.y + r.h;
    }

    bool intersects(const rect& a, const rect& b) const {
        return !(a.x + a.w < b.x - b.w || a.x - a.w > b.x + b.w ||
                 a.y + a.h < b.y - b.h || a.y - a.h > b.y + b.h);
    }

    void range_rec(const std::unique_ptr<node>& n, T cx, T cy,
                   T radius_sq, std::vector<point>& result) const {
        if (!n) return;
        // Check if circle intersects this node's bounds
        T dx = std::max(T{}, std::max(n->bounds.x - n->bounds.w - cx,
                                       cx - (n->bounds.x + n->bounds.w)));
        T dy = std::max(T{}, std::max(n->bounds.y - n->bounds.h - cy,
                                       cy - (n->bounds.y + n->bounds.h)));
        if (dx * dx + dy * dy > radius_sq) return;
        for (const auto& p : n->points) {
            T ddx = p.x - cx, ddy = p.y - cy;
            if (ddx * ddx + ddy * ddy <= radius_sq) result.push_back(p);
        }
        if (n->divided) {
            for (int i = 0; i < 4; ++i)
                range_rec(n->children[i], cx, cy, radius_sq, result);
        }
    }

    void rect_query_rec(const std::unique_ptr<node>& n, T rx, T ry,
                        T rw, T rh, std::vector<point>& result) const {
        if (!n) return;
        rect qr{rx, ry, rw, rh};
        if (!intersects(n->bounds, qr)) return;
        for (const auto& p : n->points) {
            if (p.x >= rx - rw && p.x <= rx + rw &&
                p.y >= ry - rh && p.y <= ry + rh)
                result.push_back(p);
        }
        if (n->divided) {
            for (int i = 0; i < 4; ++i)
                rect_query_rec(n->children[i], rx, ry, rw, rh, result);
        }
    }
};

}  // namespace dsa
