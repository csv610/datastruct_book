# Binary Search Trees and Balanced Search Trees

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Binary Search Trees

A **binary search tree (BST)** is a binary tree where for every node:
- All keys in the **left subtree** are less than the node's key
- All keys in the **right subtree** are greater than the node's key

```
          [50]
        /      \
      [30]     [70]
     /   \     /  \
   [20]  [40] [60] [80]
```

### BST Implementation

```cpp
template <typename K, typename V>
class bst {
public:
    using node_ptr = std::unique_ptr<node>;

    bst() = default;

    void insert(const K& key, const V& value) {
        insert(root_, key, value);
    }

    std::optional<V> find(const K& key) const {
        auto* n = find(root_.get(), key);
        if (n) return n->value;
        return std::nullopt;
    }

    void erase(const K& key) {
        root_ = erase(std::move(root_), key);
    }

    void inorder(std::ostream& os) const {
        inorder(root_.get(), os);
    }

private:
    struct node {
        K key;
        V value;
        node_ptr left;
        node_ptr right;

        node(const K& k, const V& v) : key(k), value(v) {}
    };

    static void insert(node_ptr& n, const K& key, const V& value) {
        if (!n) {
            n = std::make_unique<node>(key, value);
            return;
        }
        if (key < n->key) insert(n->left, key, value);
        else if (key > n->key) insert(n->right, key, value);
        else n->value = value;  // update
    }

    static node* find(node* n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) return find(n->left.get(), key);
        if (key > n->key) return find(n->right.get(), key);
        return n;
    }

    static node_ptr erase(node_ptr n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = erase(std::move(n->left), key);
            return n;
        }
        if (key > n->key) {
            n->right = erase(std::move(n->right), key);
            return n;
        }

        // Found the node — handle the three cases
        if (!n->left) return std::move(n->right);
        if (!n->right) return std::move(n->left);

        // Two children: replace with inorder successor
        auto successor = find_min(std::move(n->right));
        successor->right = delete_min(std::move(n->right));
        successor->left = std::move(n->left);
        return successor;
    }

    static node_ptr find_min(node_ptr n) {
        auto* current = n.get();
        while (current && current->left) current = current->left.get();
        auto result = std::make_unique<node>(current->key, current->value);
        return result;
    }

    static node_ptr delete_min(node_ptr n) {
        if (!n->left) return std::move(n->right);
        n->left = delete_min(std::move(n->left));
        return n;
    }

    static void inorder(node* n, std::ostream& os) {
        if (!n) return;
        inorder(n->left.get(), os);
        os << n->key << " ";
        inorder(n->right.get(), os);
    }

    node_ptr root_;
};
```

### Complexity

| Operation | Average Case | Worst Case |
|-----------|-------------|------------|
| find | O(log n) | O(n) |
| insert | O(log n) | O(n) |
| erase | O(log n) | O(n) |

The worst case occurs when keys are inserted in sorted (or reverse sorted) order — the BST degenerates into a linked list. **Balanced trees** solve this problem.

## AVL Trees

An **AVL tree** (Adelson-Velsky and Landis, 1962) maintains the invariant: for every node, the heights of its left and right subtrees differ by at most 1.

**Balance factor:** height(left) - height(right) ∈ {-1, 0, 1}

### Rotations

When an insertion or deletion violates the balance invariant, we perform **rotations** to restore it.

**Right rotation (LL imbalance):**

```
    [z] (bf = 2)           [y]
    /            →         /  \
  [y] (bf = 1)          [x]   [z]
  /
[x]
```

**Left rotation (RR imbalance):**

```
[z] (bf = -2)               [y]
   \              →        /  \
   [y] (bf = -1)        [z]   [x]
     \
     [x]
```

**Left-right rotation (LR imbalance):**

```
    [z]               [z]            [x]
    /                 /             /  \
  [y]      →        [x]     →     [y]  [z]
    \               /
    [x]           [y]
```

**Right-left rotation (RL imbalance):** symmetric.

### AVL Implementation

