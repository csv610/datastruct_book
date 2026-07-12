# Graphs and Graph Algorithms

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The Graph ADT

A **graph** G = (V, E) consists of a set V of **vertices** (nodes) and a set E of **edges** connecting pairs of vertices. Graphs model networks: social connections, road maps, circuit connectivity, web links, and more.

### Terminology

- **Undirected graph:** edges have no direction — (u, v) = (v, u)
- **Directed graph (digraph):** edges have direction — u→v ≠ v→u
- **Weighted graph:** each edge has a numerical weight
- **Path:** sequence of vertices connected by edges
- **Cycle:** a path where the first and last vertex are the same
- **Connected graph:** there is a path between every pair of vertices
- **Complete graph:** every pair of vertices is connected by an edge
- **Degree (undirected):** number of edges incident to a vertex
- **In-degree / Out-degree (directed):** number of incoming / outgoing edges

```
Undirected:    Directed:
   A—B          A→B
   |\           ↓↑
   C—D          C→D
```

### Graph Representations

**Adjacency Matrix:** O(V²) space, O(1) edge lookup.

```
    A B C D
  A 0 1 1 1
  B 1 0 0 1
  C 1 0 0 1
  D 1 1 1 0
```

**Adjacency List:** O(V + E) space, O(degree(v)) neighbor iteration.

```
A → B → C → D
B → A → D
C → A → D
D → A → B → C
```

## Graph Classes

### Adjacency Matrix

```cpp
class graph_matrix {
public:
    graph_matrix(size_t n) : n_(n), adj_(n, std::vector<bool>(n, false)) {}

    void add_edge(size_t u, size_t v) {
        adj_[u][v] = true;
        adj_[v][u] = true;  // undirected
    }

    bool has_edge(size_t u, size_t v) const {
        return adj_[u][v];
    }

    std::vector<size_t> neighbors(size_t v) const {
        std::vector<size_t> result;
        for (size_t i = 0; i < n_; ++i) {
            if (adj_[v][i]) result.push_back(i);
        }
        return result;
    }

    size_t size() const noexcept { return n_; }

private:
    size_t n_;
    std::vector<std::vector<bool>> adj_;
};
```

### Adjacency List

```cpp
template <typename Weight = int>
class graph_list {
public:
    struct edge {
        size_t to;
        Weight weight;
    };

    explicit graph_list(size_t n) : adj_(n) {}

    void add_edge(size_t u, size_t v, Weight w = Weight{}) {
        adj_[u].push_back({v, w});
        adj_[v].push_back({u, w});  // undirected
    }

    const std::vector<edge>& neighbors(size_t v) const {
        return adj_[v];
    }

    size_t size() const noexcept { return adj_.size(); }
    size_t edge_count() const {
        size_t e = 0;
        for (const auto& adj : adj_) e += adj.size();
        return e / 2;  // undirected
    }

private:
    std::vector<std::vector<edge>> adj_;
};
```

## Graph Traversal

### Depth-First Search (DFS)

DFS explores as far as possible along each branch before backtracking. Uses a stack (implicitly via recursion, or explicitly).

```cpp
void dfs(const graph_list<>& g, size_t start, auto& visit) {
    std::vector<bool> visited(g.size(), false);
    std::function<void(size_t)> dfs_impl = [&](size_t v) {
        visited[v] = true;
        visit(v);
        for (const auto& [to, _] : g.neighbors(v)) {
            if (!visited[to]) dfs_impl(to);
        }
    };
    dfs_impl(start);
}
```

**Applications:**
- Finding connected components
- Topological sort
- Detecting cycles
- Solving mazes
- Biconnectivity and articulation points

### Breadth-First Search (BFS)

BFS explores all neighbors at the current depth before going deeper. Uses a queue.

```cpp
std::vector<size_t> bfs(const graph_list<>& g, size_t start) {
    std::vector<size_t> distance(g.size(), SIZE_MAX);
    std::queue<size_t> q;
    
    distance[start] = 0;
    q.push(start);
    
    while (!q.empty()) {
        size_t v = q.front(); q.pop();
        for (const auto& [to, _] : g.neighbors(v)) {
            if (distance[to] == SIZE_MAX) {
                distance[to] = distance[v] + 1;
                q.push(to);
            }
        }
    }
    return distance;
}
```

**Applications:**
- Shortest path in unweighted graphs
- Finding connected components
- Web crawling
- Social network analysis (degrees of separation)

### Connected Components

```cpp
std::vector<size_t> connected_components(const graph_list<>& g) {
    size_t n = g.size();
    std::vector<size_t> component(n, SIZE_MAX);
    size_t comp_id = 0;

    for (size_t v = 0; v < n; ++v) {
        if (component[v] != SIZE_MAX) continue;
        
        // BFS from v
        std::queue<size_t> q;
        q.push(v);
        component[v] = comp_id;
        
        while (!q.empty()) {
            size_t u = q.front(); q.pop();
            for (const auto& [to, _] : g.neighbors(u)) {
                if (component[to] == SIZE_MAX) {
                    component[to] = comp_id;
                    q.push(to);
                }
            }
        }
        ++comp_id;
    }
    return component;
}
```

## Topological Sort

A **topological order** of a directed acyclic graph (DAG) is a linear ordering of vertices such that for every edge u→v, u comes before v. Only DAGs have a topological order.

```cpp
std::vector<size_t> topological_sort(const graph_list<>& g) {
    size_t n = g.size();
    std::vector<size_t> in_degree(n, 0);
    
    // Compute in-degrees
    for (size_t v = 0; v < n; ++v) {
        for (const auto& [to, _] : g.neighbors(v)) {
            ++in_degree[to];
        }
    }

    // Kahn's algorithm
    std::queue<size_t> q;
    for (size_t v = 0; v < n; ++v) {
        if (in_degree[v] == 0) q.push(v);
    }

    std::vector<size_t> result;
    while (!q.empty()) {
        size_t v = q.front(); q.pop();
        result.push_back(v);
        for (const auto& [to, _] : g.neighbors(v)) {
            if (--in_degree[to] == 0) q.push(to);
        }
    }
    return result;
}
```

**Application:** Course prerequisite ordering, build system dependency resolution.

## Minimum Spanning Tree

A **spanning tree** connects all vertices of a graph with exactly V-1 edges. A **minimum spanning tree (MST)** is a spanning tree with minimum total edge weight.

### Kruskal's Algorithm

Sort edges by weight, add edges that don't create cycles (using Union-Find).

```cpp
struct weighted_edge {
    size_t from, to;
    int weight;
};

std::vector<weighted_edge> kruskal_mst(size_t n,
                                        std::vector<weighted_edge> edges) {
    std::sort(edges.begin(), edges.end(),
              [](const auto& a, const auto& b) { return a.weight < b.weight; });

    union_find uf(n);
    std::vector<weighted_edge> mst;

    for (const auto& e : edges) {
        if (!uf.same_set(e.from, e.to)) {
            uf.union_sets(e.from, e.to);
            mst.push_back(e);
            if (mst.size() == n - 1) break;
        }
    }
    return mst;
}
```

**Complexity:** O(E log E) due to sorting. With Union-Find, cycle check is near-constant.

### Prim's Algorithm

Grow the MST from a starting vertex, always adding the cheapest edge connecting the tree to a vertex outside it.

```cpp
std::vector<weighted_edge> prim_mst(const graph_list<int>& g) {
    size_t n = g.size();
    std::vector<bool> in_mst(n, false);
    std::vector<int> key(n, INT_MAX);
    std::vector<size_t> parent(n, SIZE_MAX);
    
    using entry = std::pair<int, size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    
    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [_, v] = pq.top(); pq.pop();
        if (in_mst[v]) continue;
        in_mst[v] = true;

        for (const auto& [to, w] : g.neighbors(v)) {
            if (!in_mst[to] && w < key[to]) {
                key[to] = w;
                parent[to] = v;
                pq.push({w, to});
            }
        }
    }

    std::vector<weighted_edge> mst;
    for (size_t v = 1; v < n; ++v) {
        if (parent[v] != SIZE_MAX) {
            mst.push_back({parent[v], v, key[v]});
        }
    }
    return mst;
}
```

**Complexity:** O((V + E) log V) with a binary heap.

## Shortest Paths

### Dijkstra's Algorithm

Finds shortest path from a source to all vertices in a graph with non-negative weights.

```cpp
std::vector<int> dijkstra(const graph_list<int>& g, size_t source) {
    size_t n = g.size();
    std::vector<int> distance(n, INT_MAX);
    std::vector<bool> visited(n, false);
    
    using entry = std::pair<int, size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> pq;
    
    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [dist, v] = pq.top(); pq.pop();
        if (visited[v]) continue;
        visited[v] = true;

        for (const auto& [to, w] : g.neighbors(v)) {
            if (dist + w < distance[to]) {
                distance[to] = dist + w;
                pq.push({distance[to], to});
            }
        }
    }
    return distance;
}
```

**Complexity:** O((V + E) log V). **Cannot handle negative edges.**

### Bellman-Ford Algorithm

Handles negative edge weights (detects negative cycles).

```cpp
std::optional<std::vector<int>> bellman_ford(
    const std::vector<weighted_edge>& edges, size_t n, size_t source) {
    
    std::vector<int> distance(n, INT_MAX / 2);
    distance[source] = 0;

    // Relax all edges V-1 times
    for (size_t i = 0; i < n - 1; ++i) {
        for (const auto& [u, v, w] : edges) {
            if (distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
            }
        }
    }

    // Check for negative cycles
    for (const auto& [u, v, w] : edges) {
        if (distance[u] + w < distance[v]) {
            return std::nullopt;  // negative cycle detected
        }
    }
    return distance;
}
```

**Complexity:** O(VE).

### Floyd-Warshall (All Pairs)

```cpp
std::vector<std::vector<int>> floyd_warshall(
    const std::vector<weighted_edge>& edges, size_t n) {
    
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INT_MAX / 2));
    for (size_t i = 0; i < n; ++i) dist[i][i] = 0;
    for (const auto& [u, v, w] : edges) dist[u][v] = w;

    for (size_t k = 0; k < n; ++k) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}
```

**Complexity:** O(V³). Simple but costly; use only for dense graphs or small V.

## Strongly Connected Components

A **strongly connected component (SCC)** in a directed graph is a maximal set of vertices where every vertex is reachable from every other.

### Kosaraju's Algorithm

```cpp
std::vector<size_t> kosaraju_scc(const graph_list<>& g) {
    size_t n = g.size();
    std::vector<bool> visited(n, false);
    std::vector<size_t> order;
    
    // First pass: DFS to get finish order
    std::function<void(size_t)> dfs1 = [&](size_t v) {
        visited[v] = true;
        for (const auto& [to, _] : g.neighbors(v)) {
            if (!visited[to]) dfs1(to);
        }
        order.push_back(v);
    };
    for (size_t v = 0; v < n; ++v) if (!visited[v]) dfs1(v);

    // Second pass: DFS on reversed graph
    graph_list<> reversed(n);
    for (size_t v = 0; v < n; ++v)
        for (const auto& [to, _] : g.neighbors(v))
            reversed.add_edge(to, v, 0);

    std::vector<size_t> component(n, SIZE_MAX);
    size_t comp_id = 0;
    visited.assign(n, false);

    std::function<void(size_t)> dfs2 = [&](size_t v) {
        visited[v] = true;
        component[v] = comp_id;
        for (const auto& [to, _] : reversed.neighbors(v)) {
            if (!visited[to]) dfs2(to);
        }
    };

    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        if (!visited[*it]) {
            dfs2(*it);
            ++comp_id;
        }
    }
    return component;
}
```

## Application: Social Network Analysis

```cpp
struct social_graph {
    graph_list<> connections;
    std::vector<std::string> names;

    // Degrees of separation from a person
    std::vector<size_t> degrees_of_separation(const std::string& person) {
        auto it = std::find(names.begin(), names.end(), person);
        if (it == names.end()) return {};
        return bfs(connections, it - names.begin());
    }

    // Friend recommendations (friends of friends not already connected)
    std::vector<std::string> recommendations(const std::string& person) {
        auto it = std::find(names.begin(), names.end(), person);
        if (it == names.end()) return {};
        size_t idx = it - names.begin();

        std::unordered_set<size_t> friends;
        for (const auto& [to, _] : connections.neighbors(idx)) {
            friends.insert(to);
        }

        std::unordered_set<size_t> candidates;
        for (size_t f : friends) {
            for (const auto& [to, _] : connections.neighbors(f)) {
                if (to != idx && !friends.count(to)) {
                    candidates.insert(to);
                }
            }
        }

        std::vector<std::string> result;
        for (size_t c : candidates) result.push_back(names[c]);
        return result;
    }
};
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Forgetting to mark nodes as visited in DFS/BFS | Infinite loop, stack overflow | Mark visited when first discovered, not when processed |
| Stack overflow on deep DFS recursion | Crash for graphs with > 10^4 nodes | Use iterative DFS with an explicit stack |
| Assuming the graph is connected | Algorithm only visits one component | Iterate over all vertices; run algorithm on each unvisited one |
| Using `int` for edge weights when negative edges exist | Wrong shortest paths with Dijkstra | Use Bellman-Ford if negative edges; or detect them first |
| Not checking for negative cycles in Bellman-Ford | Infinite loop, no valid shortest path | Run one extra iteration to detect negative cycles |
| Confusing directed vs undirected — adding edge only one way | Missing connections, wrong traversal | For undirected, add both (u,v) and (v,u) |
| Using adjacency matrix for a sparse graph (E ≪ V²) | Memory waste (V²), slow iteration | Use adjacency list for sparse graphs |

## Complexity Summary

| Algorithm | Time | Space |
|-----------|------|-------|
| DFS / BFS | O(V + E) | O(V) |
| Topological sort | O(V + E) | O(V) |
| Kruskal's MST | O(E log E) | O(V + E) |
| Prim's MST | O((V + E) log V) | O(V) |
| Dijkstra | O((V + E) log V) | O(V) |
| Bellman-Ford | O(VE) | O(V) |
| Floyd-Warshall | O(V³) | O(V²) |
| Kosaraju SCC | O(V + E) | O(V) |

## Exercises

### Drill

1. For the following graph:
   ```
   A --2-- B --1-- C
   |       |       |
   4       3       2
   |       |       |
   D --1-- E --3-- F
   ```
   a) Perform DFS starting from A. List the vertices in discovery order (assume neighbor ordering by vertex label).
   b) Perform BFS starting from A.
   c) Run Dijkstra from A and report the shortest distances to all vertices.

2. Classify each edge in a DFS traversal of the graph above as: tree edge, back edge, forward edge, or cross edge.

3. What is the result of topological sort on the DAG? What happens if the graph has a cycle?
   ```
   A → B → C → D
   ↓   ↓
   E → F
   ```

4. For the graph in Exercise 1, find the Minimum Spanning Tree using:
   a) Kruskal's algorithm (show edge selection order)
   b) Prim's algorithm starting from A

5. True or false:
   a) Every DFS tree is a spanning tree
   b) BFS finds the shortest paths in any graph
   c) Dijkstra works correctly with negative edge weights
   d) A complete graph with n vertices has nⁿ⁻² spanning trees (Cayley's formula)

### Application

6. Implement a **word ladder** solver using BFS (e.g., cat → cot → cog → dog). Find the shortest transformation sequence between two words of the same length.

7. Implement Dijkstra's algorithm returning both the shortest distance and the path. Test on the graph from Exercise 1.

8. Implement Borůvka's algorithm for MST (the earliest MST algorithm, 1926). Compare its performance with Kruskal and Prim.

9. Detect cycles in a directed graph using DFS with three-color marking (white/gray/black). Extend this to find all edges in any cycle.

10. Implement A* search for a grid-based pathfinding problem with obstacles. Compare path length and nodes visited with Dijkstra.

11. Given a bipartite graph, determine if a perfect matching exists using the augmenting path algorithm (Kőnig's theorem).

12. Implement Kosaraju's algorithm for strongly connected components. Test on a directed graph with known SCCs.

13. Implement Bellman-Ford for a graph with negative edges. Detect negative-weight cycles reachable from the source.

14. Implement the Floyd-Warshall all-pairs shortest path algorithm. Use it to find the graph's diameter (longest shortest path).

15. Implement an Eulerian path algorithm (Hierholzer's algorithm). Determine whether a given graph has an Eulerian path or circuit.

### Research

16. **(PageRank)** Read about the PageRank algorithm. Implement it and run it on a small web graph (≥ 10 nodes, with links). How does the rank distribution look? Which pages get the highest rank?

17. **(Max flow)** Read Chapter 20 on maximum flow. Implement the Edmonds-Karp algorithm (Ford-Fulkerson with BFS) for a small flow network and compute the min-cut.

18. **(Graph isomorphism)** Research the graph isomorphism problem. Implement a simple (exponential) backtracking algorithm for graph isomorphism. Test it on small graphs. Why is graph isomorphism not known to be either P or NP-complete?

19. **(Johnson's algorithm)** Implement Johnson's algorithm for all-pairs shortest paths, which runs Bellman-Ford once and Dijkstra for each vertex. Compare it with Floyd-Warshall on sparse and dense graphs.

20. **(2-SAT)** Reduce the 2-SAT (2-satisfiability) problem to strongly connected components. Implement a 2-SAT solver using Kosaraju's algorithm.

## References and Further Reading

- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapters 20–25.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Chapter 4.
- Edsger W. Dijkstra, "A Note on Two Problems in Connexion with Graphs" — Numerische Mathematik, 1959.
- Robert E. Tarjan, "Depth-First Search and Linear Graph Algorithms" — SIAM Journal on Computing, 1972.
- Joseph B. Kruskal, "On the Shortest Spanning Subtree of a Graph and the Traveling Salesman Problem" — Proceedings of the AMS, 1956.
- Václav Chvátal, *Linear Programming*. W. H. Freeman, 1983.
- Lawrence Page, Sergey Brin, Rajeev Motwani, and Terry Winograd, "The PageRank Citation Ranking: Bringing Order to the Web" — Stanford Technical Report, 1999.
- Steven S. Skiena, *The Algorithm Design Manual*, 2nd Edition. Springer, 2008.
