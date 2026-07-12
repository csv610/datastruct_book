#include <algorithm>
#include <vector>
using namespace std;

struct Task {
    int id;       // task identifier
    int t;        // processing time
    int d;        // deadline
};

// Returns maximum lateness and fills 'order' with the
// scheduled task indices (EDF ordering).
int minMaximumLateness(vector<Task>& tasks,
                       vector<int>& order) {
    int n = tasks.size();
    order.resize(n);
    for (int i = 0; i < n; i++) order[i] = i;

    // Sort by deadline (earliest first)
    sort(order.begin(), order.end(),
         [&](int a, int b) {
             return tasks[a].d < tasks[b].d;
         });

    int currentTime = 0;
    int maxLateness = 0;
    for (int i = 0; i < n; i++) {
        int idx = order[i];
        currentTime += tasks[idx].t;
        int lateness = currentTime - tasks[idx].d;
        if (lateness > maxLateness)
            maxLateness = lateness;
    }
    return maxLateness;
}
