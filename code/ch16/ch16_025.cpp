int count_inversions_brute(std::span<const int> arr) {
    int count = 0;
    for (size_t i = 0; i < arr.size(); ++i)
        for (size_t j = i + 1; j < arr.size(); ++j)
            if (arr[i] > arr[j]) ++count;
    return count;
}
