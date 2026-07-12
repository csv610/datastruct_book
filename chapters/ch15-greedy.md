# The Greedy Method

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The General Method

A **greedy algorithm** makes the locally optimal choice at each step, hoping to find the global optimum. For many problems, this works — the locally optimal choice leads to the globally optimal solution. For others, it does not.

### Greedy Algorithm Template

```
1. Initialize solution = empty
2. While a solution has not been found:
   a. Select the next best candidate (locally optimal)
   b. If the candidate is feasible, add it to the solution
   c. If the candidate is not feasible, discard it
3. Return solution
```

### When Greedy Works

Greedy algorithms are correct when the problem exhibits:
- **Optimal substructure:** an optimal solution contains optimal solutions to subproblems
- **Greedy choice property:** a globally optimal solution can be reached by making locally optimal choices

## Container Loading

**Problem:** We have a ship with capacity C and containers with weights w₁, w₂, ..., wₙ. Load as many containers as possible without exceeding capacity.

**Greedy choice:** Load the lightest container first.

```cpp
std::vector<size_t> load_containers(double capacity, std::span<const double> weights) {
    // Pair (weight, original index)
    std::vector<std::pair<double, size_t>> items;
    for (size_t i = 0; i < weights.size(); ++i) items.push_back({weights[i], i});
    std::sort(items.begin(), items.end());

    std::vector<size_t> loaded;
    double current = 0;
    for (const auto& [w, idx] : items) {
        if (current + w <= capacity) {
            current += w;
            loaded.push_back(idx);
        }
    }
    return loaded;
}
```

**Correctness:** Optimal substructure — removing the lightest container from an optimal solution gives an optimal solution for the remaining capacity. Greedy choice — if an optimal solution does not contain the lightest container, we can replace a heavier one with it.

## The Fractional Knapsack Problem

**Problem:** Items have weights wᵢ and values vᵢ. We can take fractions of items. Maximize value for a knapsack of capacity C.

**Greedy choice:** Take the item with the highest value-to-weight ratio first.

```cpp
struct item { double weight, value; };

double fractional_knapsack(double capacity, std::span<const item> items) {
    std::vector<size_t> order(items.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        return items[a].value / items[a].weight > items[b].value / items[b].weight;
    });

    double total_value = 0.0;
    for (size_t i : order) {
        if (capacity >= items[i].weight) {
            capacity -= items[i].weight;
            total_value += items[i].value;
        } else {
            total_value += items[i].value * (capacity / items[i].weight);
            break;
        }
    }
    return total_value;
}
```

**Complexity:** O(n log n). The 0/1 knapsack (no fractions) is not solvable by greedy — it requires dynamic programming (Chapter 17).

## Task Scheduling

**Problem:** Schedule tasks with deadlines and profits. Each unit of time can handle one task. Maximize total profit.

**Greedy choice:** Process tasks in order of decreasing profit. Schedule each task as late as possible.

```cpp
struct task {
    int deadline;  // must be completed by this time
    int profit;
};

int schedule_tasks(std::span<task> tasks) {
    // Sort by profit descending
    std::vector<task> sorted(tasks.begin(), tasks.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const task& a, const task& b) { return a.profit > b.profit; });

    int max_deadline = 0;
    for (const auto& t : sorted) max_deadline = std::max(max_deadline, t.deadline);

    std::vector<int> slots(max_deadline + 1, -1);  // -1 = empty
    int total_profit = 0;

    for (const auto& [d, p] : sorted) {
        // Find the latest available slot before the deadline
        for (int t = d; t >= 1; --t) {
            if (slots[t] == -1) {
                slots[t] = p;
                total_profit += p;
                break;
            }
        }
    }
    return total_profit;
}
```

**Optimization:** Use Union-Find to find the latest available slot in O(α(n)) per task.

## Single-Source Shortest Paths (Dijkstra)

See Chapter 12. Dijkstra's algorithm is greedy: at each step, extract the vertex with the smallest distance estimate.

## Minimum Spanning Tree

Both Kruskal's and Prim's algorithms are greedy (Chapter 12):
- **Prim:** grows a tree by always adding the cheapest edge connecting the tree to a new vertex
- **Kruskal:** sorts edges by weight and adds them if they don't create a cycle

## Huffman Coding

**Problem:** Given character frequencies, build a prefix-free binary code that minimizes the total encoded length.

**Greedy choice:** Repeatedly combine the two characters with the smallest frequencies.

