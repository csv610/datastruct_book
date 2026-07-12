# Maximum Flow and Matching

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The Maximum Flow Problem

A **flow network** is a directed graph G = (V, E) with:
- A **source** s with no incoming edges
- A **sink** t with no outgoing edges
- **Capacity** c(u, v) ≥ 0 for each edge

A **flow** f satisfies:
- **Capacity constraint:** f(u, v) ≤ c(u, v) for all edges
- **Flow conservation:** Σ f(u, v) = Σ f(v, u) for all v ≠ s, t

The **maximum flow problem** finds the maximum possible flow from s to t.

## Dinic's Algorithm

Ford-Fulkerson is the general method: repeatedly find an augmenting path in the residual network and push flow along it. The algorithm's complexity depends on how paths are chosen. **Dinic's algorithm** uses BFS to build a **level graph** (layered network), then sends **blocking flow** via DFS — achieving O(E·V²) time in general and O(E·√V) for bipartite matching. The residual capacity c_f(u, v) = c(u, v) - f(u, v) + f(v, u).

```cpp
struct edge {
    size_t to, rev;
    int capacity;
};

class dinic {
public:
    dinic(size_t n) : graph_(n) {}

    void add_edge(size_t from, size_t to, int capacity) {
        graph_[from].push_back({to, graph_[to].size(), capacity});
        graph_[to].push_back({from, graph_[from].size() - 1, 0});  // reverse edge
    }

    int max_flow(size_t s, size_t t) {
        int flow = 0;
        std::vector<int> level;
        std::vector<size_t> it;

        while (bfs(s, t, level)) {
            it.assign(graph_.size(), 0);
            while (int pushed = dfs(s, t, INT_MAX, level, it)) {
                flow += pushed;
            }
        }
        return flow;
    }

private:
    bool bfs(size_t s, size_t t, std::vector<int>& level) {
        level.assign(graph_.size(), -1);
        std::queue<size_t> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            size_t v = q.front(); q.pop();
            for (const auto& e : graph_[v]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(size_t v, size_t t, int f, std::vector<int>& level,
            std::vector<size_t>& it) {
        if (v == t) return f;
        for (size_t& i = it[v]; i < graph_[v].size(); ++i) {
            edge& e = graph_[v][i];
            if (e.capacity > 0 && level[v] < level[e.to]) {
                int pushed = dfs(e.to, t, std::min(f, e.capacity), level, it);
                if (pushed > 0) {
                    e.capacity -= pushed;
                    graph_[e.to][e.rev].capacity += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<edge>> graph_;
};
```

**Dinic's Complexity:** O(E·V²) general, O(E·√V) for bipartite matching, O(min(V^(2/3), √E)·E) for unit capacities.

## Edmonds-Karp Algorithm

Ford-Fulkerson with BFS for augmenting path selection guarantees O(V·E²) time — each augmentation saturates at least one edge, and the distance from s to any vertex increases monotonically.

```cpp
int edmonds_karp(const std::vector<edge>& graph, size_t s, size_t t) {
    int flow = 0;
    // Build residual adjacency on the fly; same edge struct as Dinic
    while (true) {
        std::vector<edge*> parent(graph.size(), nullptr);
        std::queue<size_t> q;
        q.push(s);
        while (!q.empty() && parent[t] == nullptr) {
            size_t v = q.front(); q.pop();
            for (auto& e : graph[v]) {
                if (e.capacity > 0 && parent[e.to] == nullptr && e.to != s) {
                    parent[e.to] = &e;
                    q.push(e.to);
                }
            }
        }
        if (parent[t] == nullptr) break;
        int aug = INT_MAX;
        for (size_t v = t; v != s; v = parent[v]->to)
            aug = std::min(aug, parent[v]->capacity);
        for (size_t v = t; v != s; v = parent[v]->to) {
            parent[v]->capacity -= aug;
            graph[parent[v]->to][parent[v]->rev].capacity += aug;
        }
        flow += aug;
    }
    return flow;
}
```

