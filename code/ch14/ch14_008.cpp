auto min_combine = [](int a, int b) { return std::min(a, b); };
segment_tree<int, decltype(min_combine)> st(data, min_combine);

// Query minimum in range [2, 5]
int min_val = st.query(2, 5);
