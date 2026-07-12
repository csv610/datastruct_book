# Segment Trees, Fenwick Trees, and Union-Find

Range queries — asking for the sum, minimum, maximum, or other aggregate over a contiguous subarray — are among the most common non-trivial problems in algorithm design. This chapter presents three data structures that answer such queries efficiently.

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Segment Tree

A **segment tree** is a binary tree that stores information about intervals of an array. It supports:
- **Range query** (e.g., sum, min, max over [l, r)): O(log n)
- **Point update** (change a single element): O(log n)
- **Range update** (with lazy propagation): O(log n)

### Structure

Given an array of size n, the segment tree has 2n nodes (or 4n for simplicity). The leaves store the original array elements. Each internal node stores the aggregate of its two children (e.g., sum, min, max).

```
Array: [1, 3, 5, 7, 9, 11]

Segment tree (sum):
            [36]
         /        \
       [9]         [27]
      /   \       /    \
    [4]    [5]  [16]   [11]
   /  \    / \   / \    / \
  [1] [3] [5][7][9][11]

Leaves (original array): 1  3  5  7  9  11
```

### Implementation

```cpp
template <typename T, typename Combine = std::plus<T>>
class segment_tree {
public:
    segment_tree(std::span<const T> data, Combine combine = {})
        : n_(data.size()), tree_(4 * n_), combine_(combine) {
        build(data, 1, 0, n_ - 1);
    }

    // Query over range [l, r] inclusive
    T query(size_t l, size_t r) {
        return query(1, 0, n_ - 1, l, r);
    }

    // Point update: set position pos to value
    void update(size_t pos, const T& value) {
        update(1, 0, n_ - 1, pos, value);
    }

private:
    void build(std::span<const T> data, size_t node, size_t l, size_t r) {
        if (l == r) {
            tree_[node] = data[l];
            return;
        }
        size_t mid = l + (r - l) / 2;
        build(data, 2 * node, l, mid);
        build(data, 2 * node + 1, mid + 1, r);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    T query(size_t node, size_t l, size_t r, size_t ql, size_t qr) {
        if (ql > r || qr < l) return T{};  // no overlap (identity for combine)
        if (ql <= l && r <= qr) return tree_[node];  // total overlap
        size_t mid = l + (r - l) / 2;
        auto left = query(2 * node, l, mid, ql, qr);
        auto right = query(2 * node + 1, mid + 1, r, ql, qr);
        return combine_(left, right);
    }

    void update(size_t node, size_t l, size_t r, size_t pos, const T& value) {
        if (l == r) {
            tree_[node] = value;
            return;
        }
        size_t mid = l + (r - l) / 2;
        if (pos <= mid) update(2 * node, l, mid, pos, value);
        else update(2 * node + 1, mid + 1, r, pos, value);
        tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
    }

    size_t n_;
    std::vector<T> tree_;
    Combine combine_;
};
```

### Query Walkthrough

Query sum over [1, 5] for array [1, 3, 5, 7, 9, 11] (0-indexed):

```
root covers [0, 5], query = [1, 5]:
  left child [0, 2] overlaps → recurse
    [0, 0]: no overlap
    [1, 2]: total overlap → return 8
  right child [3, 5] overlaps → recurse
    [3, 5]: total overlap → return 27
  combine(8, 27) = 35

Check: 3 + 5 + 7 + 9 + 11 = 35 ✓
```

### Lazy Propagation

For **range updates** (e.g., add a value to every element in [l, r]), we use lazy propagation — defer the update to children until necessary.

```cpp
void range_update(size_t node, size_t l, size_t r, size_t ql, size_t qr, const T& delta) {
    push(node, l, r);  // propagate any pending updates
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) {
        lazy_[node] += delta;  // mark for propagation
        tree_[node] += delta * (r - l + 1);  // apply to current node
        push(node, l, r);  // propagate immediately
        return;
    }
    size_t mid = l + (r - l) / 2;
    range_update(2 * node, l, mid, ql, qr, delta);
    range_update(2 * node + 1, mid + 1, r, ql, qr, delta);
    tree_[node] = combine_(tree_[2 * node], tree_[2 * node + 1]);
}
```

### Application: Range Minimum Query

