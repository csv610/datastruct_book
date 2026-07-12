#include <algorithm>
#include <vector>
#include <numeric>

std::vector<int> v = {5, 3, 1, 4, 2};

// sort with custom comparator
std::sort(v.begin(), v.end(),
    [](int a, int b) { return a > b; });
// v is now {5, 4, 3, 2, 1}

// find_if -- find first even number
auto it = std::find_if(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; });
// *it == 4

// for_each -- print each element
std::for_each(v.begin(), v.end(),
    [](int x) { std::cout << x << ' '; });

// count_if -- count positive numbers
int n = std::count_if(v.begin(), v.end(),
    [](int x) { return x > 0; });

// accumulate with lambda
int sum = std::accumulate(v.begin(), v.end(), 0,
    [](int acc, int x) { return acc + x; });
