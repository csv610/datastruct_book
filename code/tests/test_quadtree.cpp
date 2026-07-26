#include "quadtree.h"
#include <cassert>
#include <print>

int main() {
    dsa::quadtree<double> qt({5.0, 5.0, 5.0, 5.0}, 4);

    qt.insert(1.0, 1.0, 0);
    qt.insert(2.0, 2.0, 1);
    qt.insert(3.0, 3.0, 2);
    qt.insert(7.0, 7.0, 3);
    qt.insert(8.0, 8.0, 4);
    assert(qt.size() == 5);

    // Insert more to force subdivision
    for (int i = 0; i < 10; ++i) {
        qt.insert(1.0 + i * 0.5, 1.0 + i * 0.3, 10 + i);
    }
    assert(qt.size() == 15);

    // Range query (circle)
    auto range_pts = qt.range_query(2.0, 2.0, 2.0);
    assert(range_pts.size() >= 1);

    // Rect query
    auto rect_pts = qt.rect_query(1.5, 1.5, 2.0, 2.0);
    assert(rect_pts.size() >= 1);

    std::print("All quadtree tests passed\n");
    return 0;
}
