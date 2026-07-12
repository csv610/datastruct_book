#include <vector>
#include <algorithm>
#include <memory>

struct point { int x, y; };

class range_tree {
    struct node {
        point pt;
        int height = 1;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;
        std::vector<point> secondary;  // y-sorted points in this subtree

        node(const point& p) : pt(p) {}
    };

    std::unique_ptr<node> root_;

    int height(const node* n) const { return n ? n->height : 0; }

    void update_height(node* n) {
        n->height = 1 + std::max(height(n->left.get()),
                                  height(n->right.get()));
    }

    std::unique_ptr<node> rotate_right(std::unique_ptr<node> y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        update_height(x->right.get());
        update_height(x.get());
        return x;
    }

    std::unique_ptr<node> rotate_left(std::unique_ptr<node> x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        update_height(y->left.get());
        update_height(y.get());
        return y;
    }

    std::unique_ptr<node> balance(std::unique_ptr<node> n) {
        update_height(n.get());
        int bf = height(n->left.get()) - height(n->right.get());
        if (bf > 1) {
            if (height(n->left->right.get()) > height(n->left->left.get()))
                n->left = rotate_left(std::move(n->left));
            return rotate_right(std::move(n));
        }
        if (bf < -1) {
            if (height(n->right->left.get()) > height(n->right->right.get()))
                n->right = rotate_right(std::move(n->right));
            return rotate_left(std::move(n));
        }
        return n;
    }

    std::unique_ptr<node> insert(std::unique_ptr<node> n, const point& p) {
        if (!n) return std::make_unique<node>(p);
        if (p.x <= n->pt.x) n->left = insert(std::move(n->left), p);
        else                 n->right = insert(std::move(n->right), p);
        return balance(std::move(n));
    }

    void build_secondary(node* n) {
        if (!n) return;
        build_secondary(n->left.get());
        build_secondary(n->right.get());
        n->secondary.push_back(n->pt);
        if (n->left)
            std::merge(n->secondary.begin(), n->secondary.end(),
                       n->left->secondary.begin(), n->left->secondary.end(),
                       std::back_inserter(n->secondary),
                       [](const point& a, const point& b){ return a.y < b.y; });
        // Clear child secondary trees to save memory after merge
        // In production, use persistent sharing instead.
        if (n->left)  n->left->secondary.clear();
        if (n->right) n->right->secondary.clear();
    }

    void range_query_x(const node* n, int x1, int x2,
                       int y1, int y2, std::vector<point>& result) const {
        if (!n) return;
        if (x1 <= n->pt.x && n->pt.x <= x2)
            range_query_y(n->secondary, y1, y2, result);
        if (n->pt.x >= x1) range_query_x(n->left.get(), x1, x2, y1, y2, result);
        if (n->pt.x <= x2) range_query_x(n->right.get(), x1, x2, y1, y2, result);
    }

    static void range_query_y(const std::vector<point>& pts,
                              int y1, int y2, std::vector<point>& result) {
        auto lo = std::lower_bound(pts.begin(), pts.end(), y1,
                     [](const point& p, int v){ return p.y < v; });
        auto hi = std::upper_bound(pts.begin(), pts.end(), y2,
                     [](int v, const point& p){ return v < p.y; });
        result.insert(result.end(), lo, hi);
    }

public:
    void insert(const point& p) { root_ = insert(std::move(root_), p); }

    void build() { build_secondary(root_.get()); }

    std::vector<point> range_query(int x1, int x2,
                                   int y1, int y2) const {
        std::vector<point> result;
        range_query_x(root_.get(), x1, x2, y1, y2, result);
        return result;
    }
};
