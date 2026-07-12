std::hash<int> int_hash;
size_t h = int_hash(42);

std::hash<std::string> str_hash;
size_t h2 = str_hash("hello");

// Custom hash for user types:
struct point {
    int x, y;
    bool operator==(const point& other) const {
        return x == other.x && y == other.y;
    }
};

template <>
struct std::hash<point> {
    size_t operator()(const point& p) const noexcept {
        size_t h1 = std::hash<int>{}(p.x);
        size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};