In practice, Dinic outperforms Edmonds-Karp on most graphs, but Edmonds-Karp remains important for its theoretical guarantees and simplicity.

## Push-Relabel Algorithm

Instead of augmenting paths, Push-Relabel (Goldberg–Tarjan) works by **pushing** flow locally from overflowing nodes and maintaining a **height label** that guides flow towards t.

```
Algorithm push_relabel(G, s, t):
    for each edge (u, v): f(u,v) = 0
    for each edge (s, v):  f(s,v) = c(s,v)
    height[s] = V
    height[v] = 0 for all other v
    excess[v] = inflow - outflow for each v
    
    while ∃ active vertex v (v ≠ s,t with excess[v] > 0):
        push(v)     — send flow along admissible edges
        relabel(v)  — if no admissible edge, increase height[v]
    return excess[t]
```

```cpp
class push_relabel {
public:
    push_relabel(size_t n) : graph_(n), excess_(n), height_(n) {}

    int max_flow(size_t s, size_t t) {
        height_[s] = graph_.size();
        for (auto& e : graph_[s]) {
            excess_[e.to] += e.capacity;
            graph_[e.to][e.rev].capacity = e.capacity;
            e.capacity = 0;
        }
        while (true) {
            auto it = std::find_if(excess_.begin(), excess_.end(),
                [&](int x) { return x > 0; });
            if (it == excess_.end()) break;
            size_t v = it - excess_.begin();
            if (!discharge(v)) break;
        }
        return excess_[t];
    }

private:
    bool push(size_t v) {
        for (auto& e : graph_[v]) {
            if (e.capacity > 0 && height_[v] == height_[e.to] + 1) {
                int d = std::min(excess_[v], e.capacity);
                e.capacity -= d;
                graph_[e.to][e.rev].capacity += d;
                excess_[v] -= d;
                excess_[e.to] += d;
                return true;
            }
        }
        return false;
    }

    bool discharge(size_t v) {
        while (excess_[v] > 0) {
            if (!push(v)) {
                int min_h = INT_MAX;
                for (const auto& e : graph_[v])
                    if (e.capacity > 0)
                        min_h = std::min(min_h, height_[e.to]);
                if (min_h >= static_cast<int>(graph_.size()))
                    return false;
                height_[v] = min_h + 1;
            }
        }
        return true;
    }

    std::vector<std::vector<edge>> graph_;
    std::vector<int> excess_, height_;
};
```

**Complexity:** O(V²·√E) with gap heuristic and global relabeling. Push-Relabel is often the fastest max-flow algorithm in practice for dense graphs.

## Minimum Cut

The **minimum s-t cut** partitions V into S (containing s) and T (containing t) to minimize the total capacity of edges from S to T.

**Max-Flow Min-Cut Theorem:** The maximum flow equals the minimum cut capacity. The set of vertices reachable from s in the residual graph after max flow is the minimum cut set S.

## Bipartite Matching

Find the maximum set of edges in a bipartite graph with no shared vertices.

```cpp
size_t bipartite_matching(const std::vector<std::vector<size_t>>& adj_left,
                           size_t n_right) {
    size_t n_left = adj_left.size();
    dinic flow(n_left + n_right + 2);
    size_t source = n_left + n_right;
    size_t sink = n_left + n_right + 1;

    for (size_t i = 0; i < n_left; ++i) {
        flow.add_edge(source, i, 1);
        for (size_t j : adj_left[i]) {
            flow.add_edge(i, n_left + j, 1);
        }
    }
    for (size_t j = 0; j < n_right; ++j) {
        flow.add_edge(n_left + j, sink, 1);
    }
    return flow.max_flow(source, sink);
}
```

## Min-Cost Max-Flow

Find a flow of a given amount through a network where each edge has both capacity and per-unit cost. The goal is to minimize total cost while achieving the required flow.

