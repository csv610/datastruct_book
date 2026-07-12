struct edge {
    size_t to, rev;
    int capacity;
};

class dinic {
public:
    dinic(size_t n) : graph_(n) {}

    void add_edge(size_t from, size_t to, int capacity) {
        graph_[from].push_back({to, graph_[to].size(), capacity});
        graph_[to].push_back({from, graph_[from].size() - 1, 0});  // reverse edge
    }

    int max_flow(size_t s, size_t t) {
        int flow = 0;
        std::vector<int> level;
        std::vector<size_t> it;

        while (bfs(s, t, level)) {
            it.assign(graph_.size(), 0);
            while (int pushed = dfs(s, t, INT_MAX, level, it)) {
                flow += pushed;
            }
        }
        return flow;
    }

private:
    bool bfs(size_t s, size_t t, std::vector<int>& level) {
        level.assign(graph_.size(), -1);
        std::queue<size_t> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            size_t v = q.front(); q.pop();
            for (const auto& e : graph_[v]) {
                if (e.capacity > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(size_t v, size_t t, int f, std::vector<int>& level,
            std::vector<size_t>& it) {
        if (v == t) return f;
        for (size_t& i = it[v]; i < graph_[v].size(); ++i) {
            edge& e = graph_[v][i];
            if (e.capacity > 0 && level[v] < level[e.to]) {
                int pushed = dfs(e.to, t, std::min(f, e.capacity), level, it);
                if (pushed > 0) {
                    e.capacity -= pushed;
                    graph_[e.to][e.rev].capacity += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    std::vector<std::vector<edge>> graph_;
};
