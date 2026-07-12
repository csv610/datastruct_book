#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

struct ek_edge {
    int to, rev;
    int cap;
};

class edmonds_karp {
public:
    edmonds_karp(int n) : graph_(n) {}

    void add_edge(int from, int to, int cap) {
        graph_[from].push_back({to, (int)graph_[to].size(), cap});
        graph_[to].push_back({from, (int)graph_[from].size() - 1, 0});
    }

    int max_flow(int s, int t) {
        int flow = 0;
        int n = graph_.size();

        while (true) {
            std::vector<int> parent(n, -1);
            std::vector<int> parent_edge(n, -1);
            std::queue<int> q;
            q.push(s);
            parent[s] = s;

            while (!q.empty() && parent[t] == -1) {
                int v = q.front(); q.pop();
                for (int i = 0; i < (int)graph_[v].size(); ++i) {
                    ek_edge& e = graph_[v][i];
                    if (e.cap > 0 && parent[e.to] == -1) {
                        parent[e.to] = v;
                        parent_edge[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if (parent[t] == -1) break;

            int aug = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                int idx = parent_edge[v];
                aug = std::min(aug, graph_[u][idx].cap);
            }

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                int idx = parent_edge[v];
                graph_[u][idx].cap -= aug;
                int rev = graph_[u][idx].rev;
                graph_[v][rev].cap += aug;
            }

            flow += aug;
        }
        return flow;
    }

private:
    std::vector<std::vector<ek_edge>> graph_;
};
