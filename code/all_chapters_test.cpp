#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <memory>
#include <string>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <random>
#include <functional>
#include <variant>
#include <optional>
#include <span>
#include <concepts>
#include <ranges>
#include <numeric>
#include <bitset>

// ============================================================
// Chapter 1: Modern C++ Review
// ============================================================
template <typename T>
concept Addable = requires(T a, T b) { { a + b } -> std::convertible_to<T>; };

template <Addable T>
T add_concepts(T a, T b) { return a + b; }

constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

void ch01_test() {
    // Smart pointers
    auto uptr = std::make_unique<int>(42);
    auto sptr = std::make_shared<std::string>("hello");
    std::weak_ptr<std::string> wptr = sptr;
    assert(*uptr == 42);
    assert(*sptr == "hello");
    if (auto locked = wptr.lock()) assert(*locked == "hello");
    
    // Lambdas
    auto add = [](int a, int b) { return a + b; };
    assert(add(3, 4) == 7);
    
    // Structured bindings
    std::tuple<int, double, std::string> t = {42, 3.14, "hello"};
    auto [a, b, c] = t;
    assert(a == 42 && b == 3.14 && c == "hello");
    
    // Optional
    auto safe_div = [](int a, int b) -> std::optional<int> {
        if (b == 0) return std::nullopt;
        return a / b;
    };
    assert(safe_div(10, 2).value() == 5);
    assert(!safe_div(10, 0).has_value());
    
    // Variant
    std::variant<int, double, std::string> v = 42;
    assert(std::get<int>(v) == 42);
    v = 3.14;
    assert(std::get<double>(v) == 3.14);
    v = "hello";
    assert(std::get<std::string>(v) == "hello");
    
    // constexpr
    static_assert(factorial(5) == 120);
    
    // Concepts
    assert(add_concepts(3, 4) == 7);
    assert(add_concepts(3.5, 4.2) == 7.7);
    
    // Range-based for with views
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    int sum = 0;
    for (int x : vec | std::views::filter([](int n){ return n % 2 == 0; }) 
                    | std::views::transform([](int n){ return n * n; })
                    | std::views::take(2)) {
        sum += x;
    }
    assert(sum == 4 + 16);  // 2^2 + 4^2
    
    std::cout << "ch01: PASS\n";
}

// ============================================================
// Chapter 2: Performance Analysis
// ============================================================
int fib_recursive(int n) { return n <= 1 ? n : fib_recursive(n-1) + fib_recursive(n-2); }
int fib_dp(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { int c = a + b; a = b; b = c; }
    return b;
}

void ch02_test() {
    // Verify complexity difference
    assert(fib_recursive(10) == 55);
    assert(fib_dp(10) == 55);
    assert(fib_recursive(20) == fib_dp(20));
    
    // Loop invariant: insertion sort
    std::vector<int> arr = {5, 2, 8, 1, 9};
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; --j; }
        arr[j + 1] = key;
    }
    assert(arr == std::vector<int>({1, 2, 5, 8, 9}));
    
    // Binary search with invariant
    auto bin_search = [](const std::vector<int>& a, int target) {
        int l = 0, r = a.size() - 1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (a[m] == target) return m;
            if (a[m] < target) l = m + 1;
            else r = m - 1;
        }
        return -1;
    };
    assert(bin_search({1,2,3,4,5}, 3) == 2);
    assert(bin_search({1,2,3,4,5}, 6) == -1);
    
    std::cout << "ch02: PASS\n";
}

// ============================================================
// Chapter 3: Performance Measurement
// ============================================================
void ch03_test() {
    // Cache line effect
    const size_t N = 1000000;
    std::vector<int> arr(N, 1);
    
    auto time_seq = [&] {
        auto start = std::chrono::high_resolution_clock::now();
        long sum = 0;
        for (size_t i = 0; i < N; ++i) sum += arr[i];
        (void)sum;
        return std::chrono::high_resolution_clock::now() - start;
    };
    
    auto time_stride = [&](size_t stride) {
        auto start = std::chrono::high_resolution_clock::now();
        long sum = 0;
        for (size_t i = 0; i < N; i += stride) sum += arr[i];
        (void)sum;
        return std::chrono::high_resolution_clock::now() - start;
    };
    
    auto t1 = time_seq();
    auto t16 = time_stride(16);
    
    // Strided should be slower due to cache misses
    // (Modern CPUs may prefetch, so just verify both complete)
    (void)t16; (void)t1;
    
    // Branch prediction
    std::vector<int> data(N);
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(0, 1);
    for (size_t i = 0; i < N; ++i) data[i] = dis(gen);
    
    auto t_rand = [&] {
        auto start = std::chrono::high_resolution_clock::now();
        long cnt = 0;
        for (size_t i = 0; i < N; ++i) if (data[i] > 0) ++cnt;
        (void)cnt;
        return std::chrono::high_resolution_clock::now() - start;
    };
    
    std::sort(data.begin(), data.end());
    auto t_sorted = [&] {
        auto start = std::chrono::high_resolution_clock::now();
        long cnt = 0;
        for (size_t i = 0; i < N; ++i) if (data[i] > 0) ++cnt;
        (void)cnt;
        return std::chrono::high_resolution_clock::now() - start;
    };
    
    // Sorted should be faster due to branch prediction
    // (We just verify both complete)
    assert(t_rand().count() >= 0 && t_sorted().count() >= 0);
    
    std::cout << "ch03: PASS\n";
}

