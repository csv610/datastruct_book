# C++ STL Quick Reference

## A.1 Containers

| Container | Header | Description | Iterator Category |
|-----------|--------|-------------|-------------------|
| `std::vector<T>` | `<vector>` | Dynamic array | Random access |
| `std::deque<T>` | `<deque>` | Double-ended queue | Random access |
| `std::list<T>` | `<list>` | Doubly linked list | Bidirectional |
| `std::forward_list<T>` | `<forward_list>` | Singly linked list | Forward |
| `std::array<T, N>` | `<array>` | Fixed-size array | Random access |
| `std::string` | `<string>` | Dynamic character array | Random access |
| `std::set<T>` | `<set>` | Ordered set (red-black tree) | Bidirectional |
| `std::multiset<T>` | `<set>` | Ordered set with duplicates | Bidirectional |
| `std::unordered_set<T>` | `<unordered_set>` | Hash set | Forward |
| `std::map<K, V>` | `<map>` | Ordered map | Bidirectional |
| `std::multimap<K, V>` | `<map>` | Ordered map with duplicates | Bidirectional |
| `std::unordered_map<K, V>` | `<unordered_map>` | Hash map | Forward |
| `std::stack<T>` | `<stack>` | LIFO adapter | — |
| `std::queue<T>` | `<queue>` | FIFO adapter | — |
| `std::priority_queue<T>` | `<queue>` | Max-heap adapter | — |

## A.2 Algorithms (in `<algorithm>`)

| Algorithm | Description | Complexity |
|-----------|-------------|------------|
| `sort(first, last)` | Introsort | O(n log n) |
| `stable_sort(first, last)` | Merge sort (stable) | O(n log n) |
| `partial_sort(first, mid, last)` | Heap selection | O(n log k) |
| `nth_element(first, nth, last)` | Quickselect | O(n) avg |
| `lower_bound(first, last, val)` | First ≥ val | O(log n) |
| `upper_bound(first, last, val)` | First > val | O(log n) |
| `binary_search(first, last, val)` | Existence test | O(log n) |
| `merge(first1, last1, first2, last2, out)` | Merge sorted ranges | O(n + m) |
| `inplace_merge(first, mid, last)` | Merge in-place | O(n log n) |
| `remove(first, last, val)` | Remove elements | O(n) |
| `unique(first, last)` | Remove consecutive duplicates | O(n) |
| `reverse(first, last)` | Reverse range | O(n) |
| `rotate(first, mid, last)` | Rotate range | O(n) |
| `next_permutation(first, last)` | Next lexicographic perm | O(n) |
| `accumulate(first, last, init)` | Sum (in `<numeric>`) | O(n) |
| `partial_sum(first, last, out)` | Prefix sums | O(n) |
| `adjacent_difference(first, last, out)` | Differences | O(n) |

## A.3 Chrono (`<chrono>`)

```cpp
using clock = std::chrono::steady_clock;
auto start = clock::now();
// ... work ...
auto end = clock::now();
auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
```

## A.4 Smart Pointers (`<memory>`)

```cpp
auto p = std::make_unique<int>(42);
auto s = std::make_shared<int>(42);
auto w = std::weak_ptr<int>(s);
```

## A.5 Random (`<random>`)

```cpp
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 6);
int roll = dis(gen);
```

## A.6 Utilities

| Utility | Header | Description |
|---------|--------|-------------|
| `std::pair<T1, T2>` | `<utility>` | Two values |
| `std::tuple<Ts...>` | `<tuple>` | Multiple values |
| `std::optional<T>` | `<optional>` | Value or empty |
| `std::variant<Ts...>` | `<variant>` | Type-safe union |
| `std::any` | `<any>` | Type-erased value |
| `std::span<T>` | `<span>` | Non-owning view (C++20) |
| `std::string_view` | `<string_view>` | String view (C++17) |
| `std::function<R(Args...)>` | `<functional>` | Type-erased callable |
---

# Complexity Classes

## B.1 Common Time Complexities