```cpp
struct huffman_node {
    char ch;
    int freq;
    huffman_node* left;
    huffman_node* right;
};

struct compare {
    bool operator()(huffman_node* a, huffman_node* b) {
        return a->freq > b->freq;
    }
};

huffman_node* build_huffman_tree(const std::unordered_map<char, int>& frequencies) {
    std::priority_queue<huffman_node*, std::vector<huffman_node*>, compare> pq;

    for (const auto& [ch, freq] : frequencies) {
        pq.push(new huffman_node{ch, freq, nullptr, nullptr});
    }

    while (pq.size() > 1) {
        auto* left = pq.top(); pq.pop();
        auto* right = pq.top(); pq.pop();
        auto* parent = new huffman_node{'\0', left->freq + right->freq, left, right};
        pq.push(parent);
    }
    return pq.top();
}

void build_codes(huffman_node* root, std::string prefix,
                 std::unordered_map<char, std::string>& codes) {
    if (!root->left && !root->right) {
        codes[root->ch] = prefix;
        return;
    }
    build_codes(root->left, prefix + "0", codes);
    build_codes(root->right, prefix + "1", codes);
}
```

**Complexity:** O(n log n) where n is the alphabet size.

**Example:**
```
frequencies: A:45, B:13, C:12, D:16, E:9, F:5

Huffman codes:
A: 0       (1 bit)
B: 101     (3 bits)
C: 100     (3 bits)
D: 111     (3 bits)
E: 1101    (4 bits)
F: 1100    (4 bits)

Total bits = 45·1 + 13·3 + 12·3 + 16·3 + 9·4 + 5·4 = 224
vs fixed 3-bit code = 100·3 = 300
Savings: 25%
```

## Greedy Does Not Always Work

**The 0/1 Knapsack Problem:** Items must be taken whole (not fractional). Greedy fails:
```
Item A: weight 5, value 50  (ratio 10)
Item B: weight 10, value 60 (ratio 6)
Item C: weight 20, value 140 (ratio 7)
Capacity: 20

Greedy (by ratio): A + B = 110
Optimal: C alone = 140
```

**Coin Change (US denominations):** Greedy works for US coins (25, 10, 5, 1) but fails for other systems:
```
Denominations: 1, 3, 4
Amount: 6
Greedy: 4 + 1 + 1 = 3 coins
Optimal: 3 + 3 = 2 coins
```

When greedy fails, we use dynamic programming (Chapter 17) or backtracking (Chapter 18).

## Common Bugs and Pitfalls

- **Assuming greedy works without proof** — many real-world problems look greedy-solvable but aren't. Always prove optimal substructure and the greedy choice property before deployment.
- **Confusing fractional with 0/1 knapsack** — the fractional variant is greedy-solvable; the 0/1 variant is not. Accidentally applying greedy to 0/1 knapsack in a production inventory system would produce suboptimal shipping costs.
- **Dijkstra with negative edges** — Dijkstra's greedy selection fails on graphs with negative edge weights. Use Bellman-Ford instead. This is a common bug in GPS routing prototypes.
- **Huffman with unequal code lengths** — the Huffman algorithm assumes each symbol's code is a whole number of bits. For arithmetic coding (used in JPEG, H.264), the greedy Huffman approach does not apply.
- **Interval scheduling tie-breaking** — when intervals have equal finish times, the greedy choice (earliest finish) still works, but incorrect tie-breaking can eliminate valid schedules.

## Summary

1. **Greedy algorithms** make locally optimal choices, hoping for a global optimum.
2. **Correctness** requires optimal substructure and the greedy choice property.
3. **Container loading, fractional knapsack, Dijkstra, MST, and Huffman coding** are canonical greedy successes.
4. **0/1 knapsack and coin change** (in general) show that greedy does not always work.
5. Industrial applications include **CDN content placement** (Akamai), **Huffman compression** (gzip, PNG), **Dijkstra routing** (Google Maps, OSRM), **interval scheduling** (cloud compute scheduling), and **Huffman coding** (JPEG, MP3).
6. Greedy is a **problem-solving approach**, not a fixed algorithm — the art is in identifying the right greedy choice.

## Exercises

### Drill

1. For each industrial scenario, identify the greedy choice and determine (conceptually) whether it produces an optimal solution:
   a) **AWS Spot Instance bidding** — you have a budget of \$100/hr and need to run 10 batch jobs, each with a different spot price. Greedy: bid the lowest price first.
   b) **CDN content caching** — 1TB cache, 1000 popular files with different request rates. Greedy: cache the most-requested files first.
   c) **YouTube transcoding** — 50 videos to encode, 10 encoding servers with different speeds. Greedy: assign the shortest video to the fastest server.
   d) **Packet routing in a network** — forward each packet along the link with the lowest current latency.

2. A shipping company (like FedEx) loads containers of weights [5, 7, 10, 3, 18, 2, 9] onto a truck with capacity 30. Using the lightest-first greedy, which containers are loaded? What is the total weight? Is this optimal?

3. For the fractional knapsack with C = 50 and items:
   | Item | Weight (kg) | Value ($) |
   |------|-------------|-----------|
   | Gold | 10 | 600 |
   | Silver | 20 | 500 |
   | Copper | 30 | 360 |
   Compute the optimal solution using greedy by value/weight. What is the total value?

