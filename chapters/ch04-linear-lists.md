# Linear Lists

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Array-Based Lists

### The Linear List Abstract Data Type

A **linear list** (also called a sequence or vector) is an ordered collection of elements where each element has a position. The positions are numbered from 0 (or 1, depending on convention). We use 0-based indexing throughout this book.

**Operations.** A linear list ADT supports these core operations:

- **get(index)** — return the element at position index
- **set(index, value)** — replace the element at position index
- **insert(index, value)** — insert value at position index, shifting subsequent elements right
- **erase(index)** — remove the element at position index, shifting subsequent elements left
- **size()** — return the number of elements
- **empty()** — return true iff size is 0
- **push_back(value)** — append to the end (convenience)
- **pop_back()** — remove from the end (convenience)
- **clear()** — remove all elements

**Applications.** Linear lists appear everywhere:
- A text editor stores lines in a list
- A music player stores a playlist
- A spreadsheet stores rows of cells
- An image editor stores layers

### Array Representation

The simplest way to represent a linear list is with a fixed-size array. We allocate an array large enough to hold the maximum number of elements and track the current number of elements.

```tikz
\begin{scope}[every node/.style={draw,minimum width=0.8cm,minimum height=0.6cm}]
  \node (i0) at (0,0.8) {0};
  \node (i1) at (1,0.8) {1};
  \node (i2) at (2,0.8) {2};
  \node (i3) at (3,0.8) {3};
  \node (i4) at (4,0.8) {4};
  \node (i5) at (5,0.8) {5};
  \node (i6) at (6,0.8) {6};
  \node (i7) at (7,0.8) {7};
  \node[fill=blue!10] (d0) at (0,0) {10};
  \node[fill=blue!10] (d1) at (1,0) {20};
  \node[fill=blue!10] (d2) at (2,0) {30};
  \node[fill=blue!10] (d3) at (3,0) {40};
  \node[fill=blue!10] (d4) at (4,0) {50};
  \node[fill=gray!20] (d5) at (5,0) {};
  \node[fill=gray!20] (d6) at (6,0) {};
  \node[fill=gray!20] (d7) at (7,0) {};
\end{scope}
\node[above] at (3.5,1.3) {\textbf{data:}};
\node[below] at (3.5,-0.6) {\texttt{size = 5,\ \ capacity = 8}};
```

**Fixed-Capacity List.**

```cpp
template <typename T>
class array_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    explicit array_list(size_type capacity)
        : capacity_(capacity), size_(0), data_(std::make_unique<T[]>(capacity)) {}

    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    size_type capacity() const noexcept { return capacity_; }

    reference operator[](size_type index) {
        check_index(index);
        return data_[index];
    }

    const_reference operator[](size_type index) const {
        check_index(index);
        return data_[index];
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) throw std::overflow_error("list is full");
        data_[size_++] = value;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        --size_;
    }

    void insert(size_type index, const T& value) {
        if (size_ >= capacity_) throw std::overflow_error("list is full");
        if (index > size_) throw std::out_of_range("index out of range");
        for (size_type i = size_; i > index; --i) {
            data_[i] = data_[i - 1];
        }
        data_[index] = value;
        ++size_;
    }

    void erase(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        for (size_type i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
    }

    void clear() noexcept { size_ = 0; }

private:
    void check_index(size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
    }

    size_type capacity_;
    size_type size_;
    std::unique_ptr<T[]> data_;
};
```

This implementation has a serious limitation: the capacity is fixed at construction time. If we need more space, we fail.

### Dynamic Array (Resizable Array)

The solution is to grow the array when it becomes full. This is the strategy used by `std::vector`.

**Growth strategy.** When a `push_back` finds the array full:
1. Allocate a new array of larger capacity (typically 2× the old size)
2. Move all elements from the old array to the new one
3. Deallocate the old array
4. Insert the new element

**Implementation.**

