void generate_permutations(std::vector<int>& arr, size_t start) {
    if (start == arr.size()) {
        for (int x : arr) std::cout << x << " ";
        std::cout << "\n";
        return;
    }
    for (size_t i = start; i < arr.size(); ++i) {
        std::swap(arr[start], arr[i]);
        generate_permutations(arr, start + 1);
        std::swap(arr[start], arr[i]);  // backtrack
    }
}
