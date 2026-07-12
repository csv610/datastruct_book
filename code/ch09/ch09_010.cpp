std::vector<std::vector<int>> connected_components(
        const std::vector<std::vector<bool>>& image) {
    int rows = image.size();
    int cols = image[0].size();
    union_find uf(rows * cols);

    // First pass: union adjacent foreground pixels
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!image[r][c]) continue;
            int idx = r * cols + c;
            if (r > 0 && image[r-1][c])
                uf.union_sets(idx, (r-1) * cols + c);
            if (c > 0 && image[r][c-1])
                uf.union_sets(idx, r * cols + (c-1));
        }
    }

    // Second pass: assign labels
    std::unordered_map<size_t, int> label_map;
    int next_label = 0;
    std::vector<std::vector<int>> labels(rows, std::vector<int>(cols, 0));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!image[r][c]) continue;
            size_t root = uf.find(r * cols + c);
            auto it = label_map.find(root);
            if (it == label_map.end()) {
                label_map[root] = ++next_label;
                it = label_map.find(root);
            }
            labels[r][c] = it->second;
        }
    }
    return labels;
}
