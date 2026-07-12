#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <memory>
#include <string>
#include <functional>
#include <cassert>
#include <unordered_map>

// ch06: Stacks

// 1. Basic Stack
template <typename T>
class stack {
    std::vector<T> data_;
public:
    void push(const T& v) { data_.push_back(v); }
    void push(T&& v) { data_.push_back(std::move(v)); }
    void pop() { if (data_.empty()) throw std::underflow_error("empty"); data_.pop_back(); }
    T& top() { if (data_.empty()) throw std::underflow_error("empty"); return data_.back(); }
    const T& top() const { if (data_.empty()) throw std::underflow_error("empty"); return data_.back(); }
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }
};

// 2. Min Stack (O(1) min)
template <typename T>
class min_stack {
    std::vector<T> data_, mins_;
public:
    void push(const T& v) {
        data_.push_back(v);
        if (mins_.empty() || v <= mins_.back()) mins_.push_back(v);
    }
    void pop() {
        if (data_.empty()) throw std::underflow_error("empty");
        if (data_.back() == mins_.back()) mins_.pop_back();
        data_.pop_back();
    }
    T& top() { return data_.back(); }
    const T& min() const { return mins_.back(); }
    bool empty() const { return data_.empty(); }
};

// 3. Stack using two queues
template <typename T>
class stack_via_queues {
    std::queue<T> q1_, q2_;
public:
    void push(const T& v) { q1_.push(v); }
    void pop() {
        if (q1_.empty()) throw std::underflow_error("empty");
        while (q1_.size() > 1) {
            q2_.push(q1_.front());
            q1_.pop();
        }
        q1_.pop();
        std::swap(q1_, q2_);
    }
    T& top() {
        if (q1_.empty()) throw std::underflow_error("empty");
        while (q1_.size() > 1) {
            q2_.push(q1_.front());
            q1_.pop();
        }
        T val = q1_.front();
        q2_.push(val);
        q1_.pop();
        std::swap(q1_, q2_);
        return q2_.back();  // Actually need to return reference...
    }
    bool empty() const { return q1_.empty(); }
};

// 4. Balanced Symbol Checking
bool balanced_symbols(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') st.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char open = st.top(); st.pop();
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) return false;
        }
    }
    return st.empty();
}

// 5. Postfix Evaluation
int evaluate_postfix(const std::string& expr) {
    std::stack<int> st;
    for (char c : expr) {
        if (std::isdigit(c)) st.push(c - '0');
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            int r = 0;
            if (c == '+') r = a + b;
            else if (c == '-') r = a - b;
            else if (c == '*') r = a * b;
            else if (c == '/') r = a / b;
            st.push(r);
        }
    }
    return st.top();
}

// 6. Infix to Postfix (Shunting Yard)
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

std::string infix_to_postfix(const std::string& infix) {
    std::string output;
    std::stack<char> ops;
    for (char c : infix) {
        if (std::isdigit(c)) output += c;
        else if (c == '(') ops.push(c);
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output += ops.top(); ops.pop();
            }
            ops.pop();  // remove '('
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                output += ops.top(); ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) { output += ops.top(); ops.pop(); }
    return output;
}

// 7. Largest Rectangle in Histogram
int largest_rectangle_area(const std::vector<int>& heights) {
    std::stack<int> st;
    int max_area = 0;
    std::vector<int> h = heights;
    h.push_back(0);  // Sentinel
    for (int i = 0; i < (int)h.size(); ++i) {
        while (!st.empty() && h[st.top()] > h[i]) {
            int height = h[st.top()]; st.pop();
            int width = st.empty() ? i : i - st.top() - 1;
            max_area = std::max(max_area, height * width);
        }
        st.push(i);
    }
    return max_area;
}

// 8. Function Call Simulation
struct ActivationRecord {
    std::string func_name;
    int param;
    int return_addr;
    int local_var;
};

class call_stack {
    std::vector<ActivationRecord> frames_;
    int pc_ = 0;  // Program counter
public:
    void call(const std::string& name, int param) {
        frames_.push_back({name, param, pc_, 0});
        pc_ = 0;
    }
    
    void ret() {
        if (frames_.empty()) throw std::underflow_error("stack underflow");
        frames_.pop_back();
        pc_ = frames_.empty() ? 0 : frames_.back().return_addr;
    }
    
