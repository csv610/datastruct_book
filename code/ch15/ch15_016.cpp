struct job {
    int deadline, profit;
};

// Union-Find for slot allocation
struct union_find {
    std::vector<int> parent;
    union_find(int n) : parent(n + 1) {
        for (int i = 0; i <= n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) { parent[find(x)] = find(y); }
};

int job_sequencing(std::span<const job> jobs) {
    // Sort by profit descending
    std::vector<job> sorted(jobs.begin(), jobs.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const job& a, const job& b) {
                  return a.profit > b.profit;
              });

    int max_deadline = 0;
    for (const auto& j : jobs) max_deadline = std::max(max_deadline, j.deadline);

    union_find uf(max_deadline);
    int total_profit = 0;

    for (const auto& [d, p] : sorted) {
        // Find the latest available slot before the deadline
        int slot = uf.find(d);
        if (slot > 0) {
            total_profit += p;
            // Mark this slot as used: link to slot - 1
            uf.unite(slot, slot - 1);
        }
    }
    return total_profit;
}
