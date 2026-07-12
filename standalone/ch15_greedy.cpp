// ch15: Greedy Method — Fractional Knapsack, Task Scheduling, Huffman Coding
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <span>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// ---- Fractional Knapsack ----
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

// ---- Task Scheduling with Deadlines ----
struct task {
    int deadline;
    int profit;
};

int schedule_tasks(std::span<task> tasks) {
    std::vector<task> sorted(tasks.begin(), tasks.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const task& a, const task& b) { return a.profit > b.profit; });

    int max_deadline = 0;
    for (const auto& t : sorted)
        max_deadline = std::max(max_deadline, t.deadline);

    std::vector<int> slots(max_deadline + 1, -1);
    int total_profit = 0;

    for (const auto& [d, p] : sorted) {
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

// ---- Huffman Coding ----
struct huffman_node {
    char ch;
    int freq;
    huffman_node* left;
    huffman_node* right;

    ~huffman_node() { delete left; delete right; }
};

struct compare {
    bool operator()(const huffman_node* a, const huffman_node* b) const {
        return a->freq > b->freq;
    }
};

huffman_node* build_huffman_tree(const std::unordered_map<char, int>& frequencies) {
    std::priority_queue<huffman_node*, std::vector<huffman_node*>, compare> pq;

    for (const auto& [ch, freq] : frequencies)
        pq.push(new huffman_node{ch, freq, nullptr, nullptr});

    while (pq.size() > 1) {
        auto* left = pq.top(); pq.pop();
        auto* right = pq.top(); pq.pop();
        auto* parent = new huffman_node{'\0', left->freq + right->freq, left, right};
        pq.push(parent);
    }
    return pq.top();
}

void build_codes(const huffman_node* root, const std::string& prefix,
                 std::unordered_map<char, std::string>& codes) {
    if (!root->left && !root->right) {
        codes[root->ch] = prefix;
        return;
    }
    if (root->left) build_codes(root->left, prefix + "0", codes);
    if (root->right) build_codes(root->right, prefix + "1", codes);
}

int main() {
    std::cout << "=== Fractional Knapsack ===\n\n";
    std::vector<item> items = {{10, 600}, {20, 500}, {30, 360}};
    double value = fractional_knapsack(50, items);
    std::cout << "Items: (10, $600), (20, $500), (30, $360)\n";
    std::cout << "Capacity: 50\n";
    std::cout << "Maximum value: " << value << "\n\n";

    std::cout << "=== Task Scheduling ===\n\n";
    std::vector<task> tasks = {{4, 50}, {3, 40}, {2, 30}, {1, 20}, {4, 10}};
    int profit = schedule_tasks(tasks);
    std::cout << "Tasks: (d=4,p=50), (d=3,p=40), (d=2,p=30), (d=1,p=20), (d=4,p=10)\n";
    std::cout << "Maximum profit: " << profit << "\n\n";

    std::cout << "=== Huffman Coding ===\n\n";
    std::unordered_map<char, int> freqs = {
        {'A', 45}, {'B', 13}, {'C', 12}, {'D', 16}, {'E', 9}, {'F', 5}
    };

    std::cout << "Frequencies:\n";
    int total = 0;
    for (const auto& [ch, f] : freqs) {
        std::cout << "  " << ch << ": " << f << "\n";
        total += f;
    }

    auto* root = build_huffman_tree(freqs);
    std::unordered_map<char, std::string> codes;
    build_codes(root, "", codes);
    delete root;

    std::cout << "Huffman codes:\n";
    int weighted = 0;
    for (const auto& [ch, code] : codes) {
        std::cout << "  " << ch << ": " << code << "\n";
        weighted += freqs[ch] * static_cast<int>(code.size());
    }

    int fixed_bits = total * 3;
    std::cout << "Total characters: " << total << "\n";
    std::cout << "Huffman total bits: " << weighted << "\n";
    std::cout << "Fixed 3-bit total: " << fixed_bits << "\n";
    std::cout << "Savings: " << (100 * (fixed_bits - weighted) / fixed_bits) << "%\n\n";

    std::cout << "All ch15 tests passed.\n";
    return 0;
}
