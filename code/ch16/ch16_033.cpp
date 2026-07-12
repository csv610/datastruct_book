#include <vector>
#include <algorithm>
using namespace std;

double findMedianSortedArrays(vector<int>& A, vector<int>& B) {
    if (A.size() > B.size())
        return findMedianSortedArrays(B, A);

    int m = A.size(), n = B.size();
    int lo = 0, hi = m;
    int halfLen = (m + n + 1) / 2;

    while (lo <= hi) {
        int i = (lo + hi) / 2;
        int j = halfLen - i;

        int aLeft  = (i > 0) ? A[i - 1] : INT_MIN;
        int aRight = (i < m) ? A[i]     : INT_MAX;
        int bLeft  = (j > 0) ? B[j - 1] : INT_MIN;
        int bRight = (j < n) ? B[j]     : INT_MAX;

        if (aLeft <= bRight && bLeft <= aRight) {
            if ((m + n) % 2 == 1)
                return max(aLeft, bLeft);
            return (max(aLeft, bLeft)
                  + min(aRight, bRight)) / 2.0;
        } else if (aLeft > bRight) {
            hi = i - 1;
        } else {
            lo = i + 1;
        }
    }
    return 0.0;
}
