struct point { double x, y; };

class quadtree {
    struct rect {
        double x, y, w, h;
        bool contains(const point& p) const {
            return p.x >= x && p.x <= x + w
                && p.y >= y && p.y <= y + h;
        }
        bool intersects(const rect& other) const {
            return !(other.x > x + w || other.x + other.w < x
                  || other.y > y + h || other.y + other.h < y);
        }
    };

    struct node {
        rect bounds;
        std::vector<point> points;
        std::unique_ptr<node> children[4]; // NW, NE, SW, SE
        bool divided = false;

        node(const rect& b) : bounds(b) {}

        void subdivide() {
            double hw = bounds.w / 2, hh = bounds.h / 2;
            double x = bounds.x, y = bounds.y;
            children[0] = std::make_unique<node>(
                rect{x,      y + hh, hw, hh});  // NW
            children[1] = std::make_unique<node>(
                rect{x + hw, y + hh, hw, hh});  // NE
            children[2] = std::make_unique<node>(
                rect{x,      y,      hw, hh});  // SW
            children[3] = std::make_unique<node>(
                rect{x + hw, y,      hw, hh});  // SE
            divided = true;
        }
    };

    std::unique_ptr<node> root_;
    static constexpr int CAPACITY = 4;

    static void insert(node* n, const point& p) {
        if (!n->bounds.contains(p)) return;
        if (n->points.size() < CAPACITY && !n->divided) {
            n->points.push_back(p);
            return;
        }
        if (!n->divided) n->subdivide();
        for (int i = 0; i < 4; ++i) {
            if (n->children[i]->bounds.contains(p)) {
                insert(n->children[i].get(), p);
                return;
            }
        }
    }

    static void range_search(const node* n, const rect& range,
                              std::vector<point>& result) {
        if (!n || !n->bounds.intersects(range)) return;
        for (const auto& p : n->points) {
            if (range.contains(p)) result.push_back(p);
        }
        if (n->divided) {
            for (int i = 0; i < 4; ++i)
                range_search(n->children[i].get(), range, result);
        }
    }

    static void nearest(const node* n, const point& target,
                         point& best, double& best_dist) {
        if (!n) return;
        for (const auto& p : n->points) {
            double d = (p.x - target.x) * (p.x - target.x)
                     + (p.y - target.y) * (p.y - target.y);
            if (d < best_dist) { best_dist = d; best = p; }
        }
        if (!n->divided) return;
        for (int i = 0; i < 4; ++i) {
            const auto& b = n->children[i]->bounds;
            double cx = b.x + b.w / 2, cy = b.y + b.h / 2;
            double d = (cx - target.x) * (cx - target.x)
                     + (cy - target.y) * (cy - target.y);
            if (d < best_dist)
                nearest(n->children[i].get(), target, best, best_dist);
        }
    }

public:
    quadtree(const rect& bounds) {
        root_ = std::make_unique<node>(bounds);
    }

    void insert(const point& p) { insert(root_.get(), p); }

    std::vector<point> range_query(const rect& range) const {
        std::vector<point> result;
        range_search(root_.get(), range, result);
        return result;
    }

    std::optional<point> nearest_neighbor(const point& target) const {
        if (!root_ || root_->points.empty()) return std::nullopt;
        point best = root_->points[0];
        double best_dist = 1e18;
        nearest(root_.get(), target, best, best_dist);
        return best;
    }
};