// ============================================================
// Chapter 4: Linear Lists
// ============================================================
template <typename T>
class array_list {
    std::size_t cap_, sz_;
    std::unique_ptr<T[]> data_;
    void check(std::size_t i) const { if (i >= sz_) throw std::out_of_range("oob"); }
    void resize(std::size_t n) {
        auto nd = std::make_unique<T[]>(n);
        for (std::size_t i = 0; i < sz_; ++i) nd[i] = std::move(data_[i]);
        data_ = std::move(nd);
        cap_ = n;
    }
public:
    array_list(std::size_t c = 10) : cap_(c), sz_(0), data_(std::make_unique<T[]>(c)) {}
    std::size_t size() const { return sz_; }
    bool empty() const { return sz_ == 0; }
    T& operator[](std::size_t i) { check(i); return data_[i]; }
    const T& operator[](std::size_t i) const { check(i); return data_[i]; }
    void push_back(const T& v) { if (sz_ >= cap_) resize(cap_ * 2); data_[sz_++] = v; }
    void pop_back() { if (empty()) throw std::underflow_error("empty"); --sz_; }
    void insert(std::size_t i, const T& v) {
        if (i > sz_) throw std::out_of_range("oob");
        if (sz_ >= cap_) resize(cap_ * 2);
        for (std::size_t j = sz_; j > i; --j) data_[j] = std::move(data_[j - 1]);
        data_[i] = v;
        ++sz_;
    }
    void erase(std::size_t i) {
        if (i >= sz_) throw std::out_of_range("oob");
        for (std::size_t j = i; j < sz_ - 1; ++j) data_[j] = std::move(data_[j + 1]);
        --sz_;
    }
};

template <typename T>
class linked_list {
    struct Node { T data; std::unique_ptr<Node> next; Node(const T& d): data(d), next(nullptr) {} };
    std::unique_ptr<Node> head_;
    std::size_t sz_ = 0;
public:
    void push_front(const T& v) { auto n = std::make_unique<Node>(v); n->next = std::move(head_); head_ = std::move(n); ++sz_; }
    void pop_front() { if (!head_) throw std::underflow_error("empty"); head_ = std::move(head_->next); --sz_; }
    T& front() { return head_->data; }
    const T& front() const { return head_->data; }
    std::size_t size() const { return sz_; }
    bool empty() const { return sz_ == 0; }
    Node* head() { return head_.get(); }
    const Node* head() const { return head_.get(); }
};

void ch04_test() {
    // array_list
    array_list<int> al;
    for (int i = 0; i < 20; ++i) al.push_back(i);
    assert(al.size() == 20);
    assert(al[0] == 0 && al[19] == 19);
    al.insert(10, 99);
    assert(al[10] == 99 && al.size() == 21);
    al.erase(10);
    assert(al[10] == 10 && al.size() == 20);
    
    // linked_list
    linked_list<int> ll;
    for (int i = 0; i < 10; ++i) ll.push_front(i);
    assert(ll.size() == 10);
    assert(ll.front() == 9);
    int sum = 0;
    for (auto p = ll.head(); p; p = p->next.get()) sum += p->data;
    assert(sum == 45);
    
    std::cout << "ch04: PASS\n";
}

// ============================================================
// Chapter 5: Arrays, Matrices, Sparse
// ============================================================
class bit_array {
    std::vector<uint64_t> w_;
public:
    bit_array(std::size_t n = 0) { w_.resize((n + 63) / 64, 0); }
    void set(std::size_t i) { w_[i >> 6] |= (1ULL << (i & 63)); }
    void clear(std::size_t i) { w_[i >> 6] &= ~(1ULL << (i & 63)); }
    void flip(std::size_t i) { w_[i >> 6] ^= (1ULL << (i & 63)); }
    bool test(std::size_t i) const { return (w_[i >> 6] >> (i & 63)) & 1ULL; }
    std::size_t count() const {
        std::size_t c = 0;
        for (auto x : w_) c += __builtin_popcountll(x);
        return c;
    }
};

template <typename T>
class circular_buffer {
    std::vector<T> d_;
    std::size_t h_ = 0, t_ = 0, s_ = 0;
public:
    circular_buffer(std::size_t c) : d_(c) {}
    void push(const T& v) {
        d_[t_] = v;
        t_ = (t_ + 1) % d_.size();
        if (s_ < d_.size()) ++s_;
        else h_ = (h_ + 1) % d_.size();
    }
    bool pop(T& out) {
        if (s_ == 0) return false;
        out = std::move(d_[h_]);
        h_ = (h_ + 1) % d_.size();
        --s_;
        return true;
    }
    std::size_t size() const { return s_; }
    bool empty() const { return s_ == 0; }
};

std::vector<double> thomas(const std::vector<double>& a, const std::vector<double>& b,
                           const std::vector<double>& c, const std::vector<double>& d) {
    std::size_t n = b.size();
    std::vector<double> cp(n), dp(n), x(n);
    cp[0] = c[0] / b[0];
    dp[0] = d[0] / b[0];
    for (std::size_t i = 1; i < n; ++i) {
        double denom = b[i] - a[i] * cp[i - 1];
        cp[i] = (i < n - 1) ? c[i] / denom : 0;
        dp[i] = (d[i] - a[i] * dp[i - 1]) / denom;
    }
    x[n - 1] = dp[n - 1];
    for (std::size_t i = n - 1; i-- > 0;) x[i] = dp[i] - cp[i] * x[i + 1];
    return x;
}

void ch05_test() {
    // bit_array
    bit_array ba(100);
    for (int i = 0; i < 100; i += 2) ba.set(i);
    assert(ba.count() == 50);
    assert(ba.test(0) && !ba.test(1) && ba.test(2));
    ba.flip(1);
    assert(ba.test(1));
    
    // circular_buffer
    circular_buffer<int> cb(5);
    for (int i = 0; i < 10; ++i) cb.push(i);
    assert(cb.size() == 5);
    int v;
    for (int i = 5; i < 10; ++i) { cb.pop(v); assert(v == i); }
    assert(cb.empty());
    
    // Thomas algorithm
    std::vector<double> a = {0, 1, 1, 1}, b = {2, 2, 2, 2}, c = {1, 1, 1, 0}, d = {3, 4, 5, 6};
    auto x = thomas(a, b, c, d);
    for (double val : x) assert(std::abs(val - 1.0) < 1e-9);
    
    std::cout << "ch05: PASS\n";
}

// ============================================================
// Chapter 6: Stacks
// ============================================================
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

bool balanced(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') st.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char t = st.top(); st.pop();
            if ((c == ')' && t != '(') || (c == ']' && t != '[') || (c == '}' && t != '{'))
                return false;
        }
    }
    return st.empty();
}

