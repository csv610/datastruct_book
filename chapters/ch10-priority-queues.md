# Priority Queues

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The Priority Queue ADT

A **priority queue** is a collection where each element has a **priority**, and the element with the highest (or lowest) priority is always removed first.

### Operations

- **push(value)** — insert with given priority
- **pop()** — remove the highest-priority element
- **top()** — return the highest-priority element without removing
- **empty()**, **size()**

### Applications

- **Scheduling** — CPU task scheduling, print queues by priority
- **Event-driven simulation** — process events in chronological order
- **Dijkstra's shortest path** — extract the vertex with smallest distance
- **Huffman coding** — repeatedly combine the two smallest frequencies
- **Median finding** — maintain two heaps
- **A* search** — open set ordered by f-cost

## Heaps

A **max-heap** is a complete binary tree where every node's value is ≥ the values of its children. A **min-heap** is the symmetric case (≤).

```
Max-Heap:
          [90]
        /     \
      [80]    [70]
     /   \    /
   [30] [20] [50]

Array representation: [90, 80, 70, 30, 20, 50]
```

### Array Storage

Since a heap is a **complete binary tree**, we store it in an array:

```
Position i (0-indexed):
  left child:  2i + 1
  right child: 2i + 2
  parent:      (i - 1) / 2
```

### Max-Heap Implementation

```cpp
template <typename T, typename Compare = std::less<T>>
class max_heap {
public:
    max_heap() = default;

    void push(const T& value) {
        data_.push_back(value);
        sift_up(data_.size() - 1);
    }

    void push(T&& value) {
        data_.push_back(std::move(value));
        sift_up(data_.size() - 1);
    }

    void pop() {
        if (empty()) throw std::underflow_error("heap is empty");
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) sift_down(0);
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return data_[0];
    }

    bool empty() const noexcept { return data_.empty(); }
    size_t size() const noexcept { return data_.size(); }

private:
    void sift_up(size_t i) {
        while (i > 0) {
            size_t parent = (i - 1) / 2;
            if (!compare_(data_[parent], data_[i])) break;
            std::swap(data_[i], data_[parent]);
            i = parent;
        }
    }

    void sift_down(size_t i) {
        size_t n = data_.size();
        while (true) {
            size_t largest = i;
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;

            if (left < n && compare_(data_[largest], data_[left]))
                largest = left;
            if (right < n && compare_(data_[largest], data_[right]))
                largest = right;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }

    std::vector<T> data_;
    Compare compare_;  // less<T> → max-heap
};
```

### Push Walkthrough

Insert 85 into heap [90, 80, 70, 30, 20, 50]:

```
Step 1: Append 85 → [90, 80, 70, 30, 20, 50, 85]
Step 2: Compare 85 with parent 70 → 85 > 70 → swap
        [90, 80, 85, 30, 20, 50, 70]
Step 3: Compare 85 with parent 90 → 85 ≤ 90 → stop
```

### Pop Walkthrough

Remove top (90) from [90, 80, 70, 30, 20, 50]:

```
Step 1: Move last to root → [50, 80, 70, 30, 20]
Step 2: Sift down 50:
  Compare with children 80 and 70 → 80 is largest → swap
  [80, 50, 70, 30, 20]
Step 3: Compare 50 with children 30 and 20 → 50 is largest → stop
```

## Heap Initialization (Heapify)

Building a heap from an unsorted array: O(n).

```cpp
template <typename T, typename Compare = std::less<T>>
void heapify(std::vector<T>& data, Compare compare = {}) {
    int n = data.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        sift_down(data, i, n, compare);
    }
}
```

Why O(n)? Most nodes are near the bottom and sift down takes few steps. The total work is Σ (height of node) over all nodes = O(n).

## Heap Sort

```cpp
template <typename T>
void heap_sort(std::span<T> data) {
    // Build max-heap
    int n = data.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        // sift_down
        int current = i;
        while (true) {
            int largest = current;
            int left = 2 * current + 1;
            int right = 2 * current + 2;
            if (left < n && data[left] > data[largest]) largest = left;
            if (right < n && data[right] > data[largest]) largest = right;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }

    // Extract elements in sorted order
    for (int i = n - 1; i > 0; --i) {
        std::swap(data[0], data[i]);
        // sift_down on reduced heap
        int current = 0;
        int heap_size = i;
        while (true) {
            int largest = current;
            int left = 2 * current + 1;
            int right = 2 * current + 2;
            if (left < heap_size && data[left] > data[largest]) largest = left;
            if (right < heap_size && data[right] > data[largest]) largest = right;
            if (largest == current) break;
            std::swap(data[current], data[largest]);
            current = largest;
        }
    }
}
```

