class median_tracker {
public:
    void add_number(int x) {
        if (max_heap_.empty() || x <= max_heap_.top()) {
            max_heap_.push(x);
        } else {
            min_heap_.push(x);
        }

        // Rebalance
        if (max_heap_.size() > min_heap_.size() + 1) {
            min_heap_.push(max_heap_.top());
            max_heap_.pop();
        } else if (min_heap_.size() > max_heap_.size()) {
            max_heap_.push(min_heap_.top());
            min_heap_.pop();
        }
    }

    double median() const {
        if (max_heap_.size() == min_heap_.size()) {
            return (max_heap_.top() + min_heap_.top()) / 2.0;
        }
        return max_heap_.top();
    }

private:
    std::priority_queue<int> max_heap_;                // lower half
    std::priority_queue<int, std::vector<int>,
                        std::greater<int>> min_heap_;  // upper half
};
