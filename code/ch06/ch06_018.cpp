class stack_via_queues {
public:
    void push(int x) {
        q2_.push(x);
        while (!q1_.empty()) {
            q2_.push(q1_.front());
            q1_.pop();
        }
        std::swap(q1_, q2_);
    }

    void pop() {
        if (q1_.empty())
            throw std::underflow_error("empty");
        q1_.pop();
    }

    int top() {
        if (q1_.empty())
            throw std::underflow_error("empty");
        return q1_.front();
    }

    bool empty() const { return q1_.empty(); }

private:
    std::queue<int> q1_, q2_;
};
