# Hashing and Hash Tables

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The Dictionary ADT

A **dictionary** (also called map or associative array) stores key-value pairs and supports:

- **insert(key, value)** — add a new key-value pair
- **find(key)** — return the value associated with key
- **erase(key)** — remove the key-value pair

Hash tables implement dictionaries with average O(1) time per operation — a dramatic improvement over the O(log n) of balanced trees.

## Hash Tables: Basic Idea

A **hash table** uses a **hash function** h(k) to map each key k to an index in an array of size m. The pair (k, v) is stored at position h(k) in the table.

```tikz
\tikzstyle{entry}=[draw,rectangle,minimum width=1.2cm,minimum height=0.7cm,inner sep=2pt]
\tikzstyle{nullbox}=[draw,rectangle,minimum width=0.5cm,minimum height=0.7cm,inner sep=2pt]

% Hash table array
\foreach \i/\lbl in {0/{\small 0},1/{\small 1},2/{\small 2},3/{\small 3},4/{\small 4},
                      5/{\small 5},6/{\small 6},7/{\small 7},8/{\small 8}} {
  \node[entry,fill=blue!10] (t\i) at (\i*1.4,0) {\lbl};
}

% Alice -> 7
\node[anchor=east] at (-1.5,0.8) {\texttt{"Alice"}};
\draw[->,>=stealth] (-0.3,0.8) -- (0.3,0.8) node[midway,above] {\tiny $h$=7};
\draw[->,>=stealth,dashed] (0.5,0.8) -- (7*1.4,0.5);

% Bob -> 3
\node[anchor=east] at (-1.5,0) {\texttt{"Bob"}};
\draw[->,>=stealth] (-0.3,0) -- (0.3,0) node[midway,above] {\tiny $h$=3};
\draw[->,>=stealth,dashed] (0.5,0) -- (3*1.4,0.3);

% Carol -> 7 (collision)
\node[anchor=east,text=red] at (-1.5,-0.8) {\texttt{"Carol"}};
\draw[->,>=stealth,red] (-0.3,-0.8) -- (0.3,-0.8) node[midway,above] {\tiny $h$=7};
\draw[->,>=stealth,dashed,red] (0.5,-0.8) -- (7*1.4,-0.3);
\node[red,below] at (7*1.4,-0.6) {\small\bfseries collision!};
```

When two keys hash to the same index, we have a **collision**. Collision resolution is the central challenge of hash table design.

## Hash Functions

### Properties of a Good Hash Function

1. **Deterministic** — same key always produces the same hash
2. **Uniform** — keys are distributed evenly across the table
3. **Fast** — O(1) to compute
4. **Non-invertible** — hard to reconstruct the key from the hash (not a security requirement for general hashing, but good practice)

### Integer Keys

For integer keys, the simplest approach: **h(k) = k mod m**.

m should be prime to reduce patterns. For example, m = 13:
```
h(10) = 10 mod 13 = 10
h(26) = 26 mod 13 = 0
h(39) = 39 mod 13 = 0  // collision with 26!
```

### String Keys

```cpp
// djb2 hash — simple but effective
size_t hash_string(std::string_view s) {
    size_t h = 5381;
    for (char c : s) {
        h = h * 33 + static_cast<size_t>(c);
    }
    return h;
}
```

### Rob Dixon's "sdbm" hash

```cpp
size_t hash_sdbm(std::string_view s) {
    size_t h = 0;
    for (char c : s) {
        h = c + (h << 6) + (h << 16) - h;
    }
    return h;
}
```

### std::hash

C++ provides `std::hash<T>` for common types:

```cpp
std::hash<int> int_hash;
size_t h = int_hash(42);

std::hash<std::string> str_hash;
size_t h2 = str_hash("hello");

// Custom hash for user types:
struct point {
    int x, y;
    bool operator==(const point& other) const {
        return x == other.x && y == other.y;
    }
};

template <>
struct std::hash<point> {
    size_t operator()(const point& p) const noexcept {
        size_t h1 = std::hash<int>{}(p.x);
        size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};
```

## Separate Chaining