**Approach:** Successive Shortest Augmenting Path (SSP) with potentials (Johnson's). Replace residual edge costs with reduced costs to keep them nonnegative, then run Dijkstra to find the cheapest augmenting path.

```cpp
struct mincost_edge {
    size_t to, rev;
    int capacity, cost;
};

class mincost_flow {
public:
    mincost_flow(size_t n) : graph_(n), potential_(n, 0) {}

    void add_edge(size_t from, size_t to, int cap, int cost) {
        graph_[from].push_back({to, graph_[to].size(), cap, cost});
        graph_[to].push_back({from, graph_[from].size() - 1, 0, -cost});
    }

    std::pair<int, int> flow(size_t s, size_t t, int maxf) {
        int flow = 0, cost = 0;
        while (flow < maxf) {
            std::vector<int> dist(graph_.size(), INT_MAX);
            std::vector<mincost_edge*> prev(graph_.size(), nullptr);
            dist[s] = 0;
            std::priority_queue<std::pair<int, size_t>,
                std::vector<std::pair<int, size_t>>, std::greater<>> pq;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, v] = pq.top(); pq.pop();
                if (dist[v] < d) continue;
                for (auto& e : graph_[v]) {
                    if (e.capacity > 0) {
                        int nd = d + e.cost + potential_[v] - potential_[e.to];
                        if (nd < dist[e.to]) {
                            dist[e.to] = nd;
                            prev[e.to] = &e;
                            pq.push({nd, e.to});
                        }
                    }
                }
            }
            if (dist[t] == INT_MAX) break;

            for (size_t v = 0; v < graph_.size(); ++v)
                if (dist[v] < INT_MAX)
                    potential_[v] += dist[v];

            int add = maxf - flow;
            for (size_t v = t; v != s; ) {
                auto& e = *prev[v];
                add = std::min(add, e.capacity);
                v = graph_[e.to][e.rev].to;  // previous vertex
            }
            for (size_t v = t; v != s; ) {
                auto& e = *prev[v];
                e.capacity -= add;
                graph_[e.to][e.rev].capacity += add;
                cost += add * e.cost;
                v = graph_[e.to][e.rev].to;
            }
            flow += add;
        }
        return {flow, cost};
    }

private:
    std::vector<std::vector<mincost_edge>> graph_;
    std::vector<int> potential_;
};
```

**Application:** Transportation problems (minimize shipping cost with supply/demand constraints), optimal task assignment, min-cost matching, and network design.

## Application: Project Selection

Given projects with profits (positive or negative) and prerequisites, select the set of projects that maximizes total profit.

Model as min-cut: source connects to projects with positive profit (capacity = profit), projects with negative profit connect to sink (capacity = -profit), prerequisites become infinite-capacity edges. The minimum cut gives the optimal selection.

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Forgetting to add reverse edges in residual graph | Flow can't be undone, wrong max flow value | Always add edge(v, u, 0) for each edge(u, v, cap) |
| Integer overflow on large capacity networks | Negative flow values, wrong result | Use `int64_t` or `long long` for capacities |
| Not resetting `visited` between BFS/DFS phases | Algorithm terminates early, suboptimal flow | Clear the visited array at the start of each BFS/DFS |
| Infinite loop if blocking flow makes no progress | No termination | Track whether any augmenting path was found; break if not |
| Dinic's level graph not rebuilt after a blocking flow | Stale levels, wrong augmenting paths | Rebuild level graph each phase |
| Using adjacency matrix for large sparse flow graphs | O(V²) memory where O(E) would suffice | Use adjacency list |
| Assuming min-cost flow costs are non-negative | Negative cycles exploited, unbounded flow | Use potentials (Johnson's) to reweigh edges, or detect negative cycles |

## Summary

1. **Maximum flow** finds the maximum throughput in a network.
2. **Dinic's algorithm** with level graph + blocking flow is the most practical general algorithm.
3. **The max-flow min-cut theorem** connects flow to graph partitioning.
4. **Bipartite matching** reduces to flow.
5. **Min-cost max-flow** extends the model with edge costs, supporting transportation and assignment problems.
6. Applications range from transportation to image segmentation to team assignment.

## Exercises

### Drill

1. For the flow network with edges s→a(10), s→b(5), a→b(15), a→t(5), b→t(10), find the maximum flow from s to t using Ford-Fulkerson. Show the augmenting path and residual network at each step.

2. What is the min-cut value in the network above? Which edges are in the min-cut?

3. For bipartite matching: given left set {A, B, C}, right set {X, Y, Z}, and edges A-X, A-Y, B-Y, C-Z, find the maximum matching. What is the matching size?

4. Show that the max-flow min-cut theorem holds for the network from Exercise 1: compute the min-cut capacity and verify it equals the max-flow value.

5. What is the time complexity of:
   a) Ford-Fulkerson with integer capacities
   b) Dinic's algorithm on a general graph
   c) Dinic's algorithm on a unit-capacity network
   d) Edmonds-Karp (Ford-Fulkerson with BFS)

