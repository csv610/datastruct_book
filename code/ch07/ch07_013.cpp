// Amortized O(1) queue using two stacks.
// Each element is pushed and popped at most twice.
template <typename T>
class queue_via_stacks {
public:
    void push(const T& value) {
        in_stack_.push(value);
    }

    // Amortized O(1): each element moves from in_stack_
    // to out_stack exactly once over the lifetime of the queue.
    T pop() {
        if (out_stack_.empty()) {
            transfer();
        }
        if (out_stack_.empty()) {
            throw std::underflow_error("queue is empty");
        }
        T val = out_stack_.top();
        out_stack_.pop();
        return val;
    }

    T front() {
        if (out_stack_.empty()) {
            transfer();
        }
        return out_stack_.top();
    }

    bool empty() const {
        return in_stack_.empty() && out_stack_.empty();
    }

    int size() const {
        return in_stack_.size() + out_stack_.size();
    }

private:
    void transfer() {
        while (!in_stack_.empty()) {
            out_stack_.push(in_stack_.top());
            in_stack_.pop();
        }
    }

    std::stack<T> in_stack_;   // for push
    std::stack<T> out_stack_;  // for pop
};

// Exercise all operations.
int main() {
    queue_via_stacks<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    std::cout << q.pop() << "\n";  // 1 (FIFO)
    q.push(4);
    std::cout << q.pop() << "\n";  // 2
    std::cout << q.pop() << "\n";  // 3
    std::cout << q.pop() << "\n";  // 4
    return 0;
}
