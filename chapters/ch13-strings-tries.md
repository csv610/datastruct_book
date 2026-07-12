# String Algorithms and Tries

String processing is one of the most common tasks in computing. This chapter covers the fundamental data structures and algorithms for efficient string operations.

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## Tries

A **trie** (from retrieval, pronounced "try") is a tree where each node represents a prefix. The root is the empty string, and each edge is labeled with a character. Tries support O(m) lookup, insertion, and deletion, where m is the key length — independent of the number of keys.

### Standard Trie

```
Keys: "cat", "car", "dog", "do"

         root
       /      \
      c        d
     /        / \
    a        o   *
   / \      / \
  t   r    g   *
  *   *    *
```

### Trie Implementation

```cpp
template <typename V>
class trie {
public:
    void insert(std::string_view key, const V& value) {
        node* n = &root_;
        for (char c : key) {
            size_t idx = c - 'a';  // assume lowercase letters
            if (!n->children[idx]) {
                n->children[idx] = std::make_unique<node>();
            }
            n = n->children[idx].get();
        }
        n->value = value;
        n->is_end = true;
    }

    std::optional<V> find(std::string_view key) const {
        const node* n = &root_;
        for (char c : key) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return std::nullopt;
            n = n->children[idx].get();
        }
        if (n->is_end) return n->value;
        return std::nullopt;
    }

    bool starts_with(std::string_view prefix) const {
        const node* n = &root_;
        for (char c : prefix) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return false;
            n = n->children[idx].get();
        }
        return true;
    }

    std::vector<std::string> keys_with_prefix(std::string_view prefix) const {
        std::vector<std::string> result;
        const node* n = &root_;
        for (char c : prefix) {
            size_t idx = c - 'a';
            if (!n->children[idx]) return result;
            n = n->children[idx].get();
        }
        collect_keys(n, std::string(prefix), result);
        return result;
    }

private:
    static constexpr size_t alphabet_size = 26;

    struct node {
        std::array<std::unique_ptr<node>, alphabet_size> children;
        V value;
        bool is_end = false;
    };

    void collect_keys(const node* n, std::string prefix,
                      std::vector<std::string>& result) const {
        if (n->is_end) result.push_back(prefix);
        for (size_t i = 0; i < alphabet_size; ++i) {
            if (n->children[i]) {
                collect_keys(n->children[i].get(),
                             prefix + char('a' + i), result);
            }
        }
    }

    node root_;
};
```

### Compressed Trie (Radix Tree)

A compressed trie merges chains of single-child nodes into single edges labeled with strings. This reduces the number of nodes from O(total characters) to O(number of keys).

```
Standard:  root → a → p → p → l → e → *
                                     ↓
                                    s → *
                                    
Compressed: root → "appl" → "e" → *
                             ↓
                            "s" → *
```

### Ternary Search Trie (TST)

A TST is a hybrid between a binary search tree and a trie. Each node has three children: `left` (char < current char), `mid` (char == current char), `right` (char > current char).

```cpp
template <typename V>
class tst {
public:
    void insert(std::string_view key, const V& value) {
        root_ = insert(root_, key, 0, value);
    }

    std::optional<V> find(std::string_view key) const {
        auto* n = find(root_.get(), key, 0);
        if (n && n->is_end) return n->value;
        return std::nullopt;
    }

private:
    struct node {
        char c;
        std::unique_ptr<node> left, mid, right;
        V value;
        bool is_end = false;
        node(char ch) : c(ch) {}
    };

    std::unique_ptr<node> insert(std::unique_ptr<node> n,
                                  std::string_view key, size_t i,
                                  const V& value) {
        char c = key[i];
        if (!n) n = std::make_unique<node>(c);
        if (c < n->c) n->left = insert(std::move(n->left), key, i, value);
        else if (c > n->c) n->right = insert(std::move(n->right), key, i, value);
        else if (i + 1 < key.size()) n->mid = insert(std::move(n->mid), key, i+1, value);
        else { n->value = value; n->is_end = true; }
        return n;
    }

    node* find(node* n, std::string_view key, size_t i) const {
        if (!n) return nullptr;
        char c = key[i];
        if (c < n->c) return find(n->left.get(), key, i);
        if (c > n->c) return find(n->right.get(), key, i);
        if (i + 1 < key.size()) return find(n->mid.get(), key, i+1);
        return n;
    }

    std::unique_ptr<node> root_;
};
```

**TST advantage:** Uses O(3n) space instead of O(n·alphabet_size), making it practical for Unicode strings.

### Application: Autocomplete