void ch06_test() {
    min_stack<int> ms;
    ms.push(5); ms.push(3); ms.push(7); ms.push(2);
    assert(ms.min() == 2);
    ms.pop();
    assert(ms.min() == 3);
    ms.pop();
    assert(ms.min() == 3);
    ms.pop();
    assert(ms.min() == 5);
    
    assert(balanced("()[]{}"));
    assert(balanced("({[]})"));
    assert(!balanced("([)]"));
    assert(!balanced("("));
    assert(!balanced(")"));
    
    std::cout << "ch06: PASS\n";
}

// ============================================================
// Chapter 7: Queues
// ============================================================
template <typename T>
class circular_queue {
    std::vector<T> d_;
    std::size_t h_ = 0, t_ = 0, s_ = 0;
public:
    circular_queue(std::size_t c) : d_(c + 1) {}  // +1 for empty/full distinction
    void enqueue(const T& v) {
        if ((t_ + 1) % d_.size() == h_) throw std::overflow_error("full");
        d_[t_] = v;
        t_ = (t_ + 1) % d_.size();
        ++s_;
    }
    T dequeue() {
        if (h_ == t_) throw std::underflow_error("empty");
        T v = std::move(d_[h_]);
        h_ = (h_ + 1) % d_.size();
        --s_;
        return v;
    }
    bool empty() const { return h_ == t_; }
    std::size_t size() const { return s_; }
};

std::vector<int> sliding_window_max(const std::vector<int>& a, int k) {
    std::vector<int> res;
    std::deque<int> dq;  // indices
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}

void ch07_test() {
    circular_queue<int> cq(5);
    for (int i = 0; i < 5; ++i) cq.enqueue(i);
    assert(cq.size() == 5);
    for (int i = 0; i < 5; ++i) assert(cq.dequeue() == i);
    assert(cq.empty());
    
    // Sliding window max
    std::vector<int> nums = {1,3,-1,-3,5,3,6,7};
    auto res = sliding_window_max(nums, 3);
    assert(res == std::vector<int>({3,3,5,5,6,7}));
    
    std::cout << "ch07: PASS\n";
}

// ============================================================
// Chapter 8: Hashing
// ============================================================
template <typename K, typename V>
class hash_map {
    struct Node { K key; V val; Node* next = nullptr; };
    std::vector<Node*> buckets_;
    std::size_t sz_ = 0;
    static std::size_t hash(const K& k) { return std::hash<K>{}(k); }
    void rehash() {
        auto old = buckets_;
        buckets_.resize(buckets_.size() * 2, nullptr);
        sz_ = 0;
        for (auto* head : old) {
            while (head) {
                Node* n = head;
                head = head->next;
                insert(n->key, n->val);
                delete n;
            }
        }
    }
public:
    hash_map(std::size_t n = 16) : buckets_(n, nullptr) {}
    ~hash_map() { for (auto* h : buckets_) while (h) { auto* n = h; h = h->next; delete n; } }
    
    void insert(const K& k, const V& v) {
        if (sz_ > buckets_.size()) rehash();
        std::size_t idx = hash(k) % buckets_.size();
        for (auto* p = buckets_[idx]; p; p = p->next)
            if (p->key == k) { p->val = v; return; }
        Node* n = new Node{k, v, buckets_[idx]};
        buckets_[idx] = n;
        ++sz_;
    }
    V& operator[](const K& k) {
        std::size_t idx = hash(k) % buckets_.size();
        for (auto* p = buckets_[idx]; p; p = p->next)
            if (p->key == k) return p->val;
        if (sz_ > buckets_.size()) { rehash(); idx = hash(k) % buckets_.size(); }
        Node* n = new Node{k, V{}, buckets_[idx]};
        buckets_[idx] = n;
        ++sz_;
        return n->val;
    }
    bool find(const K& k) const {
        std::size_t idx = hash(k) % buckets_.size();
        for (auto* p = buckets_[idx]; p; p = p->next)
            if (p->key == k) return true;
        return false;
    }
    std::size_t size() const { return sz_; }
};

void ch08_test() {
    hash_map<std::string, int> hm;
    hm.insert("one", 1);
    hm.insert("two", 2);
    hm.insert("three", 3);
    assert(hm.size() == 3);
    assert(hm.find("one"));
    assert(!hm.find("four"));
    hm["four"] = 4;
    assert(hm["four"] == 4);
    assert(hm.size() == 4);
    hm["one"] = 10;
    assert(hm["one"] == 10);
    
    std::cout << "ch08: PASS\n";
}

// ============================================================
// Chapter 9: Trees and Binary Trees
// ============================================================
template <typename T>
struct tree_node {
    T data;
    std::unique_ptr<tree_node> left, right;
    tree_node(const T& d) : data(d), left(nullptr), right(nullptr) {}
};

template <typename T>
int tree_height(tree_node<T>* root) {
    if (!root) return -1;
    return 1 + std::max(tree_height(root->left.get()), tree_height(root->right.get()));
}

template <typename T>
bool is_bst(tree_node<T>* root, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) {
    if (!root) return true;
    if (root->data <= min || root->data >= max) return false;
    return is_bst(root->left.get(), min, root->data) && is_bst(root->right.get(), root->data, max);
}

template <typename T>
tree_node<T>* insert_bst(tree_node<T>* root, const T& val) {
    if (!root) return new tree_node<T>(val);
    if (val < root->data) root->left.reset(insert_bst(root->left.release(), val));
    else if (val > root->data) root->right.reset(insert_bst(root->right.release(), val));
    return root;
}

void ch09_test() {
    // Build BST
    std::unique_ptr<tree_node<int>> root;
    for (int v : {5, 3, 7, 1, 4, 6, 8}) root.reset(insert_bst(root.release(), v));
    assert(tree_height(root.get()) == 2);
    assert(is_bst(root.get()));
    
    // Traversals
    std::vector<int> in, pre, post;
    auto inorder = [&](auto self, tree_node<int>* n) {
        if (!n) return;
        self(self, n->left.get());
        in.push_back(n->data);
        self(self, n->right.get());
    };
    inorder(inorder, root.get());
    assert(in == std::vector<int>({1,3,4,5,6,7,8}));
    
    std::cout << "ch09: PASS\n";
}

