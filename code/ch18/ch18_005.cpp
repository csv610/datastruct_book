void generate_subsets(int n) {
    std::vector<int> subset;
    std::function<void(int)> backtrack = [&](int i) {
        if (i > n) {
            // Process subset
            for (int x : subset) std::cout << x << " ";
            std::cout << "\n";
            return;
        }
        subset.push_back(i);     // include i
        backtrack(i + 1);
        subset.pop_back();        // exclude i
        backtrack(i + 1);
    };
    backtrack(1);
}
