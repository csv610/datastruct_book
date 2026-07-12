template <typename T, size_t K>
class kd_tree {
    struct point {
        std::array<T, K> coords;
        bool operator<(const point& other) const { return coords < other.coords; }
    };

    struct node {
        point pt;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;
        node(const point& p) : pt(p) {}
    };

    using node_ptr = std::unique_ptr<node>;
    node_ptr root_;

    node_ptr build(std::vector<point>& pts, int depth) {
        if (pts.empty()) return nullptr;
        size_t axis = depth % K;
        size_t mid = pts.size() / 2;
        std::nth_element(pts.begin(), pts.begin() + mid, pts.end(),
            [axis](const point& a, const point& b) {
                return a.coords[axis] < b.coords[axis];
            });
        auto n = std::make_unique<node>(pts[mid]);
        std::vector<point> left(pts.begin(), pts.begin() + mid);
        std::vector<point> right(pts.begin() + mid + 1, pts.end());
        n->left = build(left, depth + 1);
        n->right = build(right, depth + 1);
        return n;
    }

    // Nearest neighbor search with pruning
    void nn_search(const node* n, const point& target,
                   point& best, T& best_dist, int depth) const {
        if (!n) return;
        T dist = euclidean_dist_sq(n->pt, target);
        if (dist < best_dist) {
            best_dist = dist;
            best = n->pt;
        }
        size_t axis = depth % K;
        T diff = target.coords[axis] - n->pt.coords[axis];

        // Search the side of the split that contains the target first
        const node* first = diff < 0 ? n->left.get() : n->right.get();
        const node* second = diff < 0 ? n->right.get() : n->left.get();
        nn_search(first, target, best, best_dist, depth + 1);

        // Only search the other side if the splitting plane is closer
        // than the current best distance
        if (diff * diff < best_dist) {
            nn_search(second, target, best, best_dist, depth + 1);
        }
    }

    static T euclidean_dist_sq(const point& a, const point& b) {
        T sum = T{};
        for (size_t i = 0; i < K; ++i)
            sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
        return sum;
    }

public:
    kd_tree(std::vector<point> points) {
        root_ = build(points, 0);
    }

    // Find the nearest neighbor to target
    std::optional<point> nearest(const point& target) const {
        if (!root_) return std::nullopt;
        point best = root_->pt;
        T best_dist = euclidean_dist_sq(root_->pt, target);
        nn_search(root_.get(), target, best, best_dist, 0);
        return best;
    }

    // Find all points within a hyperrectangle [lo, hi]
    void range_search(const node* n, int depth,
                      const point& lo, const point& hi,
                      std::vector<point>& result) const {
        if (!n) return;
        bool inside = true;
        for (size_t i = 0; i < K; ++i) {
            if (n->pt.coords[i] < lo.coords[i] || n->pt.coords[i] > hi.coords[i]) {
                inside = false;
                break;
            }
        }
        if (inside) result.push_back(n->pt);

        size_t axis = depth % K;
        if (lo.coords[axis] <= n->pt.coords[axis])
            range_search(n->left.get(), depth + 1, lo, hi, result);
        if (hi.coords[axis] >= n->pt.coords[axis])
            range_search(n->right.get(), depth + 1, lo, hi, result);
    }

    std::vector<point> range_query(const point& lo, const point& hi) const {
        std::vector<point> result;
        range_search(root_.get(), 0, lo, hi, result);
        return result;
    }
};