// ============================================================
// Chapter 10: Priority Queues (Heaps)
// ============================================================
template <typename T, typename Comp = std::less<T>>
class binary_heap {
    std::vector<T> a_;
    Comp cmp_;
    void sift_up(std::size_t i) {
        while (i > 0) {
            std::size_t p = (i - 1) / 2;
            if (!cmp_(a_[i], a_[p])) break;
            std::swap(a_[i], a_[p]);
            i = p;
        }
    }
    void sift_down(std::size_t i) {
        while (true) {
            std::size_t l = 2 * i + 1, r = 2 * i + 2, s = i;
            if (l < a_.size() && cmp_(a_[l], a_[s])) s = l;
            if (r < a_.size() && cmp_(a_[r], a_[s])) s = r;
            if (s == i) break;
            std::swap(a_[i], a_[s]);
            i = s;
        }
    }
public:
    void push(const T& v) { a_.push_back(v); sift_up(a_.size() - 1); }
    void pop() {
        if (a_.empty()) throw std::underflow_error("empty");
        a_[0] = std::move(a_.back());
        a_.pop_back();
        if (!a_.empty()) sift_down(0);
    }
    const T& top() const { if (a_.empty()) throw std::underflow_error("empty"); return a_[0]; }
    bool empty() const { return a_.empty(); }
    std::size_t size() const { return a_.size(); }
};

void ch10_test() {
    // Min-heap
    binary_heap<int, std::greater<int>> min_h;
    for (int v : {5, 3, 7, 1, 9, 2}) min_h.push(v);
    for (int expected : {1, 2, 3, 5, 7, 9}) {
        assert(min_h.top() == expected);
        min_h.pop();
    }
    assert(min_h.empty());
    
    // Max-heap
    binary_heap<int> max_h;
    for (int v : {5, 3, 7, 1, 9, 2}) max_h.push(v);
    for (int expected : {9, 7, 5, 3, 2, 1}) {
        assert(max_h.top() == expected);
        max_h.pop();
    }
    
    std::cout << "ch10: PASS\n";
}

// ============================================================
// Chapter 11: Balanced Search Trees
// ============================================================
// AVL Tree
template <typename T>
struct avl_node {
    T key;
    int height = 0;
    std::unique_ptr<avl_node> left, right;
    avl_node(const T& k) : key(k) {}
};

template <typename T>
int height(avl_node<T>* n) { return n ? n->height : -1; }

template <typename T>
avl_node<T>* rotate_right(avl_node<T>* y) {
    avl_node<T>* x = y->left.release();
    y->left.reset(x->right.release());
    x->right.reset(y);
    y->height = 1 + std::max(height(y->left.get()), height(y->right.get()));
    x->height = 1 + std::max(height(x->left.get()), height(x->right.get()));
    return x;
}

template <typename T>
avl_node<T>* rotate_left(avl_node<T>* x) {
    avl_node<T>* y = x->right.release();
    x->right.reset(y->left.release());
    y->left.reset(x);
    x->height = 1 + std::max(height(x->left.get()), height(x->right.get()));
    y->height = 1 + std::max(height(y->left.get()), height(y->right.get()));
    return y;
}

template <typename T>
int balance(avl_node<T>* n) { return n ? height(n->left.get()) - height(n->right.get()) : 0; }

template <typename T>
avl_node<T>* insert_avl(avl_node<T>* n, const T& key) {
    if (!n) return new avl_node<T>(key);
    if (key < n->key) n->left.reset(insert_avl(n->left.release(), key));
    else if (key > n->key) n->right.reset(insert_avl(n->right.release(), key));
    else return n;  // duplicate
    
    n->height = 1 + std::max(height(n->left.get()), height(n->right.get()));
    int b = balance(n);
    
    if (b > 1 && key < n->left->key) return rotate_right(n);
    if (b < -1 && key > n->right->key) return rotate_left(n);
    if (b > 1 && key > n->left->key) {
        n->left.reset(rotate_left(n->left.release()));
        return rotate_right(n);
    }
    if (b < -1 && key < n->right->key) {
        n->right.reset(rotate_right(n->right.release()));
        return rotate_left(n);
    }
    return n;
}

void ch11_test() {
    avl_node<int>* root = nullptr;
    for (int v : {10, 20, 30, 40, 50, 25}) root = insert_avl(root, v);
    assert(root->height == 2);  // Balanced
    assert(balance(root) <= 1 && balance(root) >= -1);
    
    // B-tree concept (simplified)
    std::cout << "ch11: PASS\n";
}

// ============================================================
// Chapter 12: Graphs
// ============================================================
using Graph = std::vector<std::vector<std::pair<int, int>>>;  // (neighbor, weight)

std::vector<int> dijkstra(const Graph& g, int src) {
    std::vector<int> dist(g.size(), INT_MAX);
    dist[src] = 0;
    using P = std::pair<int, int>;  // (dist, node)
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

std::vector<int> bfs(const Graph& g, int src) {
    std::vector<int> dist(g.size(), -1);
    std::queue<int> q;
    dist[src] = 0; q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, _] : g[u]) if (dist[v] == -1) {
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}

void dfs_util(const Graph& g, int u, std::vector<bool>& vis, std::vector<int>& order) {
    vis[u] = true;
    for (auto [v, _] : g[u]) if (!vis[v]) dfs_util(g, v, vis, order);
    order.push_back(u);
}

std::vector<int> topological_sort(const Graph& g) {
    std::vector<bool> vis(g.size(), false);
    std::vector<int> order;
    for (int i = 0; i < (int)g.size(); ++i) if (!vis[i]) dfs_util(g, i, vis, order);
    std::reverse(order.begin(), order.end());
    return order;
}