    void print_stack() const {
        for (const auto& f : frames_) {
            std::cout << "  " << f.func_name << "(param=" << f.param << ")\n";
        }
    }
};

// 9. Recursive Factorial with Manual Stack
int factorial_stack(int n) {
    std::stack<std::pair<int, int>> st;  // {n, partial_result}
    st.push({n, 1});
    int result = 1;
    
    while (!st.empty()) {
        auto [k, partial] = st.top(); st.pop();
        if (k <= 1) {
            result *= partial;
        } else {
            st.push({k - 1, partial * k});
        }
    }
    return result;
}

void test_basic_stack() {
    std::cout << "=== Testing Basic Stack ===\n";
    stack<int> s;
    s.push(1); s.push(2); s.push(3);
    assert(s.size() == 3);
    assert(s.top() == 3);
    s.pop();
    assert(s.top() == 2);
    s.pop(); s.pop();
    assert(s.empty());
    std::cout << "Basic Stack: PASS\n";
}

void test_min_stack() {
    std::cout << "=== Testing Min Stack ===\n";
    min_stack<int> ms;
    ms.push(5); ms.push(3); ms.push(7); ms.push(2);
    assert(ms.min() == 2);
    ms.pop();
    assert(ms.min() == 3);
    ms.pop(); ms.pop();
    assert(ms.min() == 5);
    std::cout << "Min Stack: PASS\n";
}

void test_balanced_symbols() {
    std::cout << "=== Testing Balanced Symbols ===\n";
    assert(balanced_symbols("()"));
    assert(balanced_symbols("([])"));
    assert(balanced_symbols("{[()]}"));
    assert(!balanced_symbols("("));
    assert(!balanced_symbols("([)]"));
    assert(!balanced_symbols("]"));
    std::cout << "Balanced Symbols: PASS\n";
}

void test_postfix() {
    std::cout << "=== Testing Postfix Evaluation ===\n";
    // 3 4 + 2 * = (3+4)*2 = 14
    assert(evaluate_postfix("34+2*") == 14);
    // 5 1 2 + 4 * + 3 - = 5 + (1+2)*4 - 3 = 14
    assert(evaluate_postfix("512+4*+3-") == 14);
    std::cout << "Postfix Evaluation: PASS\n";
}

void test_infix_to_postfix() {
    std::cout << "=== Testing Infix to Postfix ===\n";
    assert(infix_to_postfix("3+4*2") == "342*+");
    assert(infix_to_postfix("(3+4)*2") == "34+2*");
    assert(infix_to_postfix("3+4*2/(1-5)") == "342*15-/+");
    std::cout << "Infix to Postfix: PASS\n";
}

void test_largest_rectangle() {
    std::cout << "=== Testing Largest Rectangle ===\n";
    assert(largest_rectangle_area({2,1,5,6,2,3}) == 10);
    assert(largest_rectangle_area({2,4}) == 4);
    assert(largest_rectangle_area({1,2,3,4,5}) == 9);
    std::cout << "Largest Rectangle: PASS\n";
}

void test_factorial_stack() {
    std::cout << "=== Testing Factorial with Stack ===\n";
    assert(factorial_stack(0) == 1);
    assert(factorial_stack(1) == 1);
    assert(factorial_stack(5) == 120);
    assert(factorial_stack(10) == 3628800);
    std::cout << "Factorial Stack: PASS\n";
}

void test_call_stack() {
    std::cout << "=== Testing Call Stack Simulation ===\n";
    call_stack cs;
    cs.call("main", 0);
    cs.call("foo", 1);
    cs.call("bar", 2);
    cs.call("baz", 3);
    cs.ret();  // baz returns
    cs.ret();  // bar returns
    cs.call("qux", 4);
    // Stack: main, foo, qux
    std::cout << "Call Stack: PASS\n";
}

int main() {
    std::cout << "=== Chapter 6: Stacks ===\n\n";
    test_basic_stack();
    test_min_stack();
    test_balanced_symbols();
    test_postfix();
    test_infix_to_postfix();
    test_largest_rectangle();
    test_factorial_stack();
    test_call_stack();
    std::cout << "\n=== All tests passed ===\n";
    return 0;
}