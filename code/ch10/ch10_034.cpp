class median_tracker {
    std::priority_queue<int> lo;                          // max-heap
    std::priority_queue<int, std::vector<int>, std::greater<int>> hi;  // min-heap
public:
    void add(int num) {
        lo.push(num);
        hi.push(lo.top()); lo.pop();
        if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }
    }
    double median() {
        return lo.size() > hi.size() ? lo.top() : (lo.top() + hi.top()) / 2.0;
    }
};