Each table slot holds a linked list (or any container) of all key-value pairs that hash to that position.

```tikz
\tikzstyle{bucket}=[draw,rectangle,minimum width=0.8cm,minimum height=0.6cm,inner sep=2pt]
\tikzstyle{chainnode}=[draw,rectangle,minimum width=1.6cm,minimum height=0.6cm,inner sep=2pt]
\tikzstyle{nullbox}=[draw,rectangle,minimum width=0.4cm,minimum height=0.6cm,inner sep=2pt]

% Buckets
\node[bucket,fill=blue!10] (b0) at (0,0)      {\small\tt 0};
\node[bucket,fill=blue!10] (b1) at (0,-0.9)   {\small\tt 1};
\node[bucket,fill=blue!10] (b2) at (0,-1.8)   {\small\tt 2};
\node[bucket,fill=blue!10] (b3) at (0,-2.7)   {\small\tt 3};
\node at (0,-3.6) {$\vdots$};

% Chain entries
\node[chainnode] (c0) at (2.8,0)    {\small\tt(26,\ v26)};
\draw[->,>=stealth] (b0.east) -- (c0.west);
\node[chainnode] (c0b) at (5.4,0)   {\small\tt(39,\ v39)};
\draw[->,>=stealth] (c0.east) -- (c0b.west);
\node[nullbox] (c0c) at (7.8,0)     {$\varnothing$};
\draw[->,>=stealth] (c0b.east) -- (c0c.west);

\node[nullbox] (c1) at (2.8,-0.9)  {$\varnothing$};
\draw[->,>=stealth] (b1.east) -- (c1.west);

\node[chainnode] (c2) at (2.8,-1.8) {\small\tt(2,\ v2)};
\draw[->,>=stealth] (b2.east) -- (c2.west);
\node[nullbox] (c2b) at (5.0,-1.8) {$\varnothing$};
\draw[->,>=stealth] (c2.east) -- (c2b.west);

\node[chainnode] (c3) at (2.8,-2.7) {\small\tt(3,\ v3)};
\draw[->,>=stealth] (b3.east) -- (c3.west);
\node[chainnode] (c3b) at (5.4,-2.7) {\small\tt(16,\ v16)};
\draw[->,>=stealth] (c3.east) -- (c3b.west);
\node[nullbox] (c3c) at (7.8,-2.7) {$\varnothing$};
\draw[->,>=stealth] (c3b.east) -- (c3c.west);
```

```cpp
template <typename K, typename V, typename Hash = std::hash<K>>
class hash_table_chaining {
public:
    explicit hash_table_chaining(size_t initial_buckets = 16)
        : buckets_(initial_buckets), table_(initial_buckets) {}

    void insert(const K& key, const V& value) {
        auto& chain = table_[bucket(key)];
        for (auto& [k, v] : chain) {
            if (k == key) {
                v = value;  // update existing
                return;
            }
        }
        chain.push_back({key, value});
        ++size_;
        check_load();
    }

    std::optional<V> find(const K& key) const {
        const auto& chain = table_[bucket(key)];
        for (const auto& [k, v] : chain) {
            if (k == key) return v;
        }
        return std::nullopt;
    }

    void erase(const K& key) {
        auto& chain = table_[bucket(key)];
        auto it = std::find_if(chain.begin(), chain.end(),
            [&](const auto& p) { return p.first == key; });
        if (it != chain.end()) {
            chain.erase(it);
            --size_;
        }
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

private:
    size_t bucket(const K& key) const {
        return Hash{}(key) % buckets_;
    }

    void check_load() {
        // Rehash when load factor exceeds 1.0
        if (size_ > buckets_) {
            buckets_ *= 2;
            std::vector<std::list<std::pair<K, V>>> new_table(buckets_);
            for (const auto& chain : table_) {
                for (const auto& pair : chain) {
                    size_t b = Hash{}(pair.first) % buckets_;
                    new_table[b].push_back(pair);
                }
            }
            table_ = std::move(new_table);
        }
    }

    size_t buckets_;
    size_t size_ = 0;
    std::vector<std::list<std::pair<K, V>>> table_;
};
```