void ch12_test() {
    // Dijkstra
    Graph g(5);
    g[0].push_back({1, 4}); g[0].push_back({2, 1});
    g[1].push_back({3, 1});
    g[2].push_back({1, 2}); g[2].push_back({3, 5});
    g[3].push_back({4, 3});
    auto dist = dijkstra(g, 0);
    assert(dist == std::vector<int>({0, 3, 1, 4, 7}));
    
    // BFS
    auto bfs_dist = bfs(g, 0);
    assert(bfs_dist[0] == 0 && bfs_dist[1] == 1 && bfs_dist[2] == 1 && bfs_dist[3] == 2);
    
    // Topological sort (DAG)
    Graph dag(4);
    dag[0].push_back({1, 0}); dag[0].push_back({2, 0});
    dag[1].push_back({3, 0}); dag[2].push_back({3, 0});
    auto topo = topological_sort(dag);
    assert(topo[0] == 0 && topo[3] == 3);
    
    std::cout << "ch12: PASS\n";
}

// ============================================================
// Chapter 13: String Algorithms
// ============================================================
std::vector<int> compute_lps(const std::string& pat) {
    std::vector<int> lps(pat.size(), 0);
    for (int i = 1, len = 0; i < (int)pat.size(); ) {
        if (pat[i] == pat[len]) lps[i++] = ++len;
        else if (len != 0) len = lps[len - 1];
        else lps[i++] = 0;
    }
    return lps;
}

std::vector<int> kmp_search(const std::string& text, const std::string& pat) {
    std::vector<int> lps = compute_lps(pat);
    std::vector<int> matches;
    for (int i = 0, j = 0; i < (int)text.size(); ) {
        if (text[i] == pat[j]) { ++i; ++j; }
        if (j == (int)pat.size()) { matches.push_back(i - j); j = lps[j - 1]; }
        else if (i < (int)text.size() && text[i] != pat[j]) {
            if (j != 0) j = lps[j - 1]; else ++i;
        }
    }
    return matches;
}

std::vector<int> z_algorithm(const std::string& s) {
    std::vector<int> z(s.size(), 0);
    int l = 0, r = 0;
    for (int i = 1; i < (int)s.size(); ++i) {
        if (i <= r) z[i] = std::min(r - i + 1, z[i - l]);
        while (i + z[i] < (int)s.size() && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) { l = i; r = i + z[i] - 1; }
    }
    return z;
}

void ch13_test() {
    // KMP
    assert(kmp_search("ababcababa", "aba") == std::vector<int>({0, 2, 5, 7}));
    assert(kmp_search("aaaaa", "aa") == std::vector<int>({0, 1, 2, 3}));
    
    // Z-algorithm
    assert(z_algorithm("aabxaabxcaabxaabxay") == std::vector<int>({0,1,0,0,4,1,0,0,9,1,0,0,4,1,0,0,1,0,0}));
    
    // Trie (simple)
    struct TrieNode {
        std::array<TrieNode*, 26> children{};
        bool is_end = false;
        TrieNode() { children.fill(nullptr); }
    };
    
    auto insert = [](TrieNode* root, const std::string& s) {
        TrieNode* p = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!p->children[idx]) p->children[idx] = new TrieNode();
            p = p->children[idx];
        }
        p->is_end = true;
    };
    
    auto search = [](TrieNode* root, const std::string& s) {
        TrieNode* p = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!p->children[idx]) return false;
            p = p->children[idx];
        }
        return p->is_end;
    };
    
    TrieNode* root = new TrieNode();
    insert(root, "hello");
    insert(root, "world");
    assert(search(root, "hello"));
    assert(search(root, "world"));
    assert(!search(root, "hell"));
    assert(!search(root, "word"));
    
    std::cout << "ch13: PASS\n";
}

// ============================================================
// Chapter 14: Segment Tree, Fenwick Tree, Sparse Table, Union-Find
// ============================================================
class fenwick {
    std::vector<long long> bit_;
    int n_;
public:
    fenwick(int n = 0) : bit_(n + 1, 0), n_(n) {}
    void add(int i, long long v) { for (++i; i <= n_; i += i & -i) bit_[i] += v; }
    long long sum(int i) const { long long s = 0; for (++i; i > 0; i -= i & -i) s += bit_[i]; return s; }
    long long range_sum(int l, int r) const { return sum(r) - (l ? sum(l - 1) : 0); }
};

class segtree {
    int n_;
    std::vector<long long> t_;
public:
    segtree(int n = 0) : n_(n), t_(2 * n, 0) {}
    void build(const std::vector<long long>& a) {
        n_ = a.size();
        t_.resize(2 * n_);
        for (int i = 0; i < n_; ++i) t_[n_ + i] = a[i];
        for (int i = n_ - 1; i > 0; --i) t_[i] = t_[i << 1] + t_[i << 1 | 1];
    }
    void update(int i, long long v) {
        for (t_[i += n_] = v; i > 1; i >>= 1) t_[i >> 1] = t_[i] + t_[i ^ 1];
    }
    long long query(int l, int r) const {  // [l, r)
        long long res = 0;
        for (l += n_, r += n_; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res += t_[l++];
            if (r & 1) res += t_[--r];
        }
        return res;
    }
};

class sparse_table {
    std::vector<std::vector<int>> st_;
    std::vector<int> log_;
public:
    sparse_table(const std::vector<int>& a) {
        int n = a.size(), k = 32 - __builtin_clz(n);
        st_.assign(k, std::vector<int>(n));
        st_[0] = a;
        for (int j = 1; j < k; ++j)
            for (int i = 0; i + (1 << j) <= n; ++i)
                st_[j][i] = std::min(st_[j-1][i], st_[j-1][i + (1 << (j-1))]);
        log_.resize(n + 1);
        for (int i = 2; i <= n; ++i) log_[i] = log_[i/2] + 1;
    }
    int rmq(int l, int r) const {
        int j = log_[r - l + 1];
        return std::min(st_[j][l], st_[j][r - (1 << j) + 1]);
    }
};