```cpp
class autocomplete {
public:
    void add_word(std::string_view word, int frequency) {
        trie_.insert(word, frequency);
    }

    std::vector<std::string> suggest(std::string_view prefix, int max_suggestions = 5) {
        auto candidates = trie_.keys_with_prefix(prefix);
        std::sort(candidates.begin(), candidates.end(),
            [&](const std::string& a, const std::string& b) {
                return trie_.find(a).value_or(0) > trie_.find(b).value_or(0);
            });
        if (candidates.size() > max_suggestions)
            candidates.resize(max_suggestions);
        return candidates;
    }

private:
    trie<int> trie_;
};
```

## String Matching

### Naive Matching

```cpp
std::vector<size_t> naive_match(std::string_view text, std::string_view pattern) {
    std::vector<size_t> matches;
    for (size_t i = 0; i <= text.size() - pattern.size(); ++i) {
        size_t j = 0;
        while (j < pattern.size() && text[i + j] == pattern[j]) ++j;
        if (j == pattern.size()) matches.push_back(i);
    }
    return matches;
}
```

**Complexity:** O(n·m) worst case, where n = text length, m = pattern length.

### Knuth-Morris-Pratt (KMP)

KMP preprocesses the pattern to build a **failure function** (also called the prefix function) that allows skipping ahead when a mismatch occurs.

```cpp
// Compute the prefix function (failure function)
std::vector<size_t> compute_prefix(std::string_view pattern) {
    size_t m = pattern.size();
    std::vector<size_t> pi(m, 0);
    size_t k = 0;

    for (size_t q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q]) {
            k = pi[k - 1];
        }
        if (pattern[k] == pattern[q]) ++k;
        pi[q] = k;
    }
    return pi;
}

std::vector<size_t> kmp_match(std::string_view text, std::string_view pattern) {
    std::vector<size_t> matches;
    auto pi = compute_prefix(pattern);
    size_t q = 0;  // number of matched characters

    for (size_t i = 0; i < text.size(); ++i) {
        while (q > 0 && pattern[q] != text[i]) {
            q = pi[q - 1];
        }
        if (pattern[q] == text[i]) ++q;
        if (q == pattern.size()) {
            matches.push_back(i - pattern.size() + 1);
            q = pi[q - 1];  // look for next match
        }
    }
    return matches;
}
```

**Complexity:** O(n + m) — optimal for linear-time matching.

### Rabin-Karp Algorithm

Uses rolling hash to find matches in O(n + m) average time.

```cpp
constexpr size_t base = 91138233;
constexpr size_t mod = 97266353;

std::vector<size_t> rabin_karp(std::string_view text, std::string_view pattern) {
    size_t n = text.size(), m = pattern.size();
    if (m > n) return {};

    // Precompute base^(m-1) mod mod
    size_t h = 1;
    for (size_t i = 0; i < m - 1; ++i) h = (h * base) % mod;

    // Compute hash of pattern and first window
    size_t pattern_hash = 0;
    size_t window_hash = 0;
    for (size_t i = 0; i < m; ++i) {
        pattern_hash = (pattern_hash * base + pattern[i]) % mod;
        window_hash = (window_hash * base + text[i]) % mod;
    }

    std::vector<size_t> matches;
    for (size_t i = 0; i <= n - m; ++i) {
        if (pattern_hash == window_hash) {
            // Verify (in case of hash collision)
            if (text.substr(i, m) == pattern) {
                matches.push_back(i);
            }
        }
        // Roll the hash
        if (i < n - m) {
            window_hash = (window_hash - text[i] * h % mod + mod) % mod;
            window_hash = (window_hash * base + text[i + m]) % mod;
        }
    }
    return matches;
}
```

### Boyer-Moore Algorithm

Boyer-Moore scans the pattern from right to left, skipping ahead based on two heuristics (bad character and good suffix). In practice, it is often faster than KMP, especially for large alphabets.

## Suffix Arrays and LCP Arrays

A **suffix array** is a sorted array of all suffixes of a string. It enables O(m log n) substring search and O(n) construction of many string properties.

**String:** "banana"

```
Suffixes:
0: banana
1: anana
2: nana
3: ana
4: na
5: a

Sorted suffix array SA = [5, 3, 1, 0, 4, 2]
                          a  ana anana banana na nana
```

### Suffix Array Construction

```cpp
std::vector<size_t> build_suffix_array(std::string_view s) {
    size_t n = s.size();
    std::vector<size_t> sa(n);
    std::iota(sa.begin(), sa.end(), 0);

    std::sort(sa.begin(), sa.end(), [&](size_t a, size_t b) {
        return s.substr(a) < s.substr(b);
    });
    return sa;
}
```