4. Trace Google Maps' Dijkstra on a graph with vertices (A, B, C, D, E) and edges: A→B(4), A→C(2), B→C(1), B→D(5), C→D(8), C→E(10), D→E(2), D→T(6), E→T(3), starting from A to T. Show distance estimates after each vertex extraction.

5. Trace Huffman coding on character frequencies from an English novel: E:12.7%, T:9.1%, A:8.2%, O:7.5%, I:7.0%, N:6.7%, S:6.3%, H:6.1%, R:6.0%. Build the tree, compute the weighted path length, and compare with fixed 4-bit encoding (16 symbols → 4 bits).

6. What is the greedy choice in Google's PageRank-optimized crawling? In Akamai's CDN edge server selection? Why are both greedy?

### Application

7. **CDN content placement (Akamai).** Akamai has edge servers worldwide. Given request rates for 1000 video files from 50 geographic regions, place copies on 10 edge servers to minimize total latency. Implement the greedy algorithm: place the most-requested file on the server closest to its requestors. Measure total latency vs an optimal (exhaustive) placement for small N.

8. **Spot instance bidding on AWS.** You have 100 batch jobs with different deadlines and spot prices. Each job requires 1 hour on a single instance. You have \$500 total budget. Implement a greedy scheduler: sort by profit margin (value/hour ÷ price) and schedule the most profitable jobs first. Compare total profit with an optimal DP solution (Chapter 17). How far from optimal is the greedy result?

9. **Huffman compression for HTTP/2.** HTTP/2 uses HPACK header compression (a variant of Huffman). Implement Huffman encoding/decoding for HTTP headers: given a set of header names and frequencies from 10,000 real HTTP requests, build the Huffman tree and compress a sample request. Compare compression ratio with gzip and with no compression.

10. **Interval scheduling for cloud computing.** AWS EC2 offers reserved instances in 1-hour intervals. Your workloads specify start and end times (100 workloads, each with value \$). Implement earliest-finish-time-first scheduling to maximize the number of workloads. Then modify to maximize total value (weighted interval scheduling). Show that the unweighted greedy is optimal but the weighted variant needs DP.

11. **Greedy set cover for test suite minimization.** Given 100 test cases and 50 code modules (each test covers certain modules), select the minimum number of tests that cover all modules. Implement the greedy set cover algorithm (pick the test covering the most uncovered modules). Compare the solution size with the optimal (exponential) solution for small cases. What approximation ratio do you observe?

12. **Ad slot allocation.** A publisher has 10 ad slots per page view and 100 ad campaigns each with budget, CPM, and target audience size. Implement a greedy allocation: assign each slot to the campaign with the highest bid × relevance score. Measure total revenue vs a linear programming solution for 1000 page views.

### Research

13. **(Matroids)** Read about matroids and prove that the minimum spanning tree problem is a graphic matroid. Then find a real problem (e.g., maximum-weight forest in a network design problem) that is also a matroid. What does the matroid property tell you about the applicability of greedy?

14. **(Greedy approximation for set cover)** Prove that the greedy set cover algorithm is a (ln n)-approximation. Implement it for a real-world test suite minimization problem (e.g., minimize the number of test cases while maintaining code coverage). On real coverage data, does the greedy solution ever exceed ln(n) × optimal?

15. **(Adaptive Huffman)** Implement the Faller-Gallager-Knuth adaptive Huffman algorithm that updates the code tree incrementally (used in `bzip2`). Compare its compression ratio with static Huffman on a 10MB text file. How much worse is the adaptive version, and why is it useful for streaming?

16. **(Scheduling theory)** Read about Smith's rule and the EDD rule for minimizing maximum lateness. Implement a greedy scheduler for a real manufacturing line simulation: 100 jobs with processing times and due dates. Show that EDD minimizes maximum lateness.

17. **(Greedy for submodular maximization)** Read about the greedy (1-1/e)-approximation for submodular maximization under a cardinality constraint. Implement it for a real sensor placement problem: given 100 candidate locations and a coverage function, place 10 sensors to maximize coverage. Why is submodularity the right model for diminishing returns?

## References and Further Reading

- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapter 16 (greedy), Chapter 35 (approximation).
- David A. Huffman, "A Method for the Construction of Minimum-Redundancy Codes" — Proceedings of the IRE, 1952.
- Jack Edmonds, "Matroids and the Greedy Algorithm" — Mathematical Programming, 1971.
- Jon Kleinberg and Éva Tardos, *Algorithm Design*. Chapter 4 (greedy). Addison-Wesley, 2005.
- Steven S. Skiena, *The Algorithm Design Manual*, 2nd Edition. Springer, 2008.
- Eugene L. Lawler, *Combinatorial Optimization: Networks and Matroids*. Holt, Rinehart and Winston, 1976.