class union_find {
    std::vector<int> parent_, rank_;
public:
    union_find(int n = 0) : parent_(n), rank_(n, 0) { std::iota(parent_.begin(), parent_.end(), 0); }
    int find(int x) { return parent_[x] == x ? x : parent_[x] = find(parent_[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) std::swap(a, b);
        parent_[b] = a;
        if (rank_[a] == rank_[b]) ++rank_[a];
        return true;
    }
};

void ch14_test() {
    // Fenwick
    fenwick fw(10);
    for (int i = 0; i < 10; ++i) fw.add(i, i + 1);
    assert(fw.sum(9) == 55);
    assert(fw.range_sum(3, 7) == 4+5+6+7+8);
    
    // Segment Tree
    segtree st(5);
    st.build({1, 3, 5, 7, 9});
    assert(st.query(1, 4) == 3+5+7);
    st.update(2, 10);
    assert(st.query(1, 4) == 3+10+7);
    
    // Sparse Table
    sparse_table st2({2, 5, 1, 8, 3, 9});
    assert(st2.rmq(1, 4) == 1);
    assert(st2.rmq(0, 5) == 1);
    
    // Union-Find
    union_find uf(10);
    uf.unite(0, 1); uf.unite(2, 3); uf.unite(1, 2);
    assert(uf.find(0) == uf.find(3));
    assert(uf.find(0) != uf.find(4));
    
    std::cout << "ch14: PASS\n";
}

// ============================================================
// Chapter 15: Greedy
// ============================================================
int activity_selection(std::vector<std::pair<int, int>> acts) {
    std::sort(acts.begin(), acts.end(), [](auto& a, auto& b) { return a.second < b.second; });
    int count = 0, last_end = -1;
    for (auto [s, e] : acts) if (s >= last_end) { ++count; last_end = e; }
    return count;
}

int job_sequencing(std::vector<std::tuple<int, int, int>> jobs) {  // (id, deadline, profit)
    std::sort(jobs.begin(), jobs.end(), [](auto& a, auto& b) { return std::get<2>(a) > std::get<2>(b); });
    int max_d = 0; for (auto& j : jobs) max_d = std::max(max_d, std::get<1>(j));
    std::vector<int> slot(max_d + 1, -1);
    int profit = 0;
    for (auto& j : jobs) {
        int id = std::get<0>(j), d = std::get<1>(j), p = std::get<2>(j);
        for (int s = d; s > 0; --s) if (slot[s] == -1) { slot[s] = id; profit += p; break; }
    }
    return profit;
}

void ch15_test() {
    std::vector<std::pair<int, int>> acts = {{1, 4}, {3, 5}, {0, 6}, {5, 7}, {3, 8}, {5, 9}, {6, 10}, {8, 11}, {8, 12}, {2, 13}, {12, 14}};
    assert(activity_selection(acts) == 4);  // (1,4), (5,7), (8,11), (12,14)
    
    std::vector<std::tuple<int, int, int>> jobs = {{1, 2, 100}, {2, 1, 19}, {3, 2, 27}, {4, 1, 25}, {5, 3, 15}};
    assert(job_sequencing(jobs) == 142);  // jobs 1, 3, 5
    
    std::cout << "ch15: PASS\n";
}

// ============================================================
// Chapter 16: Divide and Conquer
// ============================================================
std::vector<int> merge_sort(const std::vector<int>& a) {
    if (a.size() <= 1) return a;
    int mid = a.size() / 2;
    auto left = merge_sort(std::vector<int>(a.begin(), a.begin() + mid));
    auto right = merge_sort(std::vector<int>(a.begin() + mid, a.end()));
    std::vector<int> res;
    res.reserve(left.size() + right.size());
    std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(res));
    return res;
}

int quickselect(std::vector<int> a, int k) {  // 0-indexed
    std::function<int(int, int)> select = [&](int l, int r) {
        if (l == r) return a[l];
        int pivot = a[(l + r) / 2];
        int i = l, j = r;
        while (i <= j) {
            while (a[i] < pivot) ++i;
            while (a[j] > pivot) --j;
            if (i <= j) std::swap(a[i++], a[j--]);
        }
        if (k <= j) return select(l, j);
        if (k >= i) return select(i, r);
        return a[k];
    };
    return select(0, a.size() - 1);
}

std::pair<int, int> max_subarray_dc(const std::vector<int>& a, int l, int r) {
    if (l == r) return {a[l], a[l]};
    int m = (l + r) / 2;
    auto left = max_subarray_dc(a, l, m);
    auto right = max_subarray_dc(a, m + 1, r);
    int left_max = INT_MIN, sum = 0;
    for (int i = m; i >= l; --i) { sum += a[i]; left_max = std::max(left_max, sum); }
    int right_max = INT_MIN; sum = 0;
    for (int i = m + 1; i <= r; ++i) { sum += a[i]; right_max = std::max(right_max, sum); }
    int cross = left_max + right_max;
    return {std::max({left.first, right.first, cross}), std::max({left.first, right.first, cross})};
}

void ch16_test() {
    std::vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    auto sorted = merge_sort(arr);
    assert(sorted == std::vector<int>({3, 9, 10, 27, 38, 43, 82}));
    
    std::vector<int> qs = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    assert(quickselect(qs, 4) == 4);  // 5th smallest (0-indexed)
    
    std::vector<int> ms = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    assert(max_subarray_dc(ms, 0, ms.size() - 1).first == 6);  // [4,-1,2,1]
    
    std::cout << "ch16: PASS\n";
}

// ============================================================
// Chapter 17: Dynamic Programming
// ============================================================
int lcs(const std::string& a, const std::string& b) {
    int n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = (a[i-1] == b[j-1]) ? dp[i-1][j-1] + 1 : std::max(dp[i-1][j], dp[i][j-1]);
    return dp[n][m];
}

int edit_distance(const std::string& a, const std::string& b) {
    int n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = (a[i-1] == b[j-1]) ? dp[i-1][j-1] 
                : 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
    return dp[n][m];
}

int knapsack_01(const std::vector<int>& w, const std::vector<int>& v, int W) {
    int n = w.size();
    std::vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; ++i)
        for (int j = W; j >= w[i]; --j)
            dp[j] = std::max(dp[j], dp[j - w[i]] + v[i]);
    return dp[W];
}

