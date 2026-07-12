#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

struct point { double x, y; };

double dist(const point& a, const point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

double strip_closest(std::vector<point>& strip, double d) {
    double best = d;
    std::sort(strip.begin(), strip.end(),
              [](const point& a, const point& b) { return a.y < b.y; });
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1;
             j < strip.size() && (strip[j].y - strip[i].y) < best;
             ++j) {
            best = std::min(best, dist(strip[i], strip[j]));
        }
    }
    return best;
}

double closest_rec(std::vector<point>& pts, size_t lo, size_t hi) {
    if (hi - lo <= 3) {
        double d = std::numeric_limits<double>::infinity();
        for (size_t i = lo; i < hi; ++i)
            for (size_t j = i + 1; j < hi; ++j)
                d = std::min(d, dist(pts[i], pts[j]));
        return d;
    }

    size_t mid = lo + (hi - lo) / 2;
    double mid_x = pts[mid].x;
    double dl = closest_rec(pts, lo, mid);
    double dr = closest_rec(pts, mid, hi);
    double d = std::min(dl, dr);

    std::vector<point> strip;
    for (size_t i = lo; i < hi; ++i) {
        if (std::abs(pts[i].x - mid_x) < d)
            strip.push_back(pts[i]);
    }
    return strip_closest(strip, d);
}

double closest_pair(std::vector<point> pts) {
    std::sort(pts.begin(), pts.end(),
              [](const point& a, const point& b) { return a.x < b.x; });
    return closest_rec(pts, 0, pts.size());
}
