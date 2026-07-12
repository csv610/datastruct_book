#include <vector>
#include <algorithm>

int lis_patience(std::span<const int> nums) {
    std::vector<int> tails;
    for (int x : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end())
            tails.push_back(x);       // extend longest subsequence
        else
            *it = x;                  // replace to keep smallest tail
    }
    return tails.size();
}
