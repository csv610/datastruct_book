std::vector<int> next_greater(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    std::stack<int> st;
    for (int i = nums.size() - 1; i >= 0; --i) {
        while (!st.empty() && st.top() <= nums[i]) st.pop();
        if (!st.empty()) result[i] = st.top();
        st.push(nums[i]);
    }
    return result;
}