**Load factor:** α = n/m (number of elements / number of buckets). With separate chaining, α can exceed 1. Average chain length = α.

## Open Addressing

All elements are stored in the table array itself. On collision, we probe for the next empty slot.

### Linear Probing

If slot h(k) is occupied, try h(k) + 1, h(k) + 2, ..., wrapping around.

```tikz
\tikzstyle{slot}=[draw,rectangle,minimum width=1cm,minimum height=0.7cm,inner sep=2pt]

% Step 1: insert 26
\node[above] at (1.5,0.5) {\textbf{Insert 26:}};
\node[below] at (1.5,0.1) {\small$h(26)=0$};
\foreach \i/\lbl/\clr in {0/26/blue!15,1//gray!10,2//gray!10,3//gray!10} {
  \node[slot,fill=\clr] (s\i) at (\i*1.2,-0.5) {\small\tt\lbl};
  \node[above] at (\i*1.2,-0.5) {\tiny\i};
}
\draw[->,>=stealth,blue] (-1.2,-0.2) -- (0,-0.2);
\node[below,blue] at (0,-1.0) {\small empty $\rightarrow$ place};

% Step 2: insert 39
\node[above] at (5.5,0.5) {\textbf{Insert 39:}};
\node[below] at (5.5,0.1) {\small$h(39)=0$};
\foreach \i/\lbl/\clr in {0/26/blue!15,1/39/blue!15,2//gray!10,3//gray!10} {
  \node[slot,fill=\clr] (t\i) at (5+\i*1.2,-0.5) {\small\tt\lbl};
  \node[above] at (5+\i*1.2,-0.5) {\tiny\i};
}
\draw[->,>=stealth,red] (3.8,-0.2) -- (5,-0.2);
\node[below,red] at (5,-1.0) {\small occupied $\rightarrow$ try 1};
\draw[->,>=stealth,blue] (5.8,-0.2) -- (6,-0.2);
\node[below,blue] at (6.2,-1.0) {\small empty $\rightarrow$ place};

% Step 3: insert 13
\node[above] at (10,0.5) {\textbf{Insert 13:}};
\node[below] at (10,0.1) {\small$h(13)=0$};
\foreach \i/\lbl/\clr in {0/26/blue!15,1/39/blue!15,2/13/blue!15,3//gray!10} {
  \node[slot,fill=\clr] (u\i) at (10+\i*1.2,-0.5) {\small\tt\lbl};
  \node[above] at (10+\i*1.2,-0.5) {\tiny\i};
}
\draw[->,>=stealth,red] (8.8,-0.2) -- (10,-0.2);
\draw[->,>=stealth,red] (10.8,-0.2) -- (11,-0.2);
\draw[->,>=stealth,blue] (11.8,-0.2) -- (12,-0.2);
\node[below,blue] at (12.2,-1.0) {\small place};
```

**Problem: primary clustering.** A block of consecutive occupied slots grows, attracting more collisions.

### Quadratic Probing

Try positions: h(k), h(k) + 1², h(k) + 2², h(k) + 3², ...

```
Probe sequence: p(i) = (h(k) + i²) mod m
```

Reduces clustering but may miss slots even when the table is not full.

### Double Hashing

Use a second hash function for the probe step:

```
Probe sequence: p(i) = (h(k) + i·h₂(k)) mod m
```

This gives the best distribution of the three open addressing methods.