```cpp
auto min_combine = [](int a, int b) { return std::min(a, b); };
segment_tree<int, decltype(min_combine)> st(data, min_combine);

// Query minimum in range [2, 5]
int min_val = st.query(2, 5);
```

## Fenwick Tree (Binary Indexed Tree)

A **Fenwick tree** (BIT) is a simpler alternative to segment trees for prefix queries and point updates. It uses less memory (n vs 4n) and is faster in practice.

**Operations:**
- **Prefix query** (sum over [0, i)): O(log n)
- **Point update** (add delta at index i): O(log n)
- **Range query** (sum over [l, r)): O(log n) via prefix(r) - prefix(l)

### The Idea

The Fenwick tree stores partial sums in an array where index i stores the sum of a range of length (i & -i) — the lowest set bit of i.

```
Array (1-indexed): [1, 3, 5, 7, 9, 11]

Fenwick tree (BIT):
BIT[1] = arr[1] = 1
BIT[2] = arr[1] + arr[2] = 4
BIT[3] = arr[3] = 5
BIT[4] = arr[1] + arr[2] + arr[3] + arr[4] = 16
BIT[5] = arr[5] = 9
BIT[6] = arr[5] + arr[6] = 20

To get prefix sum up to index 5:
  BIT[5] = 9
  i -= (i & -i) → i = 4
  BIT[4] = 16
  i -= (i & -i) → i = 0
  Total = 9 + 16 = 25
  Check: 1+3+5+7+9 = 25 ✓
```

### Implementation

```cpp
template <typename T>
class fenwick_tree {
public:
    explicit fenwick_tree(size_t n) : bit_(n + 1, T{}) {}

    void add(size_t idx, T delta) {
        ++idx;  // 1-indexed internally
        while (idx < bit_.size()) {
            bit_[idx] += delta;
            idx += idx & -idx;
        }
    }

    T prefix_sum(size_t idx) const {
        // Sum over [0, idx)
        T result{};
        while (idx > 0) {
            result += bit_[idx];
            idx -= idx & -idx;
        }
        return result;
    }

    T range_sum(size_t l, size_t r) const {
        // Sum over [l, r)
        return prefix_sum(r) - prefix_sum(l);
    }

    // Find smallest index such that prefix_sum >= target
    size_t lower_bound(T target) const {
        size_t idx = 0;
        size_t bit_mask = 1;
        while (bit_mask < bit_.size()) bit_mask <<= 1;
        bit_mask >>= 1;

        while (bit_mask > 0) {
            size_t next = idx + bit_mask;
            if (next < bit_.size() && bit_[next] < target) {
                target -= bit_[next];
                idx = next;
            }
            bit_mask >>= 1;
        }
        return idx;  // 0-indexed
    }

private:
    std::vector<T> bit_;
};
```

### Application: Inversion Count

Count pairs (i, j) with i < j and arr[i] > arr[j]:

```cpp
size_t inversion_count(std::span<const int> arr) {
    // Coordinate compress
    auto sorted = std::vector<int>(arr.begin(), arr.end());
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    fenwick_tree<int> bit(sorted.size());
    size_t inversions = 0;

    for (int x : arr) {
        size_t pos = std::lower_bound(sorted.begin(), sorted.end(), x) - sorted.begin();
        // Count elements greater than x that we've already seen
        inversions += bit.range_sum(pos + 1, sorted.size());
        bit.add(pos, 1);
    }
    return inversions;
}
```

## Union-Find (Disjoint Set Union)

We introduced Union-Find in Chapter 9. Here we give a fuller treatment.

### Optimal Implementation

```cpp
class union_find {
public:
    union_find(size_t n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t x) {
        // Path compression
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];  // halving
            x = parent_[x];
        }
        return x;
    }

    void union_sets(size_t x, size_t y) {
        size_t rx = find(x);
        size_t ry = find(y);
        if (rx == ry) return;

        // Union by size
        if (size_[rx] < size_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        size_[rx] += size_[ry];
    }

    bool same_set(size_t x, size_t y) {
        return find(x) == find(y);
    }

    size_t set_size(size_t x) {
        return size_[find(x)];
    }

    size_t count_sets() const {
        size_t count = 0;
        for (size_t i = 0; i < parent_.size(); ++i) {
            if (parent_[i] == i) ++count;
        }
        return count;
    }

private:
    std::vector<size_t> parent_;
    std::vector<size_t> size_;
};
```

