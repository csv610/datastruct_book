#include <iostream>
#include <vector>
#include <deque>
using namespace std;

// Returns the maximum of each sliding window of size k in A.
// Time: O(n), Space: O(k)
vector<int> slidingWindowMax(const vector<int>& A, int k) {
    deque<int> dq;   // stores indices, values decreasing
    vector<int> result;

    for (int i = 0; i < (int)A.size(); i++) {
        // 1. Remove indices that fell out of the window
        while (!dq.empty() && dq.front() <= i - k)
            dq.pop_front();

        // 2. Remove back indices with values <= A[i]
        while (!dq.empty() && A[dq.back()] <= A[i])
            dq.pop_back();

        // 3. Push current index
        dq.push_back(i);

        // 4. Record max once we have a full window
        if (i >= k - 1)
            result.push_back(A[dq.front()]);
    }
    return result;
}

int main() {
    vector<int> A = {2, 1, 5, 3, 6, 4, 8, 7};
    int k = 3;
    vector<int> ans = slidingWindowMax(A, k);

    cout << "Array:  ";
    for (int x : A) cout << x << " ";
    cout << "\nk = " << k << "\nMaxima: ";
    for (int x : ans) cout << x << " ";
    cout << endl;
    return 0;
}
