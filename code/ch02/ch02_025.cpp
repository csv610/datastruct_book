void selection_sort(std::vector<int>& a) {
    for (size_t i = 0; i + 1 < a.size(); ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < a.size(); ++j)
            if (a[j] < a[min_idx]) min_idx = j;
        std::swap(a[i], a[min_idx]);
    }
}