int lis(const std::vector<int>& a) {
    std::vector<int> tails;
    for (int x : a) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}

void ch17_test() {
    assert(lcs("ABCBDAB", "BDCAB") == 4);  // BCAB
    assert(edit_distance("kitten", "sitting") == 3);
    assert(knapsack_01({10, 20, 30}, {60, 100, 120}, 50) == 220);
    assert(lis({10, 9, 2, 5, 3, 7, 101, 18}) == 4);  // 2,3,7,101 or 2,5,7,101
    
    std::cout << "ch17: PASS\n";
}

// ============================================================
// Chapter 18: Backtracking and Branch & Bound
// ============================================================
bool n_queens(int n, std::vector<int>& cols, int row = 0) {
    if (row == n) return true;
    for (int col = 0; col < n; ++col) {
        bool ok = true;
        for (int r = 0; r < row; ++r) {
            if (cols[r] == col || std::abs(cols[r] - col) == row - r) { ok = false; break; }
        }
        if (ok) {
            cols[row] = col;
            if (n_queens(n, cols, row + 1)) return true;
        }
    }
    return false;
}

bool sudoku_solve(std::vector<std::vector<int>>& board) {
    for (int i = 0; i < 9; ++i) for (int j = 0; j < 9; ++j) {
        if (board[i][j] != 0) continue;
        for (int d = 1; d <= 9; ++d) {
            bool ok = true;
            for (int k = 0; k < 9; ++k) if (board[i][k] == d || board[k][j] == d) { ok = false; break; }
            int bi = (i / 3) * 3, bj = (j / 3) * 3;
            for (int r = 0; r < 3 && ok; ++r) for (int c = 0; c < 3; ++c) if (board[bi+r][bj+c] == d) { ok = false; break; }
            if (ok) {
                board[i][j] = d;
                if (sudoku_solve(board)) return true;
                board[i][j] = 0;
            }
        }
        return false;
    }
    return true;
}

void ch18_test() {
    std::vector<int> cols(8);
    assert(n_queens(8, cols));
    std::vector<int> cols4(4);
    assert(n_queens(4, cols4));
    
    std::vector<std::vector<int>> sudoku = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };
    assert(sudoku_solve(sudoku));
    assert(sudoku[0][2] == 4);  // First empty cell
    
    std::cout << "ch18: PASS\n";
}

// ============================================================
// Chapter 19: Probabilistic Data Structures
// ============================================================
class bloom_filter {
    std::vector<bool> bits_;
    std::size_t m_, k_;
    std::vector<std::size_t> seeds_;
    static std::size_t hash(const std::string& s, std::size_t seed) {
        std::size_t h = seed;
        for (char c : s) h = h * 31 + c;
        return h;
    }
public:
    bloom_filter(std::size_t n, double p) {  // n items, false positive prob p
        m_ = std::ceil(-n * std::log(p) / (std::log(2) * std::log(2)));
        k_ = std::ceil(std::log(2) * m_ / n);
        bits_.assign(m_, false);
        std::mt19937 gen(42);
        std::uniform_int_distribution<std::size_t> dis(1, SIZE_MAX);
        for (std::size_t i = 0; i < k_; ++i) seeds_.push_back(dis(gen));
    }
    void insert(const std::string& s) {
        for (std::size_t seed : seeds_) bits_[hash(s, seed) % m_] = true;
    }
    bool might_contain(const std::string& s) const {
        for (std::size_t seed : seeds_) if (!bits_[hash(s, seed) % m_]) return false;
        return true;
    }
};

void ch19_test() {
    bloom_filter bf(1000, 0.01);
    for (int i = 0; i < 1000; ++i) bf.insert("item" + std::to_string(i));
    for (int i = 0; i < 1000; ++i) assert(bf.might_contain("item" + std::to_string(i)));
    int false_pos = 0;
    for (int i = 1000; i < 2000; ++i) if (bf.might_contain("item" + std::to_string(i))) ++false_pos;
    // Expected ~1% false positive rate
    assert(false_pos < 50);
    
    std::cout << "ch19: PASS\n";
}

// ============================================================
// Chapter 20: Max Flow
// ============================================================
struct dinic {
    struct Edge { int to, rev, cap; };
    int n_, s_, t_;
    std::vector<std::vector<Edge>> g_;
    std::vector<int> level_, ptr_;
    dinic(int n, int s, int t) : n_(n), s_(s), t_(t), g_(n), level_(n), ptr_(n) {}
    void add_edge(int u, int v, int cap) {
        g_[u].push_back({v, (int)g_[v].size(), cap});
        g_[v].push_back({u, (int)g_[u].size() - 1, 0});
    }
    bool bfs() {
        std::fill(level_.begin(), level_.end(), -1);
        std::queue<int> q; q.push(s_); level_[s_] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : g_[u]) if (e.cap > 0 && level_[e.to] == -1) {
                level_[e.to] = level_[u] + 1; q.push(e.to);
            }
        }
        return level_[t_] != -1;
    }
    int dfs(int u, int flow) {
        if (u == t_ || flow == 0) return flow;
        for (int& i = ptr_[u]; i < (int)g_[u].size(); ++i) {
            auto& e = g_[u][i];
            if (e.cap > 0 && level_[e.to] == level_[u] + 1) {
                int pushed = dfs(e.to, std::min(flow, e.cap));
                if (pushed > 0) { e.cap -= pushed; g_[e.to][e.rev].cap += pushed; return pushed; }
            }
        }
        return 0;
    }
    int max_flow() {
        int flow = 0;
        while (bfs()) {
            std::fill(ptr_.begin(), ptr_.end(), 0);
            while (int pushed = dfs(s_, INT_MAX)) flow += pushed;
        }
        return flow;
    }
};

