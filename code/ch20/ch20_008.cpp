struct mincost_edge {
    size_t to, rev;
    int capacity, cost;
};

class mincost_flow {
public:
    mincost_flow(size_t n) : graph_(n), potential_(n, 0) {}

    void add_edge(size_t from, size_t to, int cap, int cost) {
        graph_[from].push_back({to, graph_[to].size(), cap, cost});
        graph_[to].push_back({from, graph_[from].size() - 1, 0, -cost});
    }

    std::pair<int, int> flow(size_t s, size_t t, int maxf) {
        int flow = 0, cost = 0;
        while (flow < maxf) {
            std::vector<int> dist(graph_.size(), INT_MAX);
            std::vector<mincost_edge*> prev(graph_.size(), nullptr);
            dist[s] = 0;
            std::priority_queue<std::pair<int, size_t>,
                std::vector<std::pair<int, size_t>>, std::greater<>> pq;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, v] = pq.top(); pq.pop();
                if (dist[v] < d) continue;
                for (auto& e : graph_[v]) {
                    if (e.capacity > 0) {
                        int nd = d + e.cost + potential_[v] - potential_[e.to];
                        if (nd < dist[e.to]) {
                            dist[e.to] = nd;
                            prev[e.to] = &e;
                            pq.push({nd, e.to});
                        }
                    }
                }
            }
            if (dist[t] == INT_MAX) break;

            for (size_t v = 0; v < graph_.size(); ++v)
                if (dist[v] < INT_MAX)
                    potential_[v] += dist[v];

            int add = maxf - flow;
            for (size_t v = t; v != s; ) {
                auto& e = *prev[v];
                add = std::min(add, e.capacity);
                v = graph_[e.to][e.rev].to;  // previous vertex
            }
            for (size_t v = t; v != s; ) {
                auto& e = *prev[v];
                e.capacity -= add;
                graph_[e.to][e.rev].capacity += add;
                cost += add * e.cost;
                v = graph_[e.to][e.rev].to;
            }
            flow += add;
        }
        return {flow, cost};
    }

private:
    std::vector<std::vector<mincost_edge>> graph_;
    std::vector<int> potential_;
};
