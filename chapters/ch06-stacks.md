# Stacks

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The Stack ADT

A **stack** is a linear list where insertions and deletions occur only at one end, called the **top**. This LIFO (Last In, First Out) discipline is fundamental in computing.

### Operations

- **push(value)** — insert at top
- **pop()** — remove and return the top element
- **top()** — return the top element without removing it
- **empty()** — true if no elements
- **size()** — number of elements

### Applications

Stacks are used for:
- Function call/return management (the call stack)
- Expression evaluation (postfix, infix conversion)
- Undo operations
- Depth-first search (Chapter 12)
- Parsing (balanced parentheses, HTML/XML)
- Backtracking algorithms (Chapter 20)

## Array-Based Stack

```cpp
template <typename T>
class array_stack {
public:
    array_stack() = default;

    void push(const T& value) {
        if (size_ >= data_.size()) {
            data_.push_back(value);
        } else {
            data_[size_] = value;
        }
        ++size_;
    }

    void push(T&& value) {
        if (size_ >= data_.size()) {
            data_.push_back(std::move(value));
        } else {
            data_[size_] = std::move(value);
        }
        ++size_;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        if (size_ >= data_.size()) {
            data_.emplace_back(std::forward<Args>(args)...);
        } else {
            data_[size_] = T(std::forward<Args>(args)...);
        }
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("stack is empty");
        --size_;
    }

    T& top() {
        if (empty()) throw std::underflow_error("stack is empty");
        return data_[size_ - 1];
    }
    const T& top() const {
        if (empty()) throw std::underflow_error("stack is empty");
        return data_[size_ - 1];
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    std::vector<T> data_;
    size_t size_ = 0;
};
```

All operations are O(1) amortized. The `std::vector` grows as needed, but we track `size_` separately to support O(1) `pop` without deallocating memory.

## Linked Stack

```cpp
template <typename T>
class linked_stack {
public:
    linked_stack() = default;

    void push(const T& value) {
        head_ = std::make_unique<node<T>>(value, std::move(head_));
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("stack is empty");
        head_ = std::move(head_->next);
        --size_;
    }

    T& top() {
        if (empty()) throw std::underflow_error("stack is empty");
        return head_->data;
    }
    const T& top() const {
        if (empty()) throw std::underflow_error("stack is empty");
        return head_->data;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        node(const T& d, std::unique_ptr<node> nxt)
            : data(d), next(std::move(nxt)) {}
    };
    std::unique_ptr<node> head_;
    size_t size_ = 0;
};
```

All operations: O(1).

## Applications

### Balanced Parentheses

Check whether parentheses, brackets, and braces are properly matched:

```cpp
bool is_balanced(std::string_view expr) {
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
```

**Trace:** `"{[()]}"`

```
c = '{' → push: stack = ['{']
c = '[' → push: stack = ['{', '[']
c = '(' → push: stack = ['{', '[', '(']
c = ')' → top is '(' → pop: stack = ['{', '[']
c = ']' → top is '[' → pop: stack = ['{']
c = '}' → top is '{' → pop: stack = []
Result: true
```

### Postfix Evaluation

Postfix (Reverse Polish Notation) places operators after operands: `3 4 + 5 *` = (3+4)×5 = 35.

```cpp
int evaluate_postfix(std::string_view expr) {
    linked_stack<int> s;
    for (char c : expr) {
        if (std::isspace(c)) continue;
        if (std::isdigit(c)) {
            s.push(c - '0');
        } else {
            int b = s.top(); s.pop();
            int a = s.top(); s.pop();
            switch (c) {
                case '+': s.push(a + b); break;
                case '-': s.push(a - b); break;
                case '*': s.push(a * b); break;
                case '/': s.push(a / b); break;
            }
        }
    }
    return s.top();
}
```

**Trace:** `"3 4 + 5 *"`

```
'3' → push: [3]
'4' → push: [3, 4]
'+' → pop 4, pop 3 → push 7: [7]
'5' → push: [7, 5]
'*' → pop 5, pop 7 → push 35: [35]
Result: 35
```

### Expression Evaluation in a Spreadsheet Engine (e.g., Excel, Google Sheets)

Modern spreadsheet engines evaluate cell formulas using an operator-precedence parser backed by a stack. When a cell contains `=(A1+B2)*C3-D4`, the engine:

1. Converts infix to postfix (shunting-yard, stack-based)
2. Evaluates the postfix expression (also stack-based)

The shunting-yard algorithm uses an operator stack:

```cpp
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

std::string infix_to_postfix(std::string_view expr) {
    linked_stack<char> ops;
    std::string output;
    for (char c : expr) {
        if (std::isalnum(c)) {
            output += c;
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output += ops.top(); ops.pop();
            }
            ops.pop();  // discard '('
        } else { // operator
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                output += ops.top(); ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) { output += ops.top(); ops.pop(); }
    return output;
}
```

**Trace:** `"(A1+B2)*C3-D4"` → `"A1 B2 + C3 * D4 -"`

This is the same algorithm PostgreSQL uses in its expression parser (src/backend/parser/expr_parse.c) and that Google Sheets applies to every formula cell update.

## STL Connection: std::stack

`std::stack` is a container adapter — it wraps any sequence container providing `push_back`, `pop_back`, and `back`:

```cpp
std::stack<int> s;                          // uses std::deque by default
std::stack<int, std::vector<int>> sv;     // vector-based
std::stack<int, std::list<int>> sl;       // list-based

s.push(10);
s.push(20);
int x = s.top();  // 20
s.pop();          // removes 20
```

