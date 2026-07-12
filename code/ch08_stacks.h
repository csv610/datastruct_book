#ifndef STACK_H
#define STACK_H

#include <concepts>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

// ---- Array-based stack ----
template <std::regular T>
class array_stack {
public:
    array_stack() = default;

    void push(const T& value) {
        if (size_ >= data_.size())
            data_.push_back(value);
        else
            data_[size_] = value;
        ++size_;
    }

    void push(T&& value) {
        if (size_ >= data_.size())
            data_.push_back(std::move(value));
        else
            data_[size_] = std::move(value);
        ++size_;
    }

    void pop() {
        if (empty())
            throw std::underflow_error("stack is empty");
        --size_;
    }

    T& top() {
        if (empty())
            throw std::underflow_error("stack is empty");
        return data_[size_ - 1];
    }

    const T& top() const {
        if (empty())
            throw std::underflow_error("stack is empty");
        return data_[size_ - 1];
    }

    bool     empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

private:
    std::vector<T> data_;
    std::size_t    size_ = 0;
};

// ---- Linked-list-based stack ----
template <std::regular T>
class linked_stack {
    struct snode {
        T data;
        std::unique_ptr<snode> next;
        snode(const T& d, std::unique_ptr<snode> nxt)
            : data(d), next(std::move(nxt)) {}
    };

public:
    linked_stack() = default;

    void push(const T& value) {
        head_ = std::make_unique<snode>(value, std::move(head_));
        ++size_;
    }

    void push(T&& value) {
        head_ = std::make_unique<snode>(std::move(value), std::move(head_));
        ++size_;
    }

    void pop() {
        if (empty())
            throw std::underflow_error("stack is empty");
        head_ = std::move(head_->next);
        --size_;
    }

    T& top() {
        if (empty())
            throw std::underflow_error("stack is empty");
        return head_->data;
    }

    const T& top() const {
        if (empty())
            throw std::underflow_error("stack is empty");
        return head_->data;
    }

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

private:
    std::unique_ptr<snode> head_;
    std::size_t            size_ = 0;
};

// ---- Generic stack adapter (like std::stack) ----
template <std::regular T, typename Container = array_stack<T>>
class stack {
public:
    void push(const T& v) { c_.push(v); }
    void push(T&& v)      { c_.push(std::move(v)); }
    void pop()            { c_.pop(); }
    T& top()              { return c_.top(); }
    const T& top() const  { return c_.top(); }
    bool empty() const    { return c_.empty(); }
    std::size_t size() const { return c_.size(); }

private:
    Container c_;
};

// ---- Utility functions using stacks ----

// Check balanced parentheses
inline bool is_balanced(std::string_view expr) {
    linked_stack<char> s;
    for (char c : expr) {
        switch (c) {
            case '(': case '[': case '{':
                s.push(c);
                break;
            case ')':
                if (s.empty() || s.top() != '(') return false;
                s.pop();
                break;
            case ']':
                if (s.empty() || s.top() != '[') return false;
                s.pop();
                break;
            case '}':
                if (s.empty() || s.top() != '{') return false;
                s.pop();
                break;
        }
    }
    return s.empty();
}

// Evaluate postfix expression (single-digit operands)
inline int evaluate_postfix(std::string_view expr) {
    linked_stack<int> s;
    for (char c : expr) {
        if (c == ' ') continue;
        if (c >= '0' && c <= '9') {
            s.push(c - '0');
        } else {
            int b = s.top(); s.pop();
            int a = s.top(); s.pop();
            switch (c) {
                case '+': s.push(a + b); break;
                case '-': s.push(a - b); break;
                case '*': s.push(a * b); break;
                case '/': s.push(a / b); break;
                default:
                    throw std::invalid_argument("unknown operator");
            }
        }
    }
    return s.top();
}

// Convert infix to postfix (simple operators: + - * /)
inline std::string infix_to_postfix(std::string_view infix) {
    std::string result;
    linked_stack<char> ops;

    auto precedence = [](char op) -> int {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    };

    for (char c : infix) {
        if (c == ' ') continue;
        if (c >= '0' && c <= '9') {
            result += c;
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                result += ops.top();
                ops.pop();
            }
            if (!ops.empty()) ops.pop();  // remove '('
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                result += ops.top();
                ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) {
        result += ops.top();
        ops.pop();
    }
    return result;
}

#endif