```cpp
template <typename K, typename V>
class avl_tree {
    struct avl_node {
        K key;
        V value;
        std::unique_ptr<avl_node> left;
        std::unique_ptr<avl_node> right;
        int height = 1;

        avl_node(const K& k, const V& v) : key(k), value(v) {}
    };

    using node_ptr = std::unique_ptr<avl_node>;

    int height(const avl_node* n) const {
        return n ? n->height : 0;
    }

    int balance_factor(const avl_node* n) const {
        return height(n->left.get()) - height(n->right.get());
    }

    void update_height(avl_node* n) {
        n->height = 1 + std::max(height(n->left.get()), height(n->right.get()));
    }

    // Right rotation
    node_ptr rotate_right(node_ptr z) {
        auto y = std::move(z->left);
        z->left = std::move(y->right);
        update_height(z.get());
        y->right = std::move(z);
        update_height(y.get());
        return y;
    }

    // Left rotation
    node_ptr rotate_left(node_ptr z) {
        auto y = std::move(z->right);
        z->right = std::move(y->left);
        update_height(z.get());
        y->left = std::move(z);
        update_height(y.get());
        return y;
    }

    // Rebalance: apply rotations if needed
    node_ptr rebalance(node_ptr n) {
        if (!n) return nullptr;
        update_height(n.get());
        int bf = balance_factor(n.get());

        if (bf > 1) {
            // Left-heavy: check left child's balance
            if (balance_factor(n->left.get()) < 0) {
                n->left = rotate_left(std::move(n->left));
            }
            return rotate_right(std::move(n));
        }
        if (bf < -1) {
            // Right-heavy: check right child's balance
            if (balance_factor(n->right.get()) > 0) {
                n->right = rotate_right(std::move(n->right));
            }
            return rotate_left(std::move(n));
        }
        return n;
    }

public:
    void insert(const K& key, const V& value) {
        root_ = insert(std::move(root_), key, value);
    }

    void erase(const K& key) {
        root_ = erase(std::move(root_), key);
    }

    std::optional<V> find(const K& key) const {
        auto* n = find(root_.get(), key);
        return n ? std::optional<V>(n->value) : std::nullopt;
    }

private:
    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<avl_node>(key, value);
        if (key < n->key) n->left = insert(std::move(n->left), key, value);
        else if (key > n->key) n->right = insert(std::move(n->right), key, value);
        else n->value = value;  // update
        return rebalance(std::move(n));
    }

    node_ptr erase(node_ptr n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = erase(std::move(n->left), key);
        } else if (key > n->key) {
            n->right = erase(std::move(n->right), key);
        } else {
            if (!n->left) return std::move(n->right);
            if (!n->right) return std::move(n->left);
            // Two children: find inorder successor
            auto successor = find_min(std::move(n->right));
            successor->right = delete_min(std::move(n->right));
            successor->left = std::move(n->left);
            return rebalance(std::move(successor));
        }
        return rebalance(std::move(n));
    }

    static avl_node* find(avl_node* n, const K& key) {
        if (!n) return nullptr;
        if (key < n->key) return find(n->left.get(), key);
        if (key > n->key) return find(n->right.get(), key);
        return n;
    }

    node_ptr root_;
};
```

### Complexity

All operations: O(log n) worst case — guaranteed, not just expected.

## Red-Black Trees

Red-black trees are an alternative balancing scheme that powers `std::map` and `std::set`.

**Properties:**
1. Every node is red or black
2. The root is black
3. Leaves (null children) are black
4. Red nodes cannot have red children (no two consecutive reds)
5. Every path from root to leaf has the same number of black nodes

These properties guarantee that the longest path from root to leaf is at most 2× the shortest path, giving O(log n) height.

Red-black trees are more complex to implement than AVL trees but require fewer rotations during insertion (O(1) vs O(log n)). This makes them preferred in practice for `std::map`.

### RB Tree Implementation

The red-black tree below uses the same recursive approach as our AVL tree. After inserting a new red node, the recursion unrolls and checks for red-red violations at the grandparent level. The three fixup cases match the standard algorithm: recolor when the uncle is red, straight rotation when the red child is on the outside, and zig-zag rotation when it is on the inside.

