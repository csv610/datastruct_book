struct activity {
    int start, finish;
};

std::vector<activity> select_activities(std::span<const activity> acts) {
    // Sort by finish time
    std::vector<activity> sorted(acts.begin(), acts.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const activity& a, const activity& b) {
                  return a.finish < b.finish;
              });

    std::vector<activity> selected;
    int last_finish = -1;

    for (const auto& [s, f] : sorted) {
        if (s >= last_finish) {
            selected.push_back({s, f});
            last_finish = f;
        }
    }
    return selected;
}
