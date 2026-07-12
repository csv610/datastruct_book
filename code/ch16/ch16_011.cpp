// Natural merge sort: identify runs, then merge adjacent runs
void natural_merge_sort(std::span<int> arr) {
    if (arr.size() <= 1) return;

    // Phase 1: find all natural runs
    std::vector<std::pair<size_t, size_t>> runs;  // (start, length)
    size_t start = 0;
    for (size_t i = 1; i <= arr.size(); ++i) {
        if (i == arr.size() || arr[i] < arr[i - 1]) {
            runs.push_back({start, i - start});
            start = i;
        }
    }

    // Phase 2: merge adjacent runs until one remains
    while (runs.size() > 1) {
        std::vector<std::pair<size_t, size_t>> merged;
        for (size_t i = 0; i + 1 < runs.size(); i += 2) {
            // Merge runs[i] and runs[i+1]
            size_t s1 = runs[i].first, len1 = runs[i].second;
            size_t s2 = runs[i+1].first, len2 = runs[i+1].second;
            std::inplace_merge(arr.begin() + s1, arr.begin() + s1 + len1,
                               arr.begin() + s2 + len2);
            merged.push_back({s1, len1 + len2});
        }
        if (runs.size() % 2 == 1) merged.push_back(runs.back());
        runs = merged;
    }
}