This O(n² log n) naive version is too slow for large strings. The O(n log n) doubling algorithm or SA-IS (linear time) are used in practice.

### LCP Array

The **LCP (Longest Common Prefix)** array stores the length of the longest common prefix between consecutive suffixes in the suffix array.

```
SA:  [5, 3, 1, 0, 4, 2]
      a  ana anana banana na nana
LCP: [0, 1, 3, 0, 0, 2]
```

```cpp
std::vector<size_t> build_lcp(std::string_view s,
                               const std::vector<size_t>& sa) {
    size_t n = s.size();
    std::vector<size_t> rank(n);
    for (size_t i = 0; i < n; ++i) rank[sa[i]] = i;

    std::vector<size_t> lcp(n, 0);
    size_t k = 0;

    for (size_t i = 0; i < n; ++i) {
        if (rank[i] == n - 1) { k = 0; continue; }
        size_t j = sa[rank[i] + 1];  // next suffix in SA
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
        lcp[rank[i]] = k;
        if (k > 0) --k;
    }
    return lcp;
}
```

**Applications of Suffix Arrays + LCP:**
- **Pattern matching:** O(m log n)
- **Longest repeated substring:** max LCP value
- **Longest common substring** of two strings
- **Number of distinct substrings:** n(n+1)/2 - sum(LCP)
- **String compression** detection

## Application: Longest Repeated Substring

```cpp
std::string longest_repeated_substring(std::string_view s) {
    auto sa = build_suffix_array(s);
    auto lcp = build_lcp(s, sa);

    size_t max_lcp = 0;
    size_t max_pos = 0;
    for (size_t i = 0; i < lcp.size(); ++i) {
        if (lcp[i] > max_lcp) {
            max_lcp = lcp[i];
            max_pos = sa[i];
        }
    }
    return std::string(s.substr(max_pos, max_lcp));
}
```

## Application: Genome Pattern Search

Searching for a DNA sequence pattern in a genome:

```cpp
class genome_index {
public:
    explicit genome_index(std::string_view genome)
        : genome_(genome), sa_(build_suffix_array(genome)) {}

    bool contains(std::string_view pattern) const {
        // Binary search on suffix array
        auto comp = [&](size_t pos, std::string_view pat) {
            return genome_.substr(pos, pat.size()) < pat;
        };

        auto it = std::lower_bound(sa_.begin(), sa_.end(), pattern, comp);
        if (it == sa_.end()) return false;
        return genome_.substr(*it, pattern.size()) == pattern;
    }

    std::vector<size_t> find_all(std::string_view pattern) const {
        // Find range of matching suffixes
        auto [lo, hi] = std::equal_range(sa_.begin(), sa_.end(), pattern,
            [&](size_t a, std::string_view pat) {
                if (a == SIZE_MAX) return true;
                return genome_.substr(a, pat.size()) < pat;
            });
        return std::vector<size_t>(lo, hi);
    }

private:
    std::string genome_;
    std::vector<size_t> sa_;
};
```

## Common Bugs and Pitfalls

| Pitfall | Consequence | Solution |
|---------|-------------|----------|
| Trie node array of size R for Unicode (R = 1,114,112) | 4+ MB per node, memory explosion | Use TST, hash map per node, or compressed trie |
| KMP failure function π[0] = 0 but π needs π[0] = -1 for shift | Infinite loop on first mismatch | Set π[0] = -1 and adjust the matching loop |
| Rabin-Karp hash collision not detected | False positive match, corrupt search result | Verify match character-by-character when hash matches |
| Suffix array built with `std::sort` on suffixes (naive) | O(n² log n), infeasible for n > 10⁴ | Use Manber-Myers doubling or SA-IS for O(n) |
| Forgetting that `std::string` can contain embedded nulls | Truncated search, missed patterns | Use `std::string_view(data, len)` with explicit length |
| LCP array off-by-one indexing | Wrong longest repeated substring | LCP[i] contains LCP of suffix at SA[i] and SA[i-1] |
| Using int for suffix array indices when n > 2³¹ | Integer overflow on large genomes | Use `size_t` or `int64_t` for n > 2³¹ |

## Summary

1. **Tries** provide O(m) lookup for string keys, independent of the number of keys.
2. **Ternary Search Tries** reduce memory for large alphabets.
3. **KMP** achieves O(n + m) string matching using the failure function.
4. **Rabin-Karp** uses rolling hash for average O(n + m) matching.
5. **Suffix arrays** with LCP enable powerful string analysis: longest repeated substring, pattern matching, distinct substrings.
6. All these techniques are essential for text processing, genomics, and search.

## Exercises

### Drill