```cpp
template <typename K, typename V, typename Hash = std::hash<K>>
class hash_table_open {
public:
    explicit hash_table_open(size_t initial_capacity = 16)
        : capacity_(initial_capacity), size_(0),
          states_(initial_capacity, State::empty),
          keys_(initial_capacity), values_(initial_capacity) {}

    bool insert(const K& key, const V& value) {
        if (size_ >= capacity_ * 0.7) rehash();
        size_t i = 0;
        size_t pos;
        do {
            pos = probe(key, i);
            if (states_[pos] == State::empty || states_[pos] == State::deleted) {
                keys_[pos] = key;
                values_[pos] = value;
                states_[pos] = State::occupied;
                ++size_;
                return true;
            }
            if (states_[pos] == State::occupied && keys_[pos] == key) {
                values_[pos] = value;  // update
                return false;
            }
            ++i;
        } while (i < capacity_);
        rehash();
        return insert(key, value);
    }

    std::optional<V> find(const K& key) const {
        size_t i = 0;
        size_t pos;
        do {
            pos = probe(key, i);
            if (states_[pos] == State::empty) return std::nullopt;
            if (states_[pos] == State::occupied && keys_[pos] == key) {
                return values_[pos];
            }
            ++i;
        } while (i < capacity_);
        return std::nullopt;
    }

    void erase(const K& key) {
        size_t i = 0;
        size_t pos;
        do {
            pos = probe(key, i);
            if (states_[pos] == State::empty) return;
            if (states_[pos] == State::occupied && keys_[pos] == key) {
                states_[pos] = State::deleted;
                --size_;
                return;
            }
            ++i;
        } while (i < capacity_);
    }

private:
    enum class State { empty, occupied, deleted };

    size_t probe(const K& key, size_t i) const {
        size_t h1 = Hash{}(key) % capacity_;
        size_t h2 = 1 + (Hash{}(key) % (capacity_ - 1));  // second hash
        return (h1 + i * h2) % capacity_;
    }

    void rehash() {
        size_t old_cap = capacity_;
        capacity_ *= 2;
        std::vector<State> old_states = std::move(states_);
        std::vector<K> old_keys = std::move(keys_);
        std::vector<V> old_values = std::move(values_);

        states_ = std::vector<State>(capacity_, State::empty);
        keys_.resize(capacity_);
        values_.resize(capacity_);
        size_ = 0;

        for (size_t i = 0; i < old_cap; ++i) {
            if (old_states[i] == State::occupied) {
                insert(old_keys[i], old_values[i]);
            }
        }
    }

    size_t capacity_, size_;
    std::vector<State> states_;
    std::vector<K> keys_;
    std::vector<V> values_;
};
```

## Skip Lists

A **skip list** is a probabilistic data structure that maintains an ordered set with O(log n) expected time for search, insert, and erase. It consists of multiple linked levels, where each higher level acts as an "express lane" over the level below.

### Structure

A skip list is a hierarchy of sorted linked lists:

```
level 3:  1 ---------------------------→ 9
level 2:  1 -----------→ 5 -----------→ 9
level 1:  1 ---→ 3 ---→ 5 ---→ 7 ---→ 9
level 0:  1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 9 → ∅
```

Each element appears in level 0. It appears in higher levels with probability 1/2 per level (i.e., level 1: 1/2, level 2: 1/4, level 3: 1/8, etc.). The maximum level is typically capped at ⌈log₂ n⌉.

### Node Structure

```cpp
template <typename K, typename V>
struct skip_node {
    K key;
    V value;
    std::vector<skip_node*> next;  // next[i] = successor at level i

    skip_node(const K& k, const V& v, int level)
        : key(k), value(v), next(level + 1, nullptr) {}
};
```

### Search

Search proceeds from the topmost level, moving forward as far as possible at each level before descending:

```cpp
template <typename K, typename V>
V* skip_list<K, V>::find(const K& key) {
    auto* cur = head_;
    for (int i = level_; i >= 0; --i) {
        while (cur->next[i] && cur->next[i]->key < key)
            cur = cur->next[i];
    }
    cur = cur->next[0];
    if (cur && cur->key == key) return &cur->value;
    return nullptr;
}
```

### Insertion

To insert, we search through all levels and record the nodes where we need to insert at each level. We then choose a random level for the new node and link it in:

```cpp
template <typename K, typename V>
void skip_list<K, V>::insert(const K& key, const V& value) {
    std::vector<skip_node<K, V>*> update(level_ + 1, nullptr);
    auto* cur = head_;

    for (int i = level_; i >= 0; --i) {
        while (cur->next[i] && cur->next[i]->key < key)
            cur = cur->next[i];
        update[i] = cur;
    }

    cur = cur->next[0];
    if (cur && cur->key == key) {
        cur->value = value;  // key already exists
        return;
    }

    int new_level = random_level();
    if (new_level > level_) {
        head_->next.resize(new_level + 1, nullptr);
        update.resize(new_level + 1, head_);
        level_ = new_level;
    }

    auto* node = new skip_node<K, V>(key, value, new_level);
    for (int i = 0; i <= new_level; ++i) {
        node->next[i] = update[i]->next[i];
        update[i]->next[i] = node;
    }
}
```

### Choosing the Level

```cpp
int random_level() {
    int level = 0;
    while (rand() < RAND_MAX / 2 && level < MAX_LEVEL)
        ++level;
    return level;
}
```

This produces level 0 with probability 1/2, level 1 with probability 1/4, level 2 with probability 1/8, etc.

### Complexity

| Operation | Expected | Worst Case |
|-----------|----------|------------|
| Search | O(log n) | O(n) |
| Insert | O(log n) | O(n) |
| Erase | O(log n) | O(n) |
| Space | O(n) | O(n log n) |

The worst case occurs when all elements get the same level (e.g., all level 0). The probability of this is vanishingly small: (1/2)ⁿ for n elements all at level 0.

### Skip Lists vs. Hash Tables vs. Balanced Trees

| Criteria | Hash Table | Skip List | Balanced Tree |
|----------|------------|-----------|---------------|
| Average search | O(1) | O(log n) | O(log n) |
| Ordered iteration | No | O(n log n) | O(n) in-order |
| Range queries | No | O(log n + k) | O(log n + k) |
| Implementation | Medium | Medium | Complex |
| Memory | O(n + m) | O(n) expected | O(n) |

Skip lists occupy a useful middle ground: they support ordered operations like balanced trees but are much simpler to implement. They are not a replacement for hash tables (which are faster for unordered lookups), but they are competitive with balanced trees for ordered maps.

### STL Connection

The C++ standard library does not include a skip list implementation. However, the `leveldb` and `rocksdb` databases use skip lists as their default in-memory table implementation, and Redis uses skip lists for sorted sets.

## Modern Hash Table Designs

### Robin Hood Hashing

When probing, if the current occupant is "richer" (closer to its ideal position) than the incoming element, swap them. This equalizes probe distances.

```
Insert 39: h(39)=0 → occupied by 26 (probe distance 0)
           Compare: 39 has distance 0, 26 has distance 0 → keep going
           Try 1 → empty → insert 39 (distance 1)

Insert 13: h(13)=0 → 26 (dist 0) → compare equal → keep going
           1 → 39 (dist 1) → compare equal → keep going
           2 → empty → insert (dist 2)

Insert 1:  h(1)=1 → 39 (dist 1) → 1 has dist 0 < 39's dist 1 → keep going
           2 → 13 (dist 2) → 1 has dist 1 < 13's dist 2 → keep going
           3 → empty → insert (dist 3)
```

### Cuckoo Hashing

Use two hash tables with two different hash functions. On collision, evict the existing element and insert it into the other table. If this cycles, rehash.

### Swiss Table (Google's Abseil flat_hash_map)

Uses SIMD (SSE2/ARM NEON) to probe 16 slots simultaneously. Each slot has a 1-byte metadata field (7 bits of hash + 1 bit for occupancy). The lookup checks all 16 metadata bytes in one CPU instruction.

```cpp
// Abseil flat_hash_map — the fastest general-purpose hash table
#include <absl/container/flat_hash_map.h>

absl::flat_hash_map<std::string, int> map;
map["hello"] = 1;
map["world"] = 2;
```

## Complexity Analysis

| Operation | Separate Chaining | Open Addressing (double hash) |
|-----------|-------------------|------------------------------|
| average insert | O(1) | O(1) |
| average find | O(1) | O(1) |
| average erase | O(1) | O(1) |
| worst-case find | O(n) | O(n) |
| space overhead | O(n + m) | O(m) |

The worst case occurs when all keys hash to the same bucket. With a good hash function and proper resizing, this is exponentially unlikely.

## Comparison of Dictionaries