```cpp
template <typename T>
class dynamic_array_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;

    static constexpr size_type default_capacity = 4;

    dynamic_array_list() : size_(0), capacity_(default_capacity),
                           data_(std::make_unique<T[]>(default_capacity)) {}

    dynamic_array_list(std::initializer_list<T> init)
        : size_(init.size()), capacity_(init.size()),
          data_(std::make_unique<T[]>(init.size())) {
        std::copy(init.begin(), init.end(), data_.get());
    }

    dynamic_array_list(const dynamic_array_list& other)
        : size_(other.size_), capacity_(other.capacity_),
          data_(std::make_unique<T[]>(other.capacity_)) {
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
    }

    dynamic_array_list& operator=(const dynamic_array_list& other) {
        if (this != &other) {
            auto new_data = std::make_unique<T[]>(other.capacity_);
            std::copy(other.data_.get(), other.data_.get() + other.size_, new_data.get());
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = std::move(new_data);
        }
        return *this;
    }

    dynamic_array_list(dynamic_array_list&&) = default;
    dynamic_array_list& operator=(dynamic_array_list&&) = default;

    reference operator[](size_type index) {
        check_index(index);
        return data_[index];
    }
    const_reference operator[](size_type index) const {
        check_index(index);
        return data_[index];
    }

    reference at(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        return data_[index];
    }
    const_reference at(size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
        return data_[index];
    }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            grow(new_capacity);
        }
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) grow(2 * capacity_);
        data_[size_++] = value;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) grow(2 * capacity_);
        data_[size_++] = std::move(value);
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ >= capacity_) grow(2 * capacity_);
        data_[size_] = T(std::forward<Args>(args)...);
        return data_[size_++];
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        --size_;
    }

    void insert(size_type index, const T& value) {
        if (index > size_) throw std::out_of_range("index out of range");
        if (size_ >= capacity_) grow(2 * capacity_);
        for (size_type i = size_; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[index] = value;
        ++size_;
    }

    void erase(size_type index) {
        if (index >= size_) throw std::out_of_range("index out of range");
        for (size_type i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        --size_;
    }

    void clear() noexcept { size_ = 0; }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            auto new_data = std::make_unique<T[]>(size_);
            std::move(data_.get(), data_.get() + size_, new_data.get());
            capacity_ = size_;
            data_ = std::move(new_data);
        }
    }

    iterator begin() noexcept { return data_.get(); }
    const_iterator begin() const noexcept { return data_.get(); }
    iterator end() noexcept { return data_.get() + size_; }
    const_iterator end() const noexcept { return data_.get() + size_; }

private:
    void check_index(size_type index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
    }

    void grow(size_type new_capacity) {
        auto new_data = std::make_unique<T[]>(new_capacity);
        std::move(data_.get(), data_.get() + size_, new_data.get());
        capacity_ = new_capacity;
        data_ = std::move(new_data);
    }

    size_type size_;
    size_type capacity_;
    std::unique_ptr<T[]> data_;
};
```

**Step-by-step walkthrough.** Consider the sequence of operations on an initially empty list:

**push_back(10):** size=0, capacity=4. Insert data[0]=10, size=1.
```
    [10 | _ | _ | _ ]
     0    1   2   3
```

**push_back(20):** size=1, capacity=4. Insert data[1]=20, size=2.
```
    [10 | 20 | _ | _ ]
```

**push_back(30), push_back(40):** size=4.
```
    [10 | 20 | 30 | 40]
```

**push_back(50):** size=4 ≥ capacity=4 → grow to 8. Copy elements.
```
    [10 | 20 | 30 | 40 | 50 | _ | _ | _ ]
```

**insert(2, 25):** size=5, capacity=8. Shift elements at indices 2,3,4 right.
```
    [10 | 20 | 25 | 30 | 40 | 50 | _ | _ ]
```

**erase(4):** size=6. Shift elements at indices 5,6 left.
```
    [10 | 20 | 25 | 30 | 50 | _ | _ | _ ]
    size=5
```

### Complexity Analysis

| Operation | Fixed Array | Dynamic Array (amortized) |
|-----------|-------------|---------------------------|
| `operator[]` | O(1) | O(1) |
| `push_back` | O(1) | O(1) amortized |
| `pop_back` | O(1) | O(1) |
| `insert` at front | O(n) | O(n) |
| `insert` at back | O(1) | O(1) amortized |
| `insert` at middle | O(n) | O(n) |
| `erase` at front | O(n) | O(n) |
| `erase` at back | O(1) | O(1) |
| `erase` at middle | O(n) | O(n) |
| Space (overhead) | O(capacity) | O(size) |

Why is `push_back` O(1) amortized? The resize copies n elements every n insertions. The total cost of n insertions is at most 3n: n unit-cost insertions plus at most 2n element copies across all resizes (as shown in Chapter 2).

### Application: Polynomial Arithmetic

