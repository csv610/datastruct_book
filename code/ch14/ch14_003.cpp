std::vector<int> arr = {1, 3, 5, 7, 9, 11, 2, 4};
sparse_table st(arr);

st.query_min(0, 7);  // 1
st.query_min(2, 6);  // 2
st.query_min(4, 5);  // 9
