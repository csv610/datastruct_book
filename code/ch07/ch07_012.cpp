// Returns the maximum in every window of size k.
std::vector<int> sliding_window_max(
    const std::vector<int>& nums, int k)
{
    std::deque<int> dq;  // stores indices
    std::vector<int> result;

    for (int i = 0; i < (int)nums.size(); ++i) {
        // Remove indices outside the window.
        while (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // Remove smaller elements from the back.
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}
