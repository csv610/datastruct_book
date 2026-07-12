class stack_via_queues_v2 {
public:
    void push(int x) {
        q1_.push(x);
    }

    void pop() {
        if (q1_.empty())
            throw std::underflow_error("empty");
        while (q1_.size() > 1) {
            q2_.push(q1_.front());
            q1_.pop();
        }
        q1_.pop();
        std::swap(q1_, q2_);
    }

    int top() {
        if (q1_.empty())
            throw std::underflow_error("empty");
        while (q1_.size() > 1) {
            q2_.push(q1_.front());
            q1_.pop();
        }
        int val = q1_.front();
        q2_.push(val);
        q1_.pop();
        std::swap(q1_, q2_);
        return val;
    }

    bool empty() const { return q1_.empty(); }

private:
    std::queue<int> q1_, q2_;
};