We can represent a polynomial using an array list of coefficients. The index is the exponent; the value is the coefficient.

```cpp
class polynomial {
public:
    polynomial() : coeff_(1) {}  // zero polynomial

    void set_coefficient(int exponent, double coefficient) {
        while (static_cast<int>(coeff_.size()) <= exponent) {
            coeff_.push_back(0.0);
        }
        coeff_[exponent] = coefficient;
    }

    double evaluate(double x) const {
        double result = 0.0;
        double power = 1.0;
        for (size_t i = 0; i < coeff_.size(); ++i) {
            result += coeff_[i] * power;
            power *= x;
        }
        return result;
    }

    double coefficient(int exponent) const {
        if (exponent >= static_cast<int>(coeff_.size())) return 0.0;
        return coeff_[exponent];
    }

    int degree() const {
        for (int i = static_cast<int>(coeff_.size()) - 1; i >= 0; --i) {
            if (coeff_[i] != 0.0) return i;
        }
        return -1;  // zero polynomial
    }

private:
    dynamic_array_list<double> coeff_;
};
```

**Example.** Polynomial: 3x⁵ - 2x² + 7

```
coeff_ = [7, 0, -2, 0, 0, 3]
index:    0  1   2  3  4  5

evaluate at x = 2:
7 + 0·2 + (-2)·4 + 0·8 + 0·16 + 3·32
= 7 - 8 + 96 = 95
```

### Application: Image Editor Layer Stack

A layer stack in an image editor is a linear list of layers. Adding a layer appends to the top. Deleting a layer removes it by index. Reordering moves a layer from one index to another.

```cpp
struct layer {
    std::string name;
    bool visible = true;
    float opacity = 1.0f;
};

using layer_stack = dynamic_array_list<layer>;

void reorder_layer(layer_stack& stack, size_t from, size_t to) {
    layer l = std::move(stack[from]);
    stack.erase(from);
    stack.insert(to, std::move(l));
}
```

### STL Connection: std::vector

Our `dynamic_array_list` closely mirrors `std::vector`. The standard library version adds:
- **Allocator support** — custom memory allocation strategies
- **`data()`** — raw pointer access
- **`resize(n)`** — change size, default-construct or destroy elements
- **`assign`** — replace all elements
- **Comparison operators** — lexicographic comparison
- **`swap`** — O(1) exchange of two vectors
- **Exception safety guarantees** — strong guarantee for most operations

```cpp
std::vector<int> v = {1, 2, 3};
v.push_back(4);
v.insert(v.begin(), 0);
std::sort(v.begin(), v.end());
```

The fundamental design is the same: a contiguous array that grows geometrically.

---

## Linked Lists

Array-based lists have a fundamental limitation: inserting or erasing elements in the middle requires shifting O(n) elements. A **linked list** avoids this by storing elements in separately allocated **nodes** connected by pointers.

Each node contains:
- A **data** field holding the element
- A **next** pointer to the next node (and optionally a **prev** pointer for doubly linked lists)

### Singly Linked List (Chain)

A singly linked list connects nodes in one direction:

```tikz
\tikzstyle{nodebox}=[draw,rectangle,minimum width=1.8cm,minimum height=0.7cm,inner sep=2pt]
\tikzstyle{nullbox}=[draw,rectangle,minimum width=0.4cm,minimum height=0.7cm,inner sep=2pt]
\node (headlabel) at (0,0.5) {\texttt{head}};
\node[nodebox] (n1) at (2.5,0.5) {\texttt{10}\hspace{0.8cm}\texttt{$\bullet$}};
\node[nodebox] (n2) at (5.5,0.5) {\texttt{20}\hspace{0.8cm}\texttt{$\bullet$}};
\node[nodebox] (n3) at (8.5,0.5) {\texttt{30}\hspace{0.8cm}\texttt{$\bullet$}};
\node[nullbox] (null) at (10.5,0.5) {$\varnothing$};
\draw[->,>=stealth,thick] (headlabel.east) -- (n1.west);
\draw[->,>=stealth,thick] (1.6,0.5) -- node[above]{\small\texttt{next}} (n2.west);
\draw[->,>=stealth,thick] (4.6,0.5) -- (n3.west);
\draw[->,>=stealth,thick] (7.6,0.5) -- (null.west);
```

**Node structure.**