```cpp
template <typename K, typename V, typename Compare = std::less<K>>
class rb_tree {
    enum color { RED, BLACK };

    struct rb_node {
        K key;
        V value;
        color col = RED;
        std::unique_ptr<rb_node> left;
        std::unique_ptr<rb_node> right;

        rb_node(const K& k, const V& v) : key(k), value(v) {}
    };

    using node_ptr = std::unique_ptr<rb_node>;
    Compare comp_;
    node_ptr root_;

    static bool is_red(const rb_node* n) { return n && n->col == RED; }

    node_ptr rotate_left(node_ptr x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        return y;
    }

    node_ptr rotate_right(node_ptr y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        return x;
    }

    // Fix red-red violations on the way back up.
    // n is the grandparent of the red-red pair.
    node_ptr fixup(node_ptr n) {
        if (!n) return nullptr;

        // Left subtree violation
        if (is_red(n->left) && (is_red(n->left->left) || is_red(n->left->right))) {
            if (is_red(n->right)) {
                // Case 1: uncle is red → recolor
                n->left->col = BLACK;
                n->right->col = BLACK;
                n->col = RED;
            } else {
                // Cases 2/3: uncle is black → rotate
                if (is_red(n->left->right)) {
                    // LR: zig-zag — rotate left at child first
                    n->left = rotate_left(std::move(n->left));
                }
                // LL: straight — rotate right at grandparent
                n = rotate_right(std::move(n));
                n->col = BLACK;
                n->right->col = RED;
            }
            return n;
        }

        // Right subtree violation (symmetric)
        if (is_red(n->right) && (is_red(n->right->left) || is_red(n->right->right))) {
            if (is_red(n->left)) {
                n->left->col = BLACK;
                n->right->col = BLACK;
                n->col = RED;
            } else {
                if (is_red(n->right->left)) {
                    n->right = rotate_right(std::move(n->right));
                }
                n = rotate_left(std::move(n));
                n->col = BLACK;
                n->left->col = RED;
            }
        }
        return n;
    }

    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<rb_node>(key, value);

        if (comp_(key, n->key))
            n->left = insert(std::move(n->left), key, value);
        else if (comp_(n->key, key))
            n->right = insert(std::move(n->right), key, value);
        else {
            n->value = value;
            return n;
        }
        return fixup(std::move(n));
    }

public:
    void insert(const K& key, const V& value) {
        root_ = insert(std::move(root_), key, value);
        if (root_) root_->col = BLACK;
    }

    std::optional<V> find(const K& key) const {
        auto* n = root_.get();
        while (n) {
            if (comp_(key, n->key))      n = n->left.get();
            else if (comp_(n->key, key)) n = n->right.get();
            else return n->value;
        }
        return std::nullopt;
    }
};
```

The algorithm mirrors the standard bottom-up fixup but expressed recursively:
1. **Case 1 (uncle is red):** recolor the parent, uncle, and grandparent. The grandparent becomes red, which may create a new red-red violation higher up — handled by the next recursive return.
2. **Case 2 (zig-zag):** the red child is an *inside* grandchild (LR or RL). A rotation at the child level straightens the chain into Case 3.
3. **Case 3 (straight):** the red child is an *outside* grandchild (LL or RR). A rotation at the grandparent followed by recoloring resolves the violation permanently.

Red-black trees require O(1) rotations per insertion (amortized), compared to O(log n) for AVL trees. This makes `std::map` and `std::set` prefer red-black trees in practice.

## B-Trees

B-trees generalize binary search trees to nodes with multiple keys and children. They are the foundation of database indexing.

A B-tree of order m has:
- Every node has at most m children
- Every node (except root) has at least ⌈m/2⌉ children
- A non-leaf node with k children has k-1 keys
- All leaves are at the same depth

**B+ Trees** store all keys in leaves (linked for sequential access) and use internal nodes as routing indexes. This is the standard for relational database indexes.

```cpp
// Simplified B+ tree internal node concept:
struct btree_node {
    std::vector<int> keys;           // separating keys
    std::vector<btree_node*> children; // child pointers
    bool is_leaf;
};
```

B-trees are covered more thoroughly in database textbooks; we mention them here for completeness and as a bridge to external-memory data structures.

## Treaps (Randomized BST)

A **treap** (tree + heap) combines a BST key with a random priority. The tree is a BST by key and a max-heap by priority. With high probability, the tree is balanced.