1. **Large alphabet trie.** A trie for Unicode text could have up to 1 million children per node. That's infeasible. Name three alternatives (TST, hash map per node, compressed trie) and estimate the memory per node for each. Which would you use for an autocomplete system and why?

   *(In production, search engines use compressed trie variants to handle Unicode efficiently.)*

2. **Compressed trie.** Given the words "cat", "catastrophe", "category", "caterpillar", "catfish", draw both a standard trie and a compressed trie (radix tree). Count the nodes in each. What's the memory savings?

3. **String matching algorithms.** For pattern "aaaa" in text "aaaaaaaaaa", how many character comparisons do naive search, KMP, and Boyer-Moore each make? Show the KMP prefix function.

4. **Hash collision probability.** A rolling hash uses a 32-bit prime modulus. If a document has 10^5 hash windows, what is the approximate probability of at least one false positive? What if you use a 64-bit modulus?

5. **Memory for large strings.** A string of length 3 billion needs a suffix array. How much memory does a 32-bit index use? A 64-bit index? What technique could reduce memory for DNA sequence analysis?

   *(In production, genome aligners use the FM-index (BWT + suffix array compression) to fit human-genome-sized indices in memory.)*

### Application

6. **Autocomplete trie.** Implement a trie that supports insert(word, frequency) and top-k suggestions by prefix. Test with 10^5 words. Measure memory and query latency for prefixes of length 2, 3, 4, 5. Compare a standard trie vs a compressed trie (merge single-child chains).

7. **Pattern matching comparison.** Implement naive string search, KMP, and Boyer-Moore. Benchmark on a 10 MB text for patterns of length 10, 50, 100. Report comparisons per pattern length. Which algorithm wins and when?

8. **Suffix array.** Implement the suffix array of a string (use the simple O(n² log n) method first, then the O(n log n) doubling method). Use it to find the longest repeated substring. Benchmark both methods for n = 10^3, 10^4, 10^5.

   *(In production, plagiarism detectors like Stanford Moss use suffix arrays to find copied passages between documents.)*

9. **Multi-pattern matching (Aho-Corasick).** Implement Aho-Corasick for searching 1000 patterns in a text simultaneously. Compare with running KMP for each pattern separately. Measure build time and search speed. At what number of patterns does Aho-Corasick become faster?

   *(In production, intrusion detection systems like Snort use Aho-Corasick to match thousands of attack signatures against network traffic in real time.)*

10. **Document similarity.** Implement a program that finds the longest common substring between two documents using a suffix array. Test on two 10^5-character documents. Compare runtime with a naive O(n²) approach.

### Research

11. **Burrows-Wheeler compression.** Implement the BWT using a suffix array, then the inverse BWT. Combine with move-to-front encoding and Huffman coding to create a simple compressor. Compare compression ratio and speed with `gzip` on a 10 MB text file.

12. **Suffix automaton.** Implement a suffix automaton (SAM) — a minimal DFA accepting all substrings of a string. Use it to count occurrences of a pattern in O(m) time. Compare the number of states vs the suffix array size for n = 10^5, 10^6. Why is SAM useful for real-time genome search?

13. **String matching for version strings.** A system processes 10^6 version string lookups per second (e.g., "2.1.3" ≥ "2.1.14"). Implement a pattern matcher using the Z-algorithm. Compare throughput with KMP and `std::string::find` for exact match, prefix match, and wildcard match.

14. **Compressed string dictionary.** A serialization library stores field name → integer mappings. Build a Patricia trie (compressed radix tree) for this dictionary. Compare memory and lookup speed with `std::unordered_map<string, int>` for 10^4 field names. Which is better for read-heavy deserialization?

## References and Further Reading

- Donald E. Knuth, James H. Morris, and Vaughan R. Pratt, "Fast Pattern Matching in Strings" — SIAM Journal on Computing, 1977.
- Richard M. Karp and Michael O. Rabin, "Efficient Randomized Pattern-Matching Algorithms" — IBM Journal of R&D, 1987.
- Robert S. Boyer and J. Strother Moore, "A Fast String Searching Algorithm" — CACM, 1977.
- Udi Manber and Gene Myers, "Suffix Arrays: A New Method for On-Line String Searches" — SIAM Journal on Computing, 1993.
- Dan Gusfield, *Algorithms on Strings, Trees, and Sequences*. Cambridge University Press, 1997.
- Michael Burrows and David J. Wheeler, "A Block-Sorting Lossless Data Compression Algorithm" — DEC SRC Technical Report, 1994.
- Alfred V. Aho and Margaret J. Corasick, "Efficient String Matching: An Aid to Bibliographic Search" — CACM, 1975.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Section 5.3 (substring search).
