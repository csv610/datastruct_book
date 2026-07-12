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
