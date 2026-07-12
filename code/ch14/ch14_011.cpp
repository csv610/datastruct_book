size_t inversion_count(std::span<const int> arr) {
    // Coordinate compress
    auto sorted = std::vector<int>(arr.begin(), arr.end());
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    fenwick_tree<int> bit(sorted.size());
    size_t inversions = 0;

    for (int x : arr) {
        size_t pos = std::lower_bound(sorted.begin(), sorted.end(), x) - sorted.begin();
        // Count elements greater than x that we've already seen
        inversions += bit.range_sum(pos + 1, sorted.size());
        bit.add(pos, 1);
    }
    return inversions;
}