Our `array_stack` is more efficient than `std::stack` for push/pop because we avoid deallocation (we only decrement `size_`), while `std::stack::pop` destroys the element immediately.

## Common Bugs and Pitfalls

- **Pop on empty stack** — undefined behavior (or exception) if you call `pop()` or `top()` on an empty stack. Always guard with `empty()`.
- **Incorrect top() semantics** — many textbooks define `pop()` as "remove and return," but C++ convention separates `top()` (read) and `pop()` (remove). Confusing these leads to lost data.
- **Forgetting the amortization** — `array_stack::push` is O(1) amortized, but individual pushes can be O(n) when the vector grows. In hard real-time systems, use linked stack instead.
- **Stack overflow in recursion** — the call stack is a real stack with finite size. Deep recursion (e.g., recursive DFS on a large graph) causes stack overflow. Use an explicit stack instead.
- **Memory leak in linked stack** — with raw pointers, popping without deleting the node leaks memory. Our `linked_stack` uses `unique_ptr`, which is safe, but manually written linked stacks are a common source of leaks in production code.

## Summary

1. A **stack** is a LIFO structure — insertions and deletions at one end (the top).
2. **Array-based stacks** use a dynamic array for O(1) amortized operations.
3. **Linked stacks** use a singly linked list for O(1) worst-case operations.
4. Industrial applications include the system call stack, spreadsheet expression engines (Excel, Google Sheets), stack-based VMs (JVM), and undo/redo systems.
5. `std::stack` is a container adapter that wraps any sequence container.

## Exercises

### Drill

1. Trace the call stack for a recursive `fibonacci(5)` computation. Show the stack frame contents (return address, parameters) at each push and pop. How deep does the stack grow?

2. Convert the infix expression `price > 100 AND (quantity < 10 OR discount > 0.2)` to postfix using the shunting-yard algorithm. Treat `AND` and `OR` as binary operators with precedence: NOT (3), AND (2), OR (1).

3. Evaluate `"5 3 + 7 2 - ×"` using a stack. Show the stack state after each token.

4. A production web server receives requests at 1000 req/s. Each request enters a middleware pipeline (stack of handlers). If each handler takes 50 µs and the pipeline is 10 handlers deep, what is the minimum time before the first byte is sent? Where does the LIFO order matter for middleware semantics?

5. A Just-In-Time compiler's register allocator uses a stack to spill registers. If 6 registers are available, how many spills occur when evaluating `a + b * c / d - e` with the usual precedence? Show the stack states.

### Application

6. **Min-stack for stock price monitoring.** You are implementing a real-time trading system that must report the minimum stock price seen in O(1) time (without scanning history). Implement `array_stack` with an auxiliary stack that tracks the current minimum. Feed it a stream of 10,000 NASDAQ price ticks. Verify that `get_min()` returns the correct minimum at every point.

7. **Spreadsheet expression engine.** Implement a postfix evaluator that supports cell references, arithmetic (+, -, ×, /, ^), and functions (SUM, AVG, MIN, MAX). Test with the expression `SUM(A1:A3) * (B1 + B2)`. Hint: resolve cell references against a symbol table before pushing values.

8. **Chromium's browser back/forward.** Implement a dual-stack navigation manager as used in Chrome's tab history. Support `visit(url, is_new_page)` (pushes onto back stack, clears forward stack), `back()` (moves current to forward stack), and `forward()`. Handle the edge case of no history.

9. **V8 JavaScript call stack.** V8 maintains a stack of execution contexts (global, function, eval). Simulate this: each function call creates a `context` object with local variables and a return address. Implement a trace that shows the context stack during a recursive factorial computation. Detect and report stack overflow at depth 1000.

10. **Redis transaction stack.** Redis uses a stack to batch commands in a `MULTI`/`EXEC` transaction. Implement a `TransactionStack` that supports `queue(command)`, `execute()` (pops all and runs them atomically), and `discard()` (clears without executing). Handle nested transactions by stacking transaction states.

11. **Expression syntax checker (ESLint-style).** Implement a balanced-parentheses checker that also validates bracket matching in source code. Extend it to detect mismatched HTML/XML tags using a stack: `<div><p>text</p></div>` is valid, `<div><p>text</div></p>` is not. Test on real-world HTML snippets from the web.

### Research

12. **(Stack-based VMs)** JVM, CPython, and the Ethereum EVM are all stack-based machines. Write a minimal bytecode interpreter that supports PUSH, POP, ADD, SUB, MUL, DUP, SWAP, and JMP. Execute a small program that computes `(3 + 4) * 5`. Compare the bytecode size with a register-based ISA (e.g., Lua VM).

13. **(Treiber stack)** Implement the Treiber lock-free stack using `std::atomic<std::shared_ptr<node>>`. Benchmark it against `std::stack` protected by `std::mutex` on 2, 4, and 8 threads. At what contention level does the lock-free version win? Where does it lose?

14. **(Amortized analysis)** Prove that our `array_stack` has O(1) amortized push cost using both the accounting method and the potential method. Then compare real-world benchmark results: measure the worst-case push latency (when the vector grows) vs the average across 10⁶ pushes. How much slower is the worst case?

## References and Further Reading

- Donald E. Knuth, *The Art of Computer Programming*, Volume 1: Fundamental Algorithms. Section 2.2.1.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Section 10.1.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Section 1.3.
- R. K. Treiber, "Systems Programming: Coping with Parallelism" — IBM Technical Report, 1986.
