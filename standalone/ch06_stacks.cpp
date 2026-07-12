// ch06: Stacks
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T>
class array_stack {
    T* data_;
    std::size_t capacity_;
    std::size_t top_;
public:
    array_stack() : data_(new T[16]), capacity_(16), top_(0) {}
    explicit array_stack(std::size_t cap)
        : data_(new T[cap]), capacity_(cap), top_(0) {}
    ~array_stack() { delete[] data_; }

    void push(const T& v) {
        if (top_ == capacity_) {
            capacity_ *= 2;
            T* nd = new T[capacity_];
            for (std::size_t i = 0; i < top_; ++i) nd[i] = std::move(data_[i]);
            delete[] data_;
            data_ = nd;
        }
        data_[top_++] = v;
    }
    T pop() {
        if (top_ == 0) throw std::underflow_error("stack empty");
        return data_[--top_];
    }
    T& peek() { if (top_ == 0) throw std::underflow_error("stack empty"); return data_[top_ - 1]; }
    bool empty() const { return top_ == 0; }
    std::size_t size() const { return top_; }
};

template <typename T>
class linked_stack {
    struct list_node {
        T data;
        std::unique_ptr<list_node> next;
    };
    std::unique_ptr<list_node> head_;
public:
    void push(const T& v) {
        auto n = std::make_unique<list_node>(v, nullptr);
        n->next = std::move(head_);
        head_ = std::move(n);
    }
    T pop() {
        if (!head_) throw std::underflow_error("stack empty");
        T v = head_->data;
        head_ = std::move(head_->next);
        return v;
    }
    bool empty() const { return !head_; }
};

// ---- parentheses matching ----
bool balanced(const std::string& s) {
    array_stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') st.push(c);
        else if (c == ')' || c == '}' || c == ']') {
            if (st.empty()) return false;
            char o = st.pop();
            if ((c == ')' && o != '(') || (c == '}' && o != '{') || (c == ']' && o != '['))
                return false;
        }
    }
    return st.empty();
}

int main() {
    array_stack<int> as;
    as.push(1); as.push(2); as.push(3);
    std::cout << "stack pop: " << as.pop() << " (expect 3)\n";
    std::cout << "stack pop: " << as.pop() << " (expect 2)\n";
    std::cout << "stack size: " << as.size() << " (expect 1)\n";

    std::cout << "balanced '({[]})': " << balanced("({[]})") << " (expect 1)\n";
    std::cout << "balanced '({[})': "  << balanced("({[})")  << " (expect 0)\n";
    std::cout << "All ch06 tests passed.\n";
    return 0;
}