```cpp
template <typename K, typename V>
class treap {
    struct treap_node {
        K key;
        V value;
        int priority;
        std::unique_ptr<treap_node> left;
        std::unique_ptr<treap_node> right;

        treap_node(const K& k, const V& v)
            : key(k), value(v), priority(std::rand()) {}
    };

    node_ptr rotate_right(node_ptr p) {
        node_ptr q = std::move(p->left);
        p->left = std::move(q->right);
        q->right = std::move(p);
        return q;
    }

    node_ptr rotate_left(node_ptr p) {
        node_ptr q = std::move(p->right);
        p->right = std::move(q->left);
        q->left = std::move(p);
        return q;
    }

    node_ptr insert(node_ptr n, const K& key, const V& value) {
        if (!n) return std::make_unique<treap_node>(key, value);
        if (key < n->key) {
            n->left = insert(std::move(n->left), key, value);
            if (n->left->priority > n->priority)
                n = rotate_right(std::move(n));
        } else if (key > n->key) {
            n->right = insert(std::move(n->right), key, value);
            if (n->right->priority > n->priority)
                n = rotate_left(std::move(n));
        } else {
            n->value = value;
        }
        return n;
    }
};
```

**Expected complexity:** O(log n) for all operations. The randomization avoids worst-case inputs.

## Comparison

| Tree Type | Insert | Find | Erase | Memory | Use Case |
|-----------|--------|------|-------|--------|----------|
| BST (unbalanced) | O(n) worst | O(n) worst | O(n) worst | 2 ptrs | Educational only |
| AVL | O(log n) | O(log n) | O(log n) | 2 ptrs + height | Frequent lookups |
| Red-Black | O(log n) | O(log n) | O(log n) | 2 ptrs + color bit | General purpose |
| Treap | O(log n) exp | O(log n) exp | O(log n) exp | 2 ptrs + priority | Simple impl |
| B-Tree | O(log n) | O(log n) | O(log n) | variable | Disk-based |

## STL Connection: std::map and std::set

`std::map` is typically implemented as a red-black tree:

```cpp
std::map<std::string, int> m;
m["apple"] = 5;
m["banana"] = 3;

auto it = m.find("apple");
if (it != m.end()) {
    std::cout << it->first << ": " << it->second;
}

// Ordered iteration (guaranteed by the standard)
for (const auto& [key, value] : m) {
    std::cout << key << " " << value << '\n';
}

// Range queries
auto [lo, hi] = m.equal_range("b");
// Iterates all keys starting with "b"
```

`std::set` is a map without values (just keys).

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Inserting sorted keys into an unbalaced BST | Degenerates to O(n) — a linked list | Use AVL or red-black tree, or shuffle input |
| Forgetting to update heights after AVL rotation | Balance checks give wrong result, tree corrupts | Always recompute height after left/right rotation |
| Deleting a node with two children but not tracking parent | Orphaned subtrees, memory leak | Use successor/predecessor replacement with parent tracking |
| Confusing Red-Black property 5 (black height) | Tree violates red-black invariants | Verify black height is equal on all root-to-leaf paths |
| B-tree insertion overflow at root | Root split not handled, height never increases | Create new root with two children after root split |
| Not marking `std::set` iterators as invalid after erase | Iterator use-after-free | Erase returns the next iterator; use that |
| Using `std::map::operator[]` for lookups | Inserts default value if key missing, corrupts data | Use `map.find()` or `map.contains()` for read-only lookups |

## Summary

1. **BSTs** provide O(log n) operations on average but O(n) in the worst case.
2. **AVL trees** guarantee O(log n) by maintaining a height balance invariant.
3. **Red-black trees** are the de facto standard — they balance with fewer rotations.
4. **Treaps** use randomization to achieve O(log n) expected performance with simple code.
5. **B-trees** extend the concept to multi-way nodes for disk-based storage.
6. `std::map` and `std::set` are production red-black tree implementations.

## Exercises

### Drill

1. **Sequential keys.** Insert keys [1, 2, 3, ..., 100] into a plain BST and into a B-tree of order 5. Compare the heights. Why does the BST degenerate while the B-tree doesn't?

   *(In production, databases use B-trees specifically because they handle both random and sequential inserts gracefully — their fan-out guarantees O(log n) height regardless of insertion order.)*

2. **Tree vs hash table for small data.** A small map (under 50 entries) is used 10,000 times per second for lookups. An engineer switches from a balanced tree to a hash table for "O(1) lookups" but performance gets worse. Why? Think about hash computation cost and cache misses for small data.

3. **Missing step in deletion.** Delete a key from an AVL tree using inorder successor replacement. The tree is unbalanced afterward. What was likely forgotten? Which rotation restores balance?

