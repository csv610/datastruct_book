struct point { int x, y; };
struct point_hash {
    size_t operator()(const point& p) const {
        size_t h = std::hash<int>{}(p.x);
        h ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};
