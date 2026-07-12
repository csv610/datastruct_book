double brute_force_closest(std::span<point> pts) {
    double best = std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < pts.size(); ++i) {
        for (size_t j = i + 1; j < pts.size(); ++j) {
            double dx = pts[i].x - pts[j].x;
            double dy = pts[i].y - pts[j].y;
            double dist = std::sqrt(dx * dx + dy * dy);
            best = std::min(best, dist);
        }
    }
    return best;
}