```cpp
template <typename T>
struct node {
    T data;
    std::unique_ptr<node> next;

    explicit node(const T& value) : data(value), next(nullptr) {}
    explicit node(T&& value) : data(std::move(value)), next(nullptr) {}
};
```

We use `std::unique_ptr` for owning pointers — when a node is destroyed, its `next` chain is automatically freed.

**Implementation.**

```cpp
template <typename T>
class linked_list {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    linked_list() = default;

    linked_list(std::initializer_list<T> init) {
        for (const auto& value : init) {
            push_back(value);
        }
    }

    linked_list(const linked_list& other) {
        node<T>* current = other.head_.get();
        node<T>** tail = &head_;
        while (current) {
            *tail = std::make_unique<node<T>>(current->data);
            tail = &(*tail)->next;
            current = current->next.get();
        }
        size_ = other.size_;
    }

    linked_list& operator=(const linked_list& other) {
        if (this != &other) {
            linked_list temp(other);
            std::swap(head_, temp.head_);
            std::swap(size_, temp.size_);
        }
        return *this;
    }

    linked_list(linked_list&&) = default;
    linked_list& operator=(linked_list&&) = default;

    reference front() {
        if (empty()) throw std::underflow_error("list is empty");
        return head_->data;
    }
    const_reference front() const {
        if (empty()) throw std::underflow_error("list is empty");
        return head_->data;
    }

    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void push_front(const T& value) {
        auto new_node = std::make_unique<node<T>>(value);
        new_node->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
    }

    void push_front(T&& value) {
        auto new_node = std::make_unique<node<T>>(std::move(value));
        new_node->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
    }

    void push_back(const T& value) {
        auto new_node = std::make_unique<node<T>>(value);
        node<T>** tail = get_tail_ptr();
        *tail = std::move(new_node);
        ++size_;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("list is empty");
        head_ = std::move(head_->next);
        --size_;
    }

    void clear() noexcept {
        head_.reset();
        size_ = 0;
    }

    reference operator[](size_type index) {
        node<T>* current = head_.get();
        for (size_type i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(node<T>* ptr) : ptr_(ptr) {}

        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &ptr_->data; }
        iterator& operator++() { ptr_ = ptr_->next.get(); return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

    private:
        node<T>* ptr_;
    };

    iterator begin() noexcept { return iterator(head_.get()); }
    iterator end() noexcept { return iterator(nullptr); }

private:
    node<T>** get_tail_ptr() {
        node<T>** p = &head_;
        while (*p) p = &(*p)->next;
        return p;
    }

    std::unique_ptr<node<T>> head_;
    size_type size_ = 0;
};
```

**Step-by-step walkthrough.** Building list L = {10, 20, 30}:

**Step 1: push_back(10)**
```
head → [10 | /]
```

**Step 2: push_back(20)**
```
head → [10 | ·] → [20 | /]
```

**Step 3: push_back(30)**
```
head → [10 | ·] → [20 | ·] → [30 | /]
```

**Step 4: pop_front()**
```
head → [20 | ·] → [30 | /]
// (10 is automatically freed since unique_ptr goes out of scope)
```

### Complexity Analysis

| Operation | Array List | Singly Linked List |
|-----------|------------|-------------------|
| `operator[]` | O(1) | O(n) |
| `push_front` | O(n) | O(1) |
| `push_back` | O(1) amortized | O(1) with tail ptr |
| `pop_front` | O(n) | O(1) |
| `pop_back` | O(1) | O(n) |
| `insert` at known position | O(n) | O(1) once positioned |
| space overhead | O(1) per list | O(n) for pointers |

**Key tradeoff:** Linked lists sacrifice O(1) random access for O(1) front insertions/deletions.

### Doubly Linked List

A doubly linked list has prev and next pointers, enabling O(1) deletion at both ends and bidirectional traversal.

```
null ← [10 | · | ·] ⇄ [20 | · | ·] ⇄ [30 | / | ·] → null
```

```cpp
template <typename T>
struct dnode {
    T data;
    std::unique_ptr<dnode> next;
    dnode* prev;

    explicit dnode(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};
```

**Doubly linked list with sentinel.** A sentinel (dummy header node) simplifies edge cases — every real node has a non-null predecessor:

