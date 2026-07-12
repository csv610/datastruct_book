std::vector<int> reservoir_sample(std::span<const int> stream, size_t k) {
    std::vector<int> reservoir(stream.begin(), stream.begin() + k);
    std::mt19937 rng(std::random_device{}());

    for (size_t i = k; i < stream.size(); ++i) {
        size_t j = std::uniform_int_distribution<size_t>(0, i)(rng);
        if (j < k) reservoir[j] = stream[i];
    }
    return reservoir;
}