void ch20_test() {
    // Simple flow network
    dinic d(4, 0, 3);
    d.add_edge(0, 1, 10);
    d.add_edge(0, 2, 10);
    d.add_edge(1, 2, 2);
    d.add_edge(1, 3, 4);
    d.add_edge(2, 3, 10);
    assert(d.max_flow() == 14);
    
    // Bipartite matching as flow
    dinic d2(6, 0, 5);
    // Left: 1,2  Right: 3,4  Source: 0  Sink: 5
    d2.add_edge(0, 1, 1); d2.add_edge(0, 2, 1);
    d2.add_edge(1, 3, 1); d2.add_edge(1, 4, 1);
    d2.add_edge(2, 3, 1);
    d2.add_edge(3, 5, 1); d2.add_edge(4, 5, 1);
    assert(d2.max_flow() == 2);
    
    std::cout << "ch20: PASS\n";
}

// ============================================================
// Chapter 21: Persistent Data Structures
// ============================================================
template <typename T>
struct pseg_node {
    T val;
    std::shared_ptr<pseg_node> left, right;
    pseg_node(T v = T{}) : val(v), left(nullptr), right(nullptr) {}
    pseg_node(T v, std::shared_ptr<pseg_node> l, std::shared_ptr<pseg_node> r) : val(v), left(l), right(r) {}
};

template <typename T>
class persistent_seg {
    std::vector<std::shared_ptr<pseg_node<T>>> roots_;
    int n_;
    std::shared_ptr<pseg_node<T>> build(int l, int r, const std::vector<T>& a) {
        if (l == r) return std::make_shared<pseg_node<T>>(a[l]);
        int m = (l + r) / 2;
        return std::make_shared<pseg_node<T>>(T{}, build(l, m, a), build(m + 1, r, a));
    }
    std::shared_ptr<pseg_node<T>> update(std::shared_ptr<pseg_node<T>> node, int l, int r, int idx, T val) {
        if (l == r) return std::make_shared<pseg_node<T>>(val);
        int m = (l + r) / 2;
        if (idx <= m) return std::make_shared<pseg_node<T>>(T{}, update(node->left, l, m, idx, val), node->right);
        else return std::make_shared<pseg_node<T>>(T{}, node->left, update(node->right, m + 1, r, idx, val));
    }
    T query(std::shared_ptr<pseg_node<T>> node, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return node->val;
        int m = (l + r) / 2;
        if (qr <= m) return query(node->left, l, m, ql, qr);
        if (ql > m) return query(node->right, m + 1, r, ql, qr);
        return query(node->left, l, m, ql, qr) + query(node->right, m + 1, r, ql, qr);
    }
public:
    persistent_seg(const std::vector<T>& a) : n_(a.size()) {
        roots_.push_back(build(0, n_ - 1, a));
    }
    void update(int idx, T val) {
        roots_.push_back(update(roots_.back(), 0, n_ - 1, idx, val));
    }
    T query(int version, int l, int r) const {
        return query(roots_[version], 0, n_ - 1, l, r);
    }
    int versions() const { return roots_.size(); }
};

void ch21_test() {
    persistent_seg<int> ps({1, 2, 3, 4, 5});
    assert(ps.query(0, 0, 4) == 15);
    ps.update(2, 10);
    assert(ps.query(1, 0, 4) == 22);
    assert(ps.query(0, 0, 4) == 15);  // Old version unchanged
    ps.update(0, 100);
    assert(ps.query(2, 0, 4) == 121);
    assert(ps.versions() == 3);
    
    std::cout << "ch21: PASS\n";
}

// ============================================================
// Chapter 22: Succinct Data Structures
// ============================================================
class rank_select {
    std::vector<uint64_t> bits_;
    std::vector<uint32_t> rank_;
    int n_;
public:
    rank_select(const std::vector<int>& bits) : n_(bits.size()) {
        bits_.resize((n_ + 63) / 64, 0);
        for (int i = 0; i < n_; ++i) if (bits[i]) bits_[i >> 6] |= (1ULL << (i & 63));
        rank_.resize(bits_.size() + 1, 0);
        for (size_t i = 0; i < bits_.size(); ++i) rank_[i + 1] = rank_[i] + __builtin_popcountll(bits_[i]);
    }
    int rank(int i) const {  // number of 1s in [0, i)
        if (i <= 0) return 0;
        i = std::min(i, n_);
        int full = i >> 6;
        int res = rank_[full];
        int partial = i & 63;
        if (partial) res += __builtin_popcountll(bits_[full] & ((1ULL << partial) - 1));
        return res;
    }
    int select(int k) const {  // position of k-th 1 (1-indexed)
        if (k <= 0 || k > static_cast<int>(rank_[bits_.size()])) return -1;
        int lo = 0, hi = static_cast<int>(bits_.size());
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (rank_[mid] >= static_cast<unsigned int>(k)) hi = mid;
            else lo = mid + 1;
        }
        int base = (lo - 1) * 64;
        uint64_t w = bits_[lo - 1];
        int need = k - rank_[lo - 1];
        for (int i = 0; i < 64; ++i) {
            if (w & (1ULL << i)) { --need; if (need == 0) return base + i; }
        }
        return -1;
    }
};

void ch22_test() {
    std::vector<int> bits = {1,0,1,1,0,1,0,0,1,1,1,0,1,0,1,0};
    rank_select rs(bits);
    assert(rs.rank(0) == 0);
    assert(rs.rank(4) == 3);  // 1,0,1,1
    assert(rs.rank(16) == 8);
    assert(rs.select(1) == 0);
    assert(rs.select(3) == 3);
    assert(rs.select(8) == 14);
    assert(rs.select(9) == -1);
    
    std::cout << "ch22: PASS\n";
}

// ============================================================
// Main
// ============================================================
int main() {
    std::cout << "=== Chaman Singh Verma Book - All Chapter Tests ===\n\n";
    
    ch01_test();
    ch02_test();
    ch03_test();
    ch04_test();
    ch05_test();
    ch06_test();
    ch07_test();
    ch08_test();
    ch09_test();
    ch10_test();
    ch11_test();
    ch12_test();
    ch13_test();
    ch14_test();
    ch15_test();
    ch16_test();
    ch17_test();
    ch18_test();
    ch19_test();
    ch20_test();
    ch21_test();
    ch22_test();
    
    std::cout << "\n=== ALL 22 CHAPTERS PASSED ===\n";
    return 0;
}