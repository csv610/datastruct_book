# Trees and Binary Trees

## Introduction

Trees are the most important non-linear data structure in computing. They organize data hierarchically — file systems, parse trees, decision trees, and search structures all use trees.

A **tree** consists of **nodes** connected by **edges**. Each node has exactly one **parent** (except the **root**, which has none) and zero or more **children**. Nodes with no children are **leaves**.

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Terminology

```tikz
\tikzstyle{t}=[draw,circle,minimum size=0.6cm,inner sep=2pt,thick]
\node[t,fill=red!20] (A) at (2,0) {A};
\node[t] (B) at (0,-1.2) {B};
\node[t] (C) at (2,-1.2) {C};
\node[t] (D) at (4,-1.2) {D};
\node[t] (E) at (-1,-2.4) {E};
\node[t] (F) at (1,-2.4) {F};
\node[t] (G) at (4,-2.4) {G};
\node[t] (H) at (1,-3.6) {H};
\draw (A) -- (B) (A) -- (C) (A) -- (D);
\draw (B) -- (E) (B) -- (F);
\draw (D) -- (G);
\draw (F) -- (H);
\node[above] at (2,0.4) {\small root};
\node[left,red] at (2,0) {\tiny level 0};
\node[left] at (1.8,-1.2) {\tiny level 1};
\node[left] at (1.8,-2.4) {\tiny level 2};
\node[left] at (1.8,-3.6) {\tiny level 3};
\node[draw,rectangle,rounded corners,fill=green!10,below] at (2,-4.4) {\small Leaves: C, E, G, H};
```

- **Root:** A (no parent)
- **Parent/Child:** A is parent of B, C, D; B is child of A
- **Leaf:** C, E, G, H (nodes with no children)
- **Siblings:** B, C, D share parent A
- **Path:** sequence of connected nodes: A → B → F → H
- **Path length:** number of edges on a path (3 for A to H)
- **Depth of node:** path length from root (depth of H = 3)
- **Height of node:** longest path from node to a leaf (height of B = 2)
- **Height of tree:** height of root (height = 3)
- **Subtree:** any node and its descendants
- **Degree:** number of children (A has degree 3, H has degree 0)

## Binary Trees

A **binary tree** is a tree where each node has at most 2 children: **left** and **right**.

### Properties

1. Maximum nodes at level i: 2ⁱ
2. Maximum nodes in a tree of height h: 2ʰ⁺¹ - 1
3. Minimum height of a tree with n nodes: ⌈log₂(n+1)⌉ - 1
4. A **full binary tree** has every node with 0 or 2 children.
5. A **complete binary tree** is full except possibly the last level, which is filled left to right.
6. A **perfect binary tree** has all leaves at the same level.

**Internal path length:** sum of depths of all internal (non-leaf) nodes.  
**External path length:** sum of depths of all leaves.

```tikz
\tikzstyle{t}=[draw,circle,minimum size=0.55cm,inner sep=2pt,thick]
\tikzstyle{tn}=[t,fill=gray!15]

% Full binary tree
\node[above] at (0,0.3) {\textbf{Full:}};
\node[t] (a) at (0,-0.5) {A};
\node[t] (b) at (-0.8,-1.5) {B};
\node[t] (c) at (0.8,-1.5) {C};
\node[t] (d) at (-1.3,-2.5) {D};
\node[t] (e) at (-0.3,-2.5) {E};
\draw (a)--(b) (a)--(c) (b)--(d) (b)--(e);

% Complete binary tree
\node[above] at (4,0.3) {\textbf{Complete:}};
\node[t] (f) at (3.5,-0.5) {A};
\node[t] (g) at (2.7,-1.5) {B};
\node[t] (h) at (4.3,-1.5) {C};
\node[t] (i) at (2.2,-2.5) {D};
\node[t] (j) at (3.2,-2.5) {E};
\node[t] (k) at (4.2,-2.5) {F};
\draw (f)--(g) (f)--(h) (g)--(i) (g)--(j) (h)--(k);

% Perfect binary tree
\node[above] at (8,0.3) {\textbf{Perfect:}};
\node[t] (l) at (7.5,-0.5) {A};
\node[t] (m) at (6.7,-1.5) {B};
\node[t] (n) at (8.3,-1.5) {C};
\node[t] (o) at (6.2,-2.5) {D};
\node[t] (p) at (7.2,-2.5) {E};
\node[t] (q) at (8.2,-2.5) {F};
\node[t] (r) at (9.2,-2.5) {G};
\draw (l)--(m) (l)--(n) (m)--(o) (m)--(p) (n)--(q) (n)--(r);
```

### Binary Tree Node

```cpp
template <typename T>
struct binary_node {
    T data;
    std::unique_ptr<binary_node> left;
    std::unique_ptr<binary_node> right;

    explicit binary_node(const T& value)
        : data(value), left(nullptr), right(nullptr) {}
    explicit binary_node(T&& value)
        : data(std::move(value)), left(nullptr), right(nullptr) {}
};
```

### Binary Tree Class

```cpp
template <typename T>
class binary_tree {
public:
    using node_ptr = std::unique_ptr<binary_node<T>>;

    binary_tree() = default;

    // Construct from root
    explicit binary_tree(node_ptr root) : root_(std::move(root)) {}

    bool empty() const noexcept { return root_ == nullptr; }

    // Preorder traversal
    template <typename Visitor>
    void preorder(Visitor&& visit) const {
        preorder(root_.get(), visit);
    }

    // Inorder traversal
    template <typename Visitor>
    void inorder(Visitor&& visit) const {
        inorder(root_.get(), visit);
    }

    // Postorder traversal
    template <typename Visitor>
    void postorder(Visitor&& visit) const {
        postorder(root_.get(), visit);
    }

    // Level-order traversal (breadth-first)
    template <typename Visitor>
    void level_order(Visitor&& visit) const {
        if (!root_) return;
        std::queue<binary_node<T>*> q;
        q.push(root_.get());
        while (!q.empty()) {
            auto* node = q.front(); q.pop();
            visit(node->data);
            if (node->left) q.push(node->left.get());
            if (node->right) q.push(node->right.get());
        }
    }

    size_t height() const { return height(root_.get()); }
    size_t size() const { return size(root_.get()); }

private:
    static void preorder(binary_node<T>* node, auto& visit) {
        if (!node) return;
        visit(node->data);
        preorder(node->left.get(), visit);
        preorder(node->right.get(), visit);
    }

    static void inorder(binary_node<T>* node, auto& visit) {
        if (!node) return;
        inorder(node->left.get(), visit);
        visit(node->data);
        inorder(node->right.get(), visit);
    }

    static void postorder(binary_node<T>* node, auto& visit) {
        if (!node) return;
        postorder(node->left.get(), visit);
        postorder(node->right.get(), visit);
        visit(node->data);
    }

    static size_t height(binary_node<T>* node) {
        if (!node) return 0;
        return 1 + std::max(height(node->left.get()), height(node->right.get()));
    }

    static size_t size(binary_node<T>* node) {
        if (!node) return 0;
        return 1 + size(node->left.get()) + size(node->right.get());
    }

    node_ptr root_;
};
```

### Traversal Walkthrough

Tree:
```
        A
       / \
      B   C
     / \
    D   E
```

**Preorder (root → left → right):** A, B, D, E, C
**Inorder (left → root → right):** D, B, E, A, C
**Postorder (left → right → root):** D, E, B, C, A
**Level order (BFS):** A, B, C, D, E

### Iterative Traversals (No Recursion)

```cpp
template <typename Visitor>
void preorder_iterative(Visitor&& visit) const {
    if (!root_) return;
    std::stack<binary_node<T>*> s;
    s.push(root_.get());
    while (!s.empty()) {
        auto* node = s.top(); s.pop();
        visit(node->data);
        if (node->right) s.push(node->right.get());
        if (node->left) s.push(node->left.get());
    }
}

template <typename Visitor>
void inorder_iterative(Visitor&& visit) const {
    std::stack<binary_node<T>*> s;
    auto* current = root_.get();
    while (current || !s.empty()) {
        while (current) {
            s.push(current);
            current = current->left.get();
        }
        current = s.top(); s.pop();
        visit(current->data);
        current = current->right.get();
    }
}

template <typename Visitor>
void postorder_iterative(Visitor&& visit) const {
    if (!root_) return;
    std::stack<binary_node<T>*> s1, s2;
    s1.push(root_.get());
    while (!s1.empty()) {
        auto* node = s1.top(); s1.pop();
        s2.push(node);
        if (node->left) s1.push(node->left.get());
        if (node->right) s1.push(node->right.get());
    }
    while (!s2.empty()) {
        visit(s2.top()->data);
        s2.pop();
    }
}
```

## Array Representation of Binary Trees

A **complete binary tree** can be stored efficiently in an array:

```
Index:  0  1  2  3  4  5  6
Data:  [A, B, C, D, E, F, G]

For node at index i:
  left child:  2i + 1
  right child: 2i + 2
  parent:      (i - 1) / 2
```

This is the basis for heap storage (Chapter 10).

## Applications of Binary Trees

### Expression Trees

An expression tree represents an arithmetic expression: leaves are operands, internal nodes are operators.

```
Expression: (3 + 4) × 5

        [×]
       /   \
     [+]   [5]
    /   \
  [3]   [4]
```

```cpp
struct expr_node {
    std::string value;      // operator or operand
    std::unique_ptr<expr_node> left;
    std::unique_ptr<expr_node> right;
    
    bool is_operator() const {
        return value == "+" || value == "-" || value == "*" || value == "/";
    }
};

int evaluate(const expr_node* node) {
    if (!node->is_operator()) {
        return std::stoi(node->value);
    }
    int l = evaluate(node->left.get());
    int r = evaluate(node->right.get());
    if (node->value == "+") return l + r;
    if (node->value == "-") return l - r;
    if (node->value == "*") return l * r;
    return l / r;
}
```

### Morse Code Tree

The Morse code tree encodes letters as binary choices (dot = left, dash = right):

```
            start
           /     \
         E        T
        /  \     /  \
       I    A   N    M
      / \  / \  / \  / \
     S  U R  W D  K G  O
```

## Union-Find (Disjoint Set Union)

Union-Find tracks a partition of elements into disjoint sets. It supports:
- **find(x):** which set does x belong to?
- **union(x, y):** merge the sets containing x and y

### Tree Representation

Each set is represented as a tree. The root is the set's representative. Each node stores a pointer to its parent.

```cpp
class union_find {
public:
    union_find(size_t n) : parent_(n), rank_(n, 0) {
        for (size_t i = 0; i < n; ++i) parent_[i] = i;
    }

    // Find with path compression
    size_t find(size_t x) {
        if (parent_[x] != x) {
            parent_[x] = find(parent_[x]);  // compress
        }
        return parent_[x];
    }

    // Union by rank
    void union_sets(size_t x, size_t y) {
        size_t rx = find(x);
        size_t ry = find(y);
        if (rx == ry) return;

        if (rank_[rx] < rank_[ry]) {
            parent_[rx] = ry;
        } else if (rank_[rx] > rank_[ry]) {
            parent_[ry] = rx;
        } else {
            parent_[ry] = rx;
            ++rank_[rx];
        }
    }

    bool same_set(size_t x, size_t y) {
        return find(x) == find(y);
    }

private:
    std::vector<size_t> parent_;
    std::vector<size_t> rank_;
};
```

**Complexity:** m operations = O(m·α(n)), where α is the inverse Ackermann function — effectively constant for all practical n.

### Application: Kruskal's MST

Union-Find is essential for Kruskal's minimum spanning tree algorithm (Chapter 17).

### Application: Image Connected Components

Label connected regions in a binary image:

```cpp
std::vector<std::vector<int>> connected_components(
        const std::vector<std::vector<bool>>& image) {
    int rows = image.size();
    int cols = image[0].size();
    union_find uf(rows * cols);

    // First pass: union adjacent foreground pixels
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!image[r][c]) continue;
            int idx = r * cols + c;
            if (r > 0 && image[r-1][c])
                uf.union_sets(idx, (r-1) * cols + c);
            if (c > 0 && image[r][c-1])
                uf.union_sets(idx, r * cols + (c-1));
        }
    }

    // Second pass: assign labels
    std::unordered_map<size_t, int> label_map;
    int next_label = 0;
    std::vector<std::vector<int>> labels(rows, std::vector<int>(cols, 0));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!image[r][c]) continue;
            size_t root = uf.find(r * cols + c);
            auto it = label_map.find(root);
            if (it == label_map.end()) {
                label_map[root] = ++next_label;
                it = label_map.find(root);
            }
            labels[r][c] = it->second;
        }
    }
    return labels;
}
```

## Reconstructing Binary Trees from Traversals

Given inorder + preorder (or inorder + postorder), we can uniquely reconstruct a binary tree.

**Example:** Inorder: [D, B, E, A, C], Preorder: [A, B, D, E, C]

```
A is the root (first in preorder).
In inorder, everything left of A is left subtree, right of A is right subtree.
Left subtree inorder: [D, B, E], preorder: [B, D, E]
Right subtree inorder: [C], preorder: [C]

Recursively build:
  B is root of left subtree.
  Inorder left of B: [D] → left child
  Inorder right of B: [E] → right child
```