```cpp
template <typename T>
class doubly_linked_list {
public:
    doubly_linked_list() : size_(0) {
        head_ = std::make_unique<dnode<T>>(T());
        tail_ = head_.get();
    }

    void push_front(const T& value) {
        auto new_node = std::make_unique<dnode<T>>(value);
        new_node->prev = head_.get();
        new_node->next = std::move(head_->next);
        if (new_node->next) {
            new_node->next->prev = new_node.get();
        } else {
            tail_ = new_node.get();
        }
        head_->next = std::move(new_node);
        ++size_;
    }

    void push_back(const T& value) {
        auto new_node = std::make_unique<dnode<T>>(value);
        new_node->prev = tail_;
        tail_->next = std::move(new_node);
        tail_ = tail_->next.get();
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("list is empty");
        dnode<T>* new_tail = tail_->prev;
        new_tail->next.reset();
        tail_ = new_tail;
        --size_;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }

private:
    std::unique_ptr<dnode<T>> head_;
    dnode<T>* tail_;
    size_type size_ = 0;
};
```

### Circular Linked Lists

A circular singly linked list has the last node pointing back to the head:

```tikz
\tikzstyle{nodebox}=[draw,rectangle,minimum width=1.8cm,minimum height=0.7cm,inner sep=2pt]
\node (headlabel) at (1,1) {\texttt{head}};
\node[nodebox] (n1) at (4,1) {\texttt{10}\hspace{0.8cm}\texttt{$\bullet$}};
\node[nodebox] (n2) at (7,1) {\texttt{20}\hspace{0.8cm}\texttt{$\bullet$}};
\node[nodebox] (n3) at (10,1) {\texttt{30}\hspace{0.8cm}\texttt{$\bullet$}};
\draw[->,>=stealth,thick] (headlabel.east) -- (n1.west);
\draw[->,>=stealth,thick] (3.1,1) -- (n2.west);
\draw[->,>=stealth,thick] (6.1,1) -- (n3.west);
\draw[->,>=stealth,thick,rounded corners] (9.1,1) -- (11.5,1) -- (11.5,0) -- (2.2,0) -- (2.2,1);
\draw[->,>=stealth,thick,rounded corners] (9.1,1) -- (11.5,1) -- (11.5,0) -- (2.2,0) -- (2.2,0.5);
% The simple curved version
\draw[->,>=stealth,thick,bend left=60] (n3.east) to (n1.west);
```

### Application: Polynomial Using Linked List

For sparse polynomials (few non-zero terms), a linked representation is more space-efficient than the dense array representation.

```cpp
struct term {
    int exponent;
    double coefficient;
};

using polynomial = linked_list<term>;

linked_list<term> add_polynomials(const linked_list<term>& a,
                                  const linked_list<term>& b) {
    linked_list<term> result;
    auto it_a = a.begin();
    auto it_b = b.begin();

    while (it_a != a.end() && it_b != b.end()) {
        if (it_a->exponent == it_b->exponent) {
            double sum = it_a->coefficient + it_b->coefficient;
            if (sum != 0.0) {
                result.push_back({it_a->exponent, sum});
            }
            ++it_a; ++it_b;
        } else if (it_a->exponent > it_b->exponent) {
            result.push_back({it_a->exponent, it_a->coefficient});
            ++it_a;
        } else {
            result.push_back({it_b->exponent, it_b->coefficient});
            ++it_b;
        }
    }

    while (it_a != a.end()) {
        result.push_back({it_a->exponent, it_a->coefficient});
        ++it_a;
    }
    while (it_b != b.end()) {
        result.push_back({it_b->exponent, it_b->coefficient});
        ++it_b;
    }

    return result;
}
```

**Example:** Add 3x⁵ - 2x² + 7 and 5x³ + 2x² - 1
```
A: (5, 3) → (2, -2) → (0, 7)
B: (3, 5) → (2, 2) → (0, -1)
Result: (5, 3) → (3, 5) → (2, 0) → (0, 6)
                    ← skip zero term →
Final:  (5, 3) → (3, 5) → (0, 6)
= 3x⁵ + 5x³ + 6
```

### Application: Undo/Redo System

A text editor's undo system uses a doubly linked list of states (or commands):