### Complexity

With both path compression and union by size/rank: m operations on n elements take O(m·α(n)) time, where α(n) is the inverse Ackermann function. For any practical n, α(n) ≤ 5.

### Application: Kruskal's MST

(See Chapter 12 for the full algorithm. Union-Find is the cycle-detection mechanism.)

### Application: Percolation

Model whether a fluid can flow through a porous material:

```cpp
class percolation {
public:
    percolation(size_t n) : n_(n), uf_(n * n + 2), open_(n * n, false) {
        top_ = n * n;       // virtual top node
        bottom_ = n * n + 1; // virtual bottom node
    }

    void open(size_t row, size_t col) {
        size_t idx = row * n_ + col;
        if (open_[idx]) return;
        open_[idx] = true;

        if (row == 0) uf_.union_sets(idx, top_);
        if (row == n_ - 1) uf_.union_sets(idx, bottom_);

        // Connect to open neighbors
        if (row > 0 && open_[idx - n_]) uf_.union_sets(idx, idx - n_);
        if (row < n_ - 1 && open_[idx + n_]) uf_.union_sets(idx, idx + n_);
        if (col > 0 && open_[idx - 1]) uf_.union_sets(idx, idx - 1);
        if (col < n_ - 1 && open_[idx + 1]) uf_.union_sets(idx, idx + 1);
    }

    bool is_open(size_t row, size_t col) const {
        return open_[row * n_ + col];
    }

    bool percolates() const {
        return uf_.same_set(top_, bottom_);
    }

private:
    size_t n_;
    union_find uf_;
    std::vector<bool> open_;
    size_t top_, bottom_;
};
```

## Comparison

| Structure | Build | Query | Update | Range Update | Memory |
|-----------|-------|-------|--------|--------------|--------|
| Segment Tree | O(n) | O(log n) | O(log n) | O(log n) lazy | O(4n) |
| Fenwick Tree | O(n log n)* | O(log n) | O(log n) | No | O(n) |
| Union-Find | O(n) | O(α(n)) | O(α(n)) | N/A | O(n) |

*Fenwick can be built in O(n) by computing prefix sums first.

**When to use which:**
- **Fenwick tree:** prefix sums, point updates, no range updates — simplest and fastest
- **Segment tree:** range updates, non-invertible operations (min, max, gcd), complex queries
- **Union-Find:** disjoint set connectivity, graph connectivity, incremental connectivity

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Segment tree array size = 2n (not 4n) | Buffer overflow on non-power-of-2 arrays or range queries | Allocate 4n for safety, or check exact size formula |
| Forgetting lazy propagation in range updates | Child nodes not updated, queries return stale data | Always push lazy values before descending |
| Fenwick tree 1-indexing confusion | Off-by-one on queries, wrong prefix sums | Remember: Fenwick is 1-indexed; query index i includes element i |
| Using Fenwick tree for range minimum | Fenwick only supports invertible operations (sum, xor) | Use segment tree for min/max/gcd |
| Union-Find without path compression | O(n) find, tree becomes linear on tall chains | Always compress path during find |
| Union without union by rank | Tree depth grows O(n) in worst case | Merge smaller tree under larger |
| Reversing union order on disjoint sets | Large tree attached to small, height increases | Check rank/size before merging |

## Summary

1. **Segment trees** provide O(log n) range queries and updates with O(n) memory.
2. **Fenwick trees** are simpler and faster for prefix sums with point updates.
3. **Union-Find** tracks disjoint sets with amortized near-constant operations.
4. **Segment tree with lazy propagation** supports efficient range updates.
5. **Union-Find with path compression and union by rank** achieves O(α(n)) amortized complexity.

## Exercises

### Drill

1. **Segment tree sizing.** You build a segment tree for array size n = 10^6 (not a power of 2). A range query accesses indices beyond 2n — why? What's the correct allocation size? Draw the tree structure for n = 6.

2. **Fenwick 1-indexing.** Given values at positions 1–8: [1, 3, 5, 7, 9, 11, 13, 15], draw the Fenwick tree array. Trace query(5) (prefix sum up to index 5). Why does Fenwick use 1-based indexing? What goes wrong with 0-based?