| Complexity | Name | Example |
|-----------|------|---------|
| O(1) | Constant | Array access, hash lookup |
| O(log n) | Logarithmic | Binary search, balanced tree ops |
| O(√n) | Square root | Integer factorization (naive) |
| O(n) | Linear | Linear search, array sum |
| O(n log n) | Linearithmic | Merge sort, heap ops |
| O(n²) | Quadratic | Bubble sort, naive matrix multiply |
| O(n³) | Cubic | Floyd-Warshall, naive matrix multiply |
| O(2ⁿ) | Exponential | Subset generation, TSP |
| O(n!) | Factorial | Permutation generation |

## B.2 NP-Completeness

- **P:** problems solvable in polynomial time
- **NP:** problems verifiable in polynomial time
- **NP-complete:** hardest problems in NP (if any one is solvable in P, all are)
- **NP-hard:** at least as hard as any NP problem

**Canonical NP-complete problems:**
- Boolean satisfiability (SAT)
- Traveling salesman (decision version)
- Vertex cover
- Clique
- Graph coloring
- Subset sum
- Knapsack (decision version)
- Hamiltonian cycle

## B.3 Master Theorem (Refresher)

T(n) = a·T(n/b) + f(n), a ≥ 1, b > 1:

1. If f(n) = O(n^(log_b a - ε)) → T(n) = Θ(n^(log_b a))
2. If f(n) = Θ(n^(log_b a) · log^k n) → T(n) = Θ(n^(log_b a) · log^(k+1) n)
3. If f(n) = Ω(n^(log_b a + ε)) and a·f(n/b) ≤ c·f(n) for c < 1 → T(n) = Θ(f(n))

---

# Mathematical Review

## C.1 Summations

- Arithmetic: 1 + 2 + ... + n = n(n+1)/2
- Squares: 1² + 2² + ... + n² = n(n+1)(2n+1)/6
- Geometric: 1 + r + r² + ... + rⁿ = (r^(n+1) - 1)/(r - 1), r ≠ 1
- Harmonic: H_n = 1 + 1/2 + ... + 1/n ≈ ln n + γ (γ ≈ 0.57721)

## C.2 Logarithms

- log_b (xy) = log_b x + log_b y
- log_b (x/y) = log_b x - log_b y
- log_b (x^k) = k log_b x
- log_a x = log_b x / log_b a
- log n! = Θ(n log n)

## C.3 Combinatorics

- n choose k: C(n, k) = n! / (k! (n-k)!)
- Binomial: (x + y)ⁿ = Σ C(n, k) x^k y^(n-k)
- Number of subsets of an n-element set: 2ⁿ
- Number of permutations: n!
- Expected value: E[X] = Σ x·P(X = x)

## C.4 Important Inequalities

- Jensen: f(E[X]) ≤ E[f(X)] for convex f
- Markov: P(X ≥ a) ≤ E[X] / a
- Chebyshev: P(|X - μ| ≥ kσ) ≤ 1/k²
- Chernoff: P(X ≥ (1+δ)μ) ≤ exp(-μ·δ²/3) for independent Poisson trials
- Union bound: P(∪ A_i) ≤ Σ P(A_i)

## C.5 Useful Constants

- e ≈ 2.71828
- π ≈ 3.14159
- ln 2 ≈ 0.69315
- γ (Euler-Mascheroni) ≈ 0.57722
- φ (golden ratio) ≈ 1.61803

## References and Further Reading

- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Appendices A–C.
- Donald E. Knuth, *The Art of Computer Programming*, Volume 1: Fundamental Algorithms. Section 1.2 (mathematical preliminaries).
- Michael Sipser, *Introduction to the Theory of Computation*, 3rd Edition. Cengage, 2012.
- Oded Goldreich, *Computational Complexity: A Conceptual Perspective*. Cambridge University Press, 2008.
- Michael R. Garey and David S. Johnson, *Computers and Intractability: A Guide to the Theory of NP-Completeness*. W. H. Freeman, 1979.
- cppreference.com — Online C++ standard library reference.