4. **Unequal black height.** A red-black tree has some root-to-leaf paths with black height 12 and others with 13. Which property is broken? Why does this invalidate the O(log n) height guarantee?

5. **Prefix scan.** A tree index on `(status, date)` has status values 'A', 'B', 'C' and dates over a year. A query filters on status='A' and date in the last month. The database estimates it must scan ~33% of the tree. Roughly how many leaves will it touch?

### Application

6. **B-tree implementation.** Implement a B-tree of order 5 (max 4 keys, min 2 per node) supporting insert and search. Benchmark against `std::map` for 10^5 sequential and random insertions. Count the number of nodes visited per operation. Explain why a higher-order B-tree (order 100+) uses fewer cache misses than a binary tree.

7. **Prefix tree for routing.** A web server has 10,000 URL routes. Linear scanning on each request takes O(n). Build a trie (prefix tree) that stores routes by path segments. Compare lookup speed with `std::map` and linear scan:
   - Build time for 10^4 routes
   - Lookup time for 10^6 requests
   - Memory per route
   Report which structure wins for prefix matching vs exact matching.

8. **Concurrent tree.** Implement a thread-safe binary search tree using a mutex. Benchmark throughput with 1 reader, 4 readers, and 4 readers + 1 writer. Where is the bottleneck? Compare with a simple `std::shared_mutex` guarding `std::map`.

9. **Tree vs skip list.** Implement both an AVL tree and a skip list. Compare:
   - Insert 10^5 random elements
   - Range query: iterate over elements in [lo, hi]
   - Delete all elements
   Which data structure is simpler to implement? Which is faster for range queries?

   *(In production, Redis uses skip lists (not trees) for sorted sets because they are simpler to implement and offer better range-scan performance.)*

10. **Disk-based tree.** Simulate a disk-based B-tree where each node access costs 100× more than an in-memory access. Count the number of node accesses for 10^6 inserts into a B-tree (order 100) vs a BST. How does the fan-out of the B-tree reduce I/O?

### Research

11. **Bulk loading.** Build a B-tree by inserting one key at a time. Then build the same tree by sorting the keys first and constructing the tree bottom-up (layered). Compare:
    - Construction time for 10^6 keys
    - Tree height
    - Node occupancy percentage
    Explain why bulk loading produces a denser, shorter tree.

    *(In production, ClickHouse uses bulk loading for its primary index because columnar data arrives sorted by sort key.)*

12. **Write-optimized tree.** Research the Log-Structured Merge-tree (LSM-tree) design used by RocksDB. Implement a simplified version: writes go to an in-memory sorted buffer; when full, flush to disk as a sorted run. Compare write throughput with a B-tree for 10^5 inserts. When does the LSM-tree outperform the B-tree? When does it fall behind (reads)?

13. **Persistent (immutable) tree.** Implement a BST where every insert creates a new path of nodes (path copying), leaving the old tree intact. Compare:
    - Memory per insertion (how many new nodes?)
    - Throughput vs a mutable BST
    - Space after 10^5 operations
    Why is this useful for concurrent or versioned data structures?

14. **Batched updates.** Research a B-tree variant where each node has a buffer of pending updates. When the buffer overflows, updates propagate to children. Implement a simplified version. Compare write throughput with a standard B-tree. How large must the buffer be to match LSM-tree performance?

## References and Further Reading

- G. M. Adelson-Velsky and E. M. Landis, "An Algorithm for the Organization of Information" — Doklady Akademii Nauk SSSR, 1962.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Chapter 3 (BSTs, red-black trees).
- Rudolf Bayer and Edward McCreight, "Organization and Maintenance of Large Ordered Indices" — Acta Informatica, 1972.
- Cecilia R. Aragon and Raimund Seidel, "Randomized Search Trees" — FOCS 1989.
- Leonidas J. Guibas and Robert Sedgewick, "A Dichromatic Framework for Balanced Trees" — FOCS 1978.
- Douglas Comer, "Ubiquitous B-Tree" — ACM Computing Surveys, 1979.
- Philip L. Lehman and S. Bing Yao, "Efficient Locking for Concurrent Operations on B-Trees" — ACM TODS, 1981.
- Mark T. O'Connell, "Purely Functional Data Structures" (1996 PhD thesis) — Cambridge University Press, 1998.
