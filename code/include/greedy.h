#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace dsa {

// ---- Fractional Knapsack (Greedy) ----
struct frac_item {
    double weight;
    double value;
};

inline double fractional_knapsack(double capacity, std::vector<frac_item> items) {
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return (a.value / a.weight) > (b.value / b.weight);
    });
    double total = 0.0;
    for (const auto& item : items) {
        if (capacity <= 0) break;
        double take = std::min(item.weight, capacity);
        total += take * (item.value / item.weight);
        capacity -= take;
    }
    return total;
}

// ---- Huffman Coding Tree ----
struct huffman_node {
    char ch;
    int freq;
    huffman_node* left = nullptr;
    huffman_node* right = nullptr;

    huffman_node(char c, int f) : ch(c), freq(f) {}
    huffman_node(int f, huffman_node* l, huffman_node* r)
        : ch(0), freq(f), left(l), right(r) {}
};

struct huffman_node_cmp {
    bool operator()(huffman_node* a, huffman_node* b) const {
        return a->freq > b->freq;
    }
};

inline void huffman_free(huffman_node* n) {
    if (!n) return;
    huffman_free(n->left);
    huffman_free(n->right);
    delete n;
}

inline huffman_node* build_huffman_tree(
    const std::vector<std::pair<char, int>>& freq_table) {

    std::priority_queue<huffman_node*, std::vector<huffman_node*>,
                        huffman_node_cmp> pq;

    for (const auto& [ch, freq] : freq_table)
        pq.push(new huffman_node(ch, freq));

    while (pq.size() > 1) {
        auto* l = pq.top(); pq.pop();
        auto* r = pq.top(); pq.pop();
        pq.push(new huffman_node(l->freq + r->freq, l, r));
    }

    return pq.empty() ? nullptr : pq.top();
}

inline void collect_codes(const huffman_node* node, std::string code,
                          std::vector<std::pair<char, std::string>>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes.push_back({node->ch, code.empty() ? "0" : code});
        return;
    }
    collect_codes(node->left, code + "0", codes);
    collect_codes(node->right, code + "1", codes);
}

inline std::vector<std::pair<char, std::string>> huffman_encode(
    const std::vector<std::pair<char, int>>& freq_table) {

    auto* root = build_huffman_tree(freq_table);
    std::vector<std::pair<char, std::string>> codes;
    collect_codes(root, "", codes);
    huffman_free(root);
    return codes;
}

}  // namespace dsa