### Application

6. Implement the **Edmonds-Karp algorithm** (Ford-Fulkerson with BFS). Compare its performance with Dinic on random networks of various sizes. Does BFS guarantee O(V·E²) in practice?

7. Use maximum flow to find the **maximum matching in a general graph** (reduction to flow). Compare with the Hopcroft-Karp algorithm for bipartite graphs.

8. Implement the **min-cost max-flow** algorithm using successive shortest augmenting paths (with potentials). Find the cheapest flow of a given amount through a network with per-unit costs on edges.

9. Solve the **baseball elimination** problem using max flow: given league standings and remaining games, determine if a team can still finish first. Test on real baseball data.

10. Model the **image segmentation** problem as a min-cut: each pixel is a node with edges to source (foreground probability) and sink (background probability). Cut edges between dissimilar pixels. Implement for a small grayscale image.

11. Implement **Dinic's algorithm** with scaling (capacity scaling Dinic). Compare its performance with standard Dinic on networks with large capacities.

12. Use max flow to find the **edge connectivity** of a graph (size of the minimum edge cut). Compute it for the graph in Chapter 12, Exercise 1.

### Research

13. **(Push-Relabel)** Implement the Push-Relabel algorithm (Goldberg-Tarjan) for max flow. Compare its performance with Dinic on large networks. Which is faster in practice?

14. **(Global min-cut)** Implement the Stoer-Wagner algorithm for global minimum cut (not just s-t). Compare with running V-1 max-flow computations.

15. **(Unit capacity networks)** Prove that Dinic runs in O(min(V^(2/3), √E) · E) on unit-capacity networks. Implement and verify experimentally.

16. **(Max flow applications)** Research the reduction of the **circulation with demands** problem to max flow. Implement a solver for the problem: given supplies and demands at nodes, find a feasible flow.

## References and Further Reading

- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapter 24 (max flow).
- L. R. Ford Jr. and D. R. Fulkerson, "Maximal Flow through a Communication Network" — Canadian Journal of Mathematics, 1956.
- E. A. Dinic, "Algorithm for Solution of a Problem of Maximum Flow in a Network with Power Estimation" — Soviet Math. Doklady, 1970.
- Jack Edmonds and Richard M. Karp, "Theoretical Improvements in Algorithmic Efficiency for Network Flow Problems" — JACM, 1972.
- Andrew V. Goldberg and Robert E. Tarjan, "A New Approach to the Maximum-Flow Problem" — JACM, 1988.
- Ravindra K. Ahuja, Thomas L. Magnanti, and James B. Orlin, *Network Flows: Theory, Algorithms, and Applications*. Prentice Hall, 1993.
- Mehdi Stoer and Frank Wagner, "A Simple Min-Cut Algorithm" — JACM, 1997.
- John E. Hopcroft and Richard M. Karp, "An n^(5/2) Algorithm for Maximum Matchings in Bipartite Graphs" — SIAM Journal on Computing, 1973.