```cpp
std::unique_ptr<binary_node<char>> build_tree(
    std::span<const char> inorder,
    std::span<const char> preorder) {
    if (inorder.empty()) return nullptr;

    char root_val = preorder[0];
    auto root = std::make_unique<binary_node<char>>(root_val);

    // Find root in inorder
    size_t split = 0;
    while (split < inorder.size() && inorder[split] != root_val) ++split;

    root->left = build_tree(
        inorder.subspan(0, split),
        preorder.subspan(1, split));
    root->right = build_tree(
        inorder.subspan(split + 1),
        preorder.subspan(split + 1));

    return root;
}
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Dereferencing a null tree/root pointer | Segmentation fault on first traversal | Check for null before accessing node fields |
| Confusing preorder, inorder, and postorder | Wrong traversal order, incorrect algorithm result | Remember: root position defines the order |
| Not deleting child subtrees before root | Memory leak — only root is freed | Postorder delete: children first, then root |
| Stack overflow on deep trees (recursive traversal) | Crash on tall trees (e.g., n > 10^5) | Use iterative traversal with explicit stack |
| Forgetting that a complete binary tree of n nodes has height ⌊log₂ n⌋ | Off-by-one in level calculations | Verify with: 2^h ≤ n < 2^{h+1} |
| Threaded tree confusion — left vs right thread flags | Wrong traversal, infinite loop | Test both `hasLeftThread` and `hasRightThread` explicitly |

## Summary

1. **Trees** model hierarchical relationships. Binary trees limit each node to at most 2 children.
2. **Traversals** — preorder, inorder, postorder, level-order — each serve different purposes.
3. **Iterative traversal** replaces the call stack with an explicit stack for non-recursive processing.
4. **Union-Find** tracks disjoint sets with near-constant time operations via path compression and union by rank.
5. **Binary tree reconstruction** from two traversals is a classic divide-and-conquer technique.

## Exercises

### Drill

1. For the binary tree:
   ```
        A
       / \
      B   C
     / \   \
    D   E   F
   ```
   List the nodes in: (a) preorder, (b) inorder, (c) postorder, (d) level-order.

2. How many different binary trees can store the keys {1, 2, 3}? Draw them all.

3. What is the maximum height of a binary tree with n nodes? The minimum height?

4. For a complete binary tree of height h (root at height 0), what is the maximum number of nodes? The minimum? The number of leaves?

5. What is the result of the following operations on the union-find data structure with elements {0, 1, 2, 3, 4, 5}:
   `union(0, 2), union(1, 3), union(2, 4), find(0), union(1, 4), find(3), union(5, 0)`
   Show the parent array after each step (with path compression).

### Application

6. Write a function `is_symmetric` that checks whether a binary tree is a mirror of itself. Test on symmetric and non-symmetric trees.

7. Implement `lowest_common_ancestor(root, p, q)` for a binary tree. What is its time complexity?

8. Write a non-recursive (iterative) `postorder` traversal using a single stack. Trace it on the tree above.

9. Given a binary tree, compute its **diameter** (longest path between any two nodes). What is the optimal algorithm?

10. Implement Huffman coding: build a Huffman tree from character frequencies, generate coding tables, encode and decode a string. Test on "this is an example for huffman encoding".

11. Implement the **Morris traversal** — an in-order traversal that uses threading (modifying right pointers temporarily) to achieve O(1) extra space. Explain why the O(1) space claim holds.

12. Write a function that serializes a binary tree to a string and deserializes it back. Use a format that handles arbitrary key values (e.g., use a sentinel marker for null).

13. Implement `floor_value(root, key)` — the largest value in a binary search tree that is ≤ key. Implement `ceil_value(root, key)` analogously.

14. Implement **tree isomorphism**: given two binary trees, determine if they are isomorphic (same structure, possibly different values).

15. Use union-find to solve the **dynamic connectivity** problem: given n vertices and a sequence of `union` and `connected` operations, answer each connectivity query.

### Research

16. **(BST reconstruction)** In a binary search tree, reconstructing from preorder alone is possible. Explain why this is true and implement the reconstruction. Prove its correctness.

17. **(Tarjan's union-find analysis)** Read Robert Tarjan's analysis showing that union-find with union by rank and path compression has inverse Ackermann complexity. Why is this effectively constant for all practical inputs?

18. **(Persistent trees)** Research persistent binary trees — immutable structures where every update produces a new tree while preserving the old one. Implement a persistent BST. How does the space complexity compare with the mutable version?

19. **(Heaps vs. trees)** Prove that the number of different binary trees on n nodes is the Catalan number C_n = (2n)! / ((n+1)! n!). Verify this for n = 1, 2, 3, 4.

## References and Further Reading

- Donald E. Knuth, *The Art of Computer Programming*, Volume 1: Fundamental Algorithms. Section 2.3.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapter 12 (trees), Chapter 21 (union-find).
- Robert E. Tarjan, "Efficiency of a Good But Not Linear Set Union Algorithm" — JACM, 1975.
- Robert E. Tarjan and Jan van Leeuwen, "Worst-Case Analysis of Set Union Algorithms" — JACM, 1984.
- Bernard Chazelle, "A Minimum Spanning Tree Algorithm with Inverse-Ackermann Type Complexity" — JACM, 2000.
- David A. Huffman, "A Method for the Construction of Minimum-Redundancy Codes" — Proceedings of the IRE, 1952.
