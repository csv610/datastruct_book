size_t random_partition(std::span<int> arr) {
    size_t r = std::rand() % arr.size();
    std::swap(arr[r], arr.back());
    return partition(arr);
}