```cpp
struct edit_action {
    std::string description;
    std::string previous_text;
    std::string new_text;
    size_t position;
};

class undo_manager {
public:
    void record_action(edit_action action) {
        while (current_ != undo_list_.end()) {
            current_ = undo_list_.erase(current_);
        }
        undo_list_.push_back(std::move(action));
        current_ = undo_list_.end();

        if (undo_list_.size() > max_history_) {
            undo_list_.pop_front();
        }
    }

    edit_action undo() {
        if (current_ == undo_list_.begin()) {
            throw std::runtime_error("nothing to undo");
        }
        --current_;
        return *current_;
    }

    edit_action redo() {
        if (current_ == undo_list_.end()) {
            throw std::runtime_error("nothing to redo");
        }
        auto action = *current_;
        ++current_;
        return action;
    }

private:
    linked_list<edit_action> undo_list_;
    linked_list<edit_action>::iterator current_;
    static constexpr size_t max_history_ = 100;
};
```

### Memory Overhead

Each node in a singly linked list stores one pointer (8 bytes on 64-bit). For a doubly linked list: two pointers (16 bytes). For a list of 1 million integers:

| Representation | Memory (data) | Pointer overhead | Total |
|---------------|---------------|------------------|-------|
| `std::vector<int>` | 4 MB | None | ~4 MB |
| Singly linked list | 4 MB | 8 MB | ~12 MB |
| Doubly linked list | 4 MB | 16 MB | ~20 MB |

Linked lists are not cache-friendly — nodes are spread across memory. Array-based lists iterate faster even when operations are theoretically the same complexity.

### STL Connection: std::forward_list and std::list

- **`std::forward_list`** — singly linked list. Provides O(1) insert/erase after a given position. Does not track `size()` (to save space). Uses `insert_after`, `erase_after` rather than `insert`, `erase`.
- **`std::list`** — doubly linked list. Provides O(1) insert/erase anywhere with an iterator. Tracks size.

```cpp
std::forward_list<int> fl = {1, 2, 3};
fl.push_front(0);
auto it = fl.begin(); ++it;
fl.insert_after(it, 5);                 // {0, 1, 5, 2, 3}
fl.erase_after(it);                     // {0, 1, 2, 3}

std::list<int> l = {1, 2, 3};
l.push_front(0);
l.push_back(4);
l.pop_front();
l.pop_back();
```

---

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Off-by-one in array index — using size vs size-1 | Out-of-bounds access, UB or segmentation fault | Remember: valid indices are [0, size) |
| Forgetting to resize the array when the list is full | Buffer overflow on next insert | Check size == capacity before inserting |
| Iterator invalidation after `insert`/`erase` on `std::vector` | Use-after-free, non-deterministic crashes | Re-acquire iterator after modifying the vector |
| Shallow copy of linked list nodes | Double free on destruction | Implement proper copy constructor / assignment |
| Dangling `tail` pointer after `pop_back` on singly linked list | Tail points to freed node | Update tail to new last node, or use doubly linked |
| Calling front/pop on an empty container | UB — no checking in standard containers | Always check `!empty()` before front/pop |

## Summary

1. A **linear list** is an ordered sequence with position-based access.
2. **Array representation** provides O(1) access by index but O(n) inserts in the middle.
3. **Dynamic arrays** grow geometrically to amortize the cost of resizing.
4. **Singly linked lists** provide O(1) insert/delete at the front but O(n) random access.
5. **Doubly linked lists** add O(1) operations at both ends and bidirectional traversal.
6. **Sentinels** simplify edge cases by guaranteeing every node has a predecessor.
7. **Smart pointers** (`std::unique_ptr`) make linked list memory management automatic and exception-safe.
8. **Memory overhead** and poor cache locality make linked lists slower than arrays in practice, despite theoretical complexity advantages.
9. Use linked lists when: (a) frequent front insertions/deletions, (b) iterator stability needed, or (c) splicing lists.
10. `std::vector` is the production-quality version of the dynamic array; `std::forward_list` and `std::list` are the standard linked list containers.

## Exercises

### Drill

1. Trace the state of a `dynamic_array_list` initially empty after the following sequence of operations:
   `push_back(3), push_back(1), push_back(4), insert(1, 2), erase(3), push_back(5), pop_back(), insert(0, 0)`
   Show the array contents and size after each step. Assume initial capacity = 2.

2. What is the amortized complexity of `push_back` on a dynamic array that grows by a factor of 2? Prove your answer.

3. Given a `dynamic_array_list<int>` of size n, what is the worst-case complexity of:
   a) `operator[]`   b) `push_back`   c) `insert` at position 0   d) `insert` at position n-1   e) `erase` at position n/2

