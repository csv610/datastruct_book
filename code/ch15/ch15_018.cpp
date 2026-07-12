#include <queue>

int optimal_merge(std::span<const int> sizes) {
    // Min-heap of file sizes
    std::priority_queue<int, std::vector<int>, std::greater<>> pq;
    for (int s : sizes) pq.push(s);

    int total_cost = 0;
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        int merged = a + b;
        total_cost += merged;
        pq.push(merged);
    }
    return total_cost;
}