| Criteria | Hash Table | Balanced Tree (std::map) |
|----------|------------|--------------------------|
| Average search | O(1) | O(log n) |
| Worst-case search | O(n) | O(log n) |
| Ordered iteration | No | Yes (in-order) |
| Range queries | No | Yes |
| Memory | More (load factor < 1) | Less (exactly n nodes) |
| Iterator validity | Invalidated on rehash | Stable |

**When to use which:**
- Hash table: unordered map, need O(1) average lookup
- Tree map: need ordered iteration, range queries, or worst-case guarantees

## Application: LZW Compression

LZW (Lempel-Ziv-Welch) compression builds a dictionary of phrases during compression:

```cpp
std::vector<int> lzw_compress(std::string_view input) {
    // Initialize dictionary with single characters
    hash_table_chaining<std::string, int> dict;
    for (int i = 0; i < 256; ++i) {
        dict.insert(std::string(1, static_cast<char>(i)), i);
    }

    std::vector<int> output;
    std::string w;
    int next_code = 256;

    for (char c : input) {
        std::string wc = w + c;
        if (dict.find(wc).has_value()) {
            w = wc;
        } else {
            output.push_back(*dict.find(w));
            dict.insert(wc, next_code++);
            w = std::string(1, c);
        }
    }
    if (!w.empty()) output.push_back(*dict.find(w));
    return output;
}
```

**Trace:** compress `"ABABABA"`

| Step | w | c | wc | in dict? | Output | New dict entry |
|------|---|---|----|----------|--------|----------------|
| 1 | — | A | A | yes | — | — |
| 2 | A | B | AB | no | 65 (A) | AB → 256 |
| 3 | B | A | BA | no | 66 (B) | BA → 257 |
| 4 | A | B | AB | yes (256) | — | — |
| 5 | AB | A | ABA | no | 256 (AB) | ABA → 258 |
| 6 | A | B | AB | yes (256) | — | — |
| 7 | AB | A | ABA | yes (258) | — | — |
| 8 | ABA | — | — | — | 258 (ABA) | — |

Output: `[65, 66, 256, 258]` — 4 codes vs 7 input characters.

## STL Connection: std::unordered_map

```cpp
std::unordered_map<std::string, int> map;
map["apple"] = 5;
map["banana"] = 3;

auto it = map.find("apple");
if (it != map.end()) {
    std::cout << it->second;  // 5
}

// Iterate (order is unspecified)
for (const auto& [key, value] : map) {
    std::cout << key << ": " << value << '\n';
}
```

**Implementation notes:**
- Uses separate chaining with linked lists
- Rehashes when load factor exceeds `max_load_factor()` (default 1.0)
- Buckets are grouped into linked lists; iterators remain valid after insert (but not after rehash)
- `std::unordered_set` is the key-only version

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Using a poor hash function (e.g., identity for integers) | Clustering, O(n) lookups instead of O(1) | Use `std::hash` or a well-mixing function |
| Forgetting to rehash when load factor exceeds threshold | Severe performance degradation | Monitor load factor; rehash when α > threshold |
| Using mutable objects as hash keys | Object in wrong bucket after mutation, unfindable | Use immutable keys, or rehash after mutation |
| Hashing floating-point NaN | NaN != NaN, key becomes unfindable | Avoid float keys; or use a canonical NaN representation |
| Iterator invalidation after rehash in `std::unordered_map` | Use-after-free on next increment | Re-acquire iterators after any insert that may rehash |
| Using `operator[]` for lookups | Inserts default value if key missing | Use `find()` or `contains()` for read-only access |

## Summary

1. **Hash functions** map keys to table indices. Good hash functions distribute keys uniformly.
2. **Separate chaining** uses linked lists per bucket. Simple and robust. α can exceed 1.
3. **Open addressing** stores all elements in the table array. Double hashing gives the best probe sequence.
4. **Rehashing** maintains the load factor below a threshold, preserving O(1) average operations.
5. **Modern designs** (Robin Hood, Cuckoo, Swiss Table) improve cache performance and worst-case behavior.
6. **std::unordered_map** is the production implementation.

