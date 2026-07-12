std::vector<size_t> load_containers(double capacity, std::span<const double> weights) {
    // Pair (weight, original index)
    std::vector<std::pair<double, size_t>> items;
    for (size_t i = 0; i < weights.size(); ++i) items.push_back({weights[i], i});
    std::sort(items.begin(), items.end());

    std::vector<size_t> loaded;
    double current = 0;
    for (const auto& [w, idx] : items) {
        if (current + w <= capacity) {
            current += w;
            loaded.push_back(idx);
        }
    }
    return loaded;
}