**Complexity:** O(n log n) worst case. **In-place** — O(1) extra space.

## Min-Heaps

A min-heap is the same structure with the order reversed. Use `std::greater<T>` as the comparator to make our `max_heap` behave as a min-heap.

C++ `std::priority_queue` provides `std::less<T>` for max-heap and `std::greater<T>` for min-heap:

```cpp
std::priority_queue<int> max_pq;                    // max-heap
std::priority_queue<int, std::vector<int>,
                    std::greater<int>> min_pq;      // min-heap
```

## Leftist Trees (Mergeable Heaps)

A **leftist tree** is a binary tree that supports O(log n) merge. This is useful when we need to merge two priority queues frequently.

**Property:** The **null path length** (npl) of a node is the length of the shortest path to a null child. A leftist tree maintains the invariant: npl(left) ≥ npl(right) for every node.

```cpp
template <typename T>
struct leftist_node {
    T data;
    std::unique_ptr<leftist_node> left;
    std::unique_ptr<leftist_node> right;
    int npl = 0;  // null path length

    explicit leftist_node(const T& value) : data(value) {}
};

template <typename T>
std::unique_ptr<leftist_node<T>> merge(
    std::unique_ptr<leftist_node<T>> a,
    std::unique_ptr<leftist_node<T>> b) {
    if (!a) return b;
    if (!b) return a;

    // Ensure a has the smaller root
    if (a->data > b->data) std::swap(a, b);

    // Recursively merge with right child
    a->right = merge(std::move(a->right), std::move(b));

    // Maintain leftist property
    if (!a->left || (a->left->npl < a->right->npl)) {
        std::swap(a->left, a->right);
    }

    // Update null path length
    a->npl = (a->right ? a->right->npl : 0) + 1;

    return a;
}
```

## Application: Event-Driven Simulation

Simulate a bank with multiple tellers:

```cpp
struct event {
    double time;
    enum { ARRIVAL, DEPARTURE } type;
    int customer_id;
    int teller_id;

    bool operator>(const event& other) const {
        return time > other.time;
    }
};

void bank_simulation(int num_tellers, double arrival_rate, double duration) {
    // Min-heap of events
    std::priority_queue<event, std::vector<event>, std::greater<event>> events;
    std::vector<int> teller_available_time(num_tellers, 0);

    // First arrival
    events.push({0.0, event::ARRIVAL, 0, -1});

    int customer_count = 0;
    double total_wait = 0.0;

    while (!events.empty()) {
        event e = events.top(); events.pop();
        if (e.time > duration) break;

        if (e.type == event::ARRIVAL) {
            ++customer_count;
            // Schedule next arrival
            double interarrival = -std::log(1.0 - std::rand() / RAND_MAX) / arrival_rate;
            events.push({e.time + interarrival, event::ARRIVAL, customer_count, -1});

            // Find free teller
            int best_teller = 0;
            for (int t = 1; t < num_tellers; ++t) {
                if (teller_available_time[t] < teller_available_time[best_teller])
                    best_teller = t;
            }

            double start = std::max(e.time, double(teller_available_time[best_teller]));
            double wait = start - e.time;
            total_wait += wait;

            double service_time = 1.0 + std::rand() / RAND_MAX * 4.0;  // 1-5 min
            teller_available_time[best_teller] = start + service_time;
            events.push({start + service_time, event::DEPARTURE, customer_count, best_teller});
        }
    }

    std::cout << "Customers served: " << customer_count << '\n'
              << "Average wait: " << total_wait / customer_count << '\n';
}
```

## Application: Median Maintenance

Maintain the median of a stream of numbers:

```cpp
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
```

## STL Connection: std::priority_queue

```cpp
std::priority_queue<int> pq;  // max-heap
pq.push(30);
pq.push(10);
pq.push(50);
std::cout << pq.top();  // 50
pq.pop();               // removes 50
std::cout << pq.top();  // 30
```

`std::priority_queue` uses `std::vector` by default and provides the standard heap operations.

For direct heap manipulation on arbitrary vectors:

```cpp
std::vector<int> v = {30, 10, 50, 20, 40};
std::make_heap(v.begin(), v.end());         // O(n)
std::pop_heap(v.begin(), v.end());          // move max to end, O(log n)
v.pop_back();
std::push_heap(v.begin(), v.end());         // after push_back, O(log n)
std::sort_heap(v.begin(), v.end());         // O(n log n)
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Confusing min-heap vs max-heap comparator | Wrong element at top, incorrect algorithm | `std::priority_queue<T>` uses `std::less` → max-heap |
| Forgetting `sift_down` after replacing the root | Heap property violated | Always restore heap after modification |
| Modifying priority of an element already in the heap | Heap property violated, wrong top | Remove and re-insert, or use a mutable heap |
| Off-by-one in child index formulas (2i vs 2i+1) | Access wrong node | Left child = 2i+1, right = 2i+2 (0-indexed) |
| Using `std::make_heap` but never `std::push_heap`/`std::pop_heap` | Not maintaining heap property after push/pop | Use the full `std::push_heap`/`std::pop_heap` pair |
| Passing the wrong comparator to `std::priority_queue` | Opposite ordering than expected | Test with `top()` after a single push to verify order |

## Summary

1. **Heaps** implement priority queues with O(log n) push/pop and O(n) heapify.
2. **Array storage** exploits the complete binary tree structure with simple index arithmetic.
3. **Heap sort** is an O(n log n) in-place sorting algorithm.
4. **Leftist trees** support O(log n) merge of two priority queues.
5. `std::priority_queue` is the production priority queue.

## Exercises

### Drill

1. Show the array representation of a max-heap after each insertion of: [10, 20, 5, 30, 15, 25]. Use the array indices 1..n (1-indexed) for clarity. Draw the tree at each step.

2. Show the array after each step of heapify on [5, 3, 17, 10, 84, 19, 6, 22, 9] (a max-heap). Start from the last non-leaf node.

3. How many comparisons are needed to build a heap of size n using heapify? Prove your answer.

4. Trace heap sort on [3, 1, 4, 1, 5, 9, 2, 6]. Show the array after each `extract_max` and after each `sift_down`.

5. For a min-heap, what is the complexity of:
   a) Finding the minimum element
   b) Inserting a new element
   c) Deleting the minimum
   d) Merging two heaps of size n and m (without using any special mergeable structure)

### Application

6. Implement a min-heap using `std::greater` as the comparator. Verify correctness against `std::priority_queue<int, std::vector<int>, std::greater<int>>`.

7. Implement heap sort in-place. Benchmark against `std::sort` for n = 10³, 10⁴, 10⁵, 10⁶ on random, sorted, and reverse-sorted data. Explain why `std::sort` is faster despite both being O(n log n).

8. Implement the **k-way merge**: merge k sorted arrays into one sorted array using a min-heap. Compare its performance with repeated 2-way merges for k = 4, 8, 16 at total n = 10⁵.

9. Implement a priority queue with **decrease-key** — needed for Dijkstra's algorithm. Store each element's position in the heap using a hash map. Verify correctness with Dijkstra on a small graph.

10. Implement `is_heap` that checks if a vector satisfies the heap property. Use it to validate your heap implementations on random vectors.

11. Use two heaps (a max-heap for the lower half, a min-heap for the upper half) to implement a `median_tracker` that finds the running median of a stream. Process 10⁶ numbers and report the median after each 100,000 elements.

12. Implement the **sliding window median**: given an array and window size k, output the median of each window. Use the two-heap approach from exercise 11.

13. Implement **Prim's MST algorithm** using a priority queue with decrease-key. Compare its runtime with the version that does not use decrease-key (inserts duplicates instead).

### Research

14. **(Binomial heaps)** Implement a binomial heap — a forest of binomial trees that supports O(log n) merge. Compare merge performance with the std::priority_queue vs your binomial heap. When does the merge capability matter?

15. **(Fibonacci heaps)** Read about Fibonacci heaps (Fredman and Tarjan, 1987). Implement decrease-key with O(1) amortized time. Compare its performance with the standard binary heap for Dijkstra's algorithm on large sparse graphs.

16. **(Priority queue STL choice)** The C++ standard requires `std::priority_queue` to use `std::vector` by default. Why not `std::deque`? What are the performance implications? Read the committee discussions.

17. **(d-ary heaps)** Implement a 4-ary heap (each node has up to 4 children). Compare its performance with the binary heap. How does the choice of d affect the number of comparisons vs the number of swaps?

## References and Further Reading

- Donald E. Knuth, *The Art of Computer Programming*, Volume 3: Sorting and Searching. Section 5.2.3.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapter 6.
- Robert W. Floyd, "Algorithm 245: Treesort" — CACM, 1964.
- J. W. J. Williams, "Algorithm 232: Heapsort" — CACM, 1964.
- Jean Vuillemin, "A Data Structure for Manipulating Priority Queues" — CACM, 1978.
- Michael L. Fredman and Robert E. Tarjan, "Fibonacci Heaps and Their Uses in Improved Network Optimization Algorithms" — JACM, 1987.
- Michael L. Fredman, Robert Sedgewick, Daniel D. Sleator, and Robert E. Tarjan, "The Pairing Heap: A New Form of Self-Adjusting Heap" — Algorithmica, 1986.