## Exercises

### Drill

1. For a hash table with 11 buckets and the hash function h(k) = k mod 11, insert the keys [22, 1, 13, 11, 24, 33, 18, 42, 15] using:
   a) Separate chaining
   b) Linear probing
   c) Double hashing with h₂(k) = 7 - (k mod 7)

   Show the state of the table after each insertion.

2. What is the load factor after inserting the keys in exercise 1? What is the average number of probes per successful search for the linear probing version?

3. For a hash table of size m = 1000 with double hashing, how many probes are needed in the worst case to find an empty slot when α = 0.75? When α = 0.95?

4. Which of the following make good hash functions for strings? For each, explain why or why not:
   a) Sum of ASCII values
   b) First character's ASCII value
   c) Length of the string
   d) djb2: h = h × 33 + cᵢ for each character

5. Why is a "universal hash function" preferable to a fixed hash function? Give an example scenario where a fixed hash function fails.

### Application

6. Implement a `hash_table_chaining` class with automatic rehashing when the load factor exceeds a configurable threshold. Compare its performance with `std::unordered_map` for 10⁶ insertions and 10⁶ lookups of random 64-bit integers.

7. Implement Robin Hood hashing with linear probing. After each insertion, swap the inserted element forward if it is farther from its ideal bucket than existing elements. Measure the maximum probe distance at α = 0.8 and compare with standard linear probing.

8. Implement a hash function for `std::pair<int, int>` that produces good distribution. Test it by inserting 10⁵ distinct pairs and measuring the maximum bucket size.

9. Implement LZW compression and decompression using a hash table for the dictionary. Verify that compress followed by decompress reproduces the original string. Test on "ABABABABA" and a short text file.

10. Design and implement a hash function for IPv4 addresses. What properties does it need? Test it on a dataset of real IP addresses (you can generate a synthetic dataset with common prefixes).

11. Implement Cuckoo hashing with two hash tables and the cycle-detection rehash strategy. Compare its worst-case lookup time with linear probing.

12. Implement a **Bloom filter** (see also Chapter 19) using two hash functions. Compare its false positive rate with the theoretical prediction for varying m/n ratios.

13. Write a spell checker that uses a hash table of dictionary words. Test it on a document and report the misspelled words found. Measure the lookup time per word.

### Research

14. **(Iterator invalidation)** Why does C++ mandate that `std::unordered_map` iterators remain valid after insert (until rehash)? How does this constraint affect implementation choices? Read the standard committee discussions.

15. **(Swiss Table)** Google's Abseil library implements a "Swiss Table" (or "raw hash table") that uses SIMD instructions for fast probing. Read the design document. Implement a simplified version that processes 4 buckets at a time using bit operations.

16. **(Perfect hashing)** Research FKS perfect hashing (Fredman, Komlós, Szemerédi 1984). Implement a static perfect hash table that supports O(1) worst-case lookups for a predetermined set of keys.

17. **(Hash flooding)** Read about hash flooding attacks against `std::unordered_map`. How did C++17's addition of `std::hash<std::filesystem::path>` and the adoption of randomized hashing mitigate this? Implement a hash table that is immune to simple collision attacks.

## References and Further Reading

- Donald E. Knuth, *The Art of Computer Programming*, Volume 3: Sorting and Searching. Section 6.4.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Chapter 11.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Section 3.4.
- Pedro Celis, "Robin Hood Hashing" — PhD thesis, University of Waterloo, 1986.
- Rasmus Pagh and Flemming Friche Rodler, "Cuckoo Hashing" — ESA 2001.
- Michael L. Fredman, János Komlós, and Endre Szemerédi, "Storing a Sparse Table with O(1) Worst Case Access Time" — JACM, 1984.
- Abseil C++ library, "Swiss Table" design document — https://abseil.io/docs/cpp/guides/container
- Go programming language, "Swiss Table" — https://go.dev/blog/swisstable
- J. T. Robinson, "The K-D-B-Tree: A Search Structure for Large Multidimensional Dynamic Indexes" — SIGMOD 1981.
