struct point { double x, y; };

double closest_pair(std::span<point> points) {
    if (points.size() <= 3) return brute_force(points);

    size_t mid = points.size() / 2;
    double mid_x = points[mid].x;

    double dl = closest_pair(points.subspan(0, mid));
    double dr = closest_pair(points.subspan(mid));
    double d = std::min(dl, dr);

    // Collect points within the strip
    std::vector<point> strip;
    for (const auto& p : points) {
        if (std::abs(p.x - mid_x) < d) strip.push_back(p);
    }

    // Sort strip by y
    std::sort(strip.begin(), strip.end(),
              [](const point& a, const point& b) { return a.y < b.y; });

    // Check each point against next few (at most 7)
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; ++j) {
            double dist = euclidean(strip[i], strip[j]);
            d = std::min(d, dist);
        }
    }
    return d;
}