3. **Union-find without compression.** Perform union(0,1), union(2,3), union(0,2), find(3) on a union-find structure without path compression. Show the parent array. What is the height of the tree containing 3? Now add path compression to find(3) — what changes?

4. **Segment tree vs sparse table.** For range minimum queries on data that never changes, which is better: segment tree (O(log n) query) or sparse table (O(1) query)? Compare build time and memory for n = 10^5. When would the segment tree still be preferred?

### Application

5. **Union-find for graph connectivity.** Implement union-find with path compression and union by rank. Test on a graph with 10^6 nodes and 5 × 10^6 random edges. Count the number of find operations and measure runtime. Verify that the total operations are near-linear.

   *(In production, Spark's GraphX library uses union-find for connected components in large distributed graphs.)*

6. **Range sum with Fenwick vs segment tree.** Implement both a Fenwick tree and a segment tree for range sum with point updates. Benchmark:
   - Query time for ranges of size 100, 1000, 10000
   - Update time for a single point
   - Memory usage for n = 10^6
   Which would you use for a monitoring dashboard (reads >> writes)? For a high-frequency trading system (writes >> reads)?

7. **Quantile estimation.** Implement a frequency Fenwick tree over a range of values. Support increment(value, delta), prefix_sum(k), and quantile(p). Feed 10^6 latency measurements (1–10000 ms) and report P50, P90, P99. Compare with exact quantiles from sorting. How much faster is the Fenwick approach?

   *(In production, Google's Sawzall log analysis language used Fenwick trees for approximate quantile computation.)*

8. **Range minimum with lazy updates.** Implement a segment tree that supports range minimum query and range add (lazy propagation). Test on 10^6 simulated updates and queries. Compare with a naive O(n) approach for each operation.

   *(In production, order-matching engines use segment-tree-like structures to maintain best bid/ask across price levels.)*

9. **Percolation threshold.** Model a material as an n × n grid where each cell is open (conductive) or blocked. Current flows from top to bottom through connected open cells. Use union-find to determine the critical fraction where current first flows through. Run 1000 simulations for n = 10, 50, 100. How does the threshold change with n?

10. **Range assign with segment tree.** Implement a segment tree that supports range assign (set all values in [l, r] to x) and point query. Benchmark for 10^6 operations. Compare with a brute-force approach.

### Research

11. **Persistent segment tree.** Implement a persistent segment tree where each update creates a new version while keeping old ones. Answer "k-th smallest element in subarray arr[l..r]" using binary search on versions. Benchmark for n = 10^5 with 10^4 queries. How does persistence affect memory and speed?

12. **Segment tree with fractional cascading.** Implement a segment tree where each node stores a sorted list of its subtree's values (merge sort tree). Then apply fractional cascading to reduce query time from O(log² n) to O(log n). Benchmark before and after.

13. **Union-find with rollback.** Implement union-find that supports undo — store each change on a stack. Solve the offline dynamic connectivity problem: given a graph with edge deletions over time, answer connectivity queries at each time step. Benchmark for 10^5 operations.

14. **Sparse table.** Implement a sparse table for O(1) range minimum queries on static data (no updates). Compare build time, query time, and memory with a segment tree for n = 10^5. For which applications would you choose each? *(In production, monitoring dashboards use sparse tables for historical metrics that never change.)*

## References and Further Reading

- Peter M. Fenwick, "A New Data Structure for Cumulative Frequency Tables" — Software: Practice and Experience, 1994.
- Robert E. Tarjan and Jan van Leeuwen, "Worst-Case Analysis of Set Union Algorithms" — JACM, 1984.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapters 19 (Fibonacci heaps, related), 21 (union-find).
- Bernard Chazelle, "A Minimum Spanning Tree Algorithm with Inverse-Ackermann Type Complexity" — JACM, 2000.
- Robert E. Tarjan, "Efficiency of a Good But Not Linear Set Union Algorithm" — JACM, 1975.
- Harold N. Gabow, Jon Louis Bentley, and Robert E. Tarjan, "Scaling and Related Techniques for Geometry Problems" — STOC 1984 (on segment trees).