4. Draw the singly linked list after each step of the following operations on an initially empty list:
   `push_front(5), push_front(3), push_back(7), insert(1, 4), erase(0), pop_front(), push_back(9)`
   Show the chain nodes and pointers.

5. Trace the execution of `reverse()` on a singly linked list containing [1, 2, 3, 4]. Draw the state of the pointers after each iteration.

6. What is the worst-case time complexity of:
   a) `push_front` on a singly linked list
   b) `push_back` on a singly linked list without a tail pointer
   c) `push_back` on a singly linked list with a tail pointer
   d) `insert` at position k in a doubly linked list
   e) `erase` by value (removing the first match) in a singly linked list

7. Explain why a sentinel node simplifies the `erase` operation in a singly linked list. Show the code for `erase` with and without a sentinel.

### Application

8. Modify `dynamic_array_list` to use a growth factor of 1.5 instead of 2. Benchmark both for 10^6 `push_back` operations. Which is faster? Which wastes less memory?

9. Add a `reserve(size_type n)` method that pre-allocates capacity. Add a `shrink_to_fit()` method that reduces capacity to match size.

10. Implement `operator==` and `operator<=>` (C++20 three-way comparison) for `dynamic_array_list`.

11. Implement an STL-style iterator for `dynamic_array_list`. Support `operator*`, `operator++`, `operator!=`, `operator--`. Test with a range-based for loop.

12. Implement a `circular_buffer<T>` of fixed capacity using a dynamic array. Support `push_back`, `pop_front`, `front`, `back`, `size`, `empty`, and `operator[]`.

13. Implement `sparse_polynomial` using a sorted list of (exponent, coefficient) pairs. Compare its space and time efficiency with the dense representation.

14. Implement `erase(const T& value)` that removes the first occurrence of value from the singly linked list.

15. Implement `insert_after(iterator pos, const T& value)` and `erase_after(iterator pos)` for the singly linked list.

16. Implement `merge(other)` that merges two sorted singly linked lists into one sorted list, borrowing nodes from `other`.

17. Implement `reverse()` for the singly linked list that reverses the links in O(n) time and O(1) extra space.

18. Implement the Josephus problem using a circular linked list: n people stand in a circle, every k-th is eliminated. Find the survivor.

19. Implement a `split(list& front, list& back)` method that splits a singly linked list into two halves using the slow/fast pointer technique.

20. Write a function `detect_cycle(const forward_list&)` that determines whether a linked list has a cycle (Floyd's tortoise and hare algorithm).

### Research

21. **(vector<bool>)** `std::vector<bool>` is a specialization that packs bits. Research its design. What are the pros and cons?

22. **(Growth factor analysis)** Study the impact of different growth factors (1.25, 1.5, 2.0, 3.0) on memory fragmentation. Read about the "golden ratio" growth factor.

23. **(Small vector optimization)** Study how `std::pmr::vector` or `folly::small_vector` uses inline storage for small sizes.

24. **(Allocator-aware containers)** Read about `std::scoped_allocator_adaptor`. Implement an `array_list` that accepts a custom allocator.

25. **(Memory locality)** Benchmark iteration over `std::vector`, `std::list`, and `std::forward_list` for n = 10^6 elements. Use tools to measure cache misses.

26. **(Intrusive lists)** Research intrusive linked lists (used in the Linux kernel and Boost.Intrusive).

27. **(Lock-free lists)** Read about lock-free linked lists (Michael and Scott, 1996). Implement a concurrent singly linked list using `std::atomic`.

## References and Further Reading

- Bjarne Stroustrup, *The C++ Programming Language*, 4th Edition. Sections 31.3–31.4.
- Nicolai M. Josuttis, *The C++ Standard Library*, 2nd Edition. Chapters 7, 9, and 10.
- Herb Sutter, "vector<bool>: More Problems, Better Solutions" — GotW #86.
- Donald E. Knuth, *The Art of Computer Programming*, Volume 1: Fundamental Algorithms. Section 2.2.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Sections 10.2–10.3.
- Maged M. Michael and Michael L. Scott, "Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms" — PODC 1996.
- Raymond Chen, "The Old New Thing" — various posts on linked list pitfalls.
- ISO C++ Committee, "P0404R0: Wording for out-of-thin-air resolution" (on vector growth factor).
- Facebook Folly library, `small_vector` implementation — https://github.com/facebook/folly
- Boost.Intrusive documentation — https://www.boost.org/doc/libs/release/doc/html/intrusive.html
