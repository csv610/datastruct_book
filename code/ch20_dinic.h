#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <limits>
#include <queue>
#include <stdexcept>
#include <vector>

// ---- Dinic Max Flow ----
template <std::integral FlowUnit = int>
class dinic {
    struct edge {
        std::size_t to, rev;
        FlowUnit cap;
    };

public:
    dinic(std::size_t n) : adj_(n) {}

    void add_edge(std::size_t from, std::size_t to, FlowUnit cap) {
        adj_[from].push_back({to, adj_[to].size(), cap});
        adj_[to].push_back({from, adj_[from].size() - 1, FlowUnit{}});
    }

    FlowUnit max_flow(std::size_t source, std::size_t sink) {
        FlowUnit flow{};
        std::vector<int> level;

        while (bfs(source, sink, level)) {
            std::vector<std::size_t> ptr(adj_.size(), 0);
            while (auto pushed = dfs(source, sink, std::numeric_limits<FlowUnit>::max(), level, ptr))
                flow += pushed;
        }
        return flow;
    }

    // After max_flow, returns vertices reachable from source in residual graph
    std::vector<bool> min_cut(std::size_t source) const {
        std::vector<bool> visited(adj_.size(), false);
        std::queue<std::size_t> q;
        visited[source] = true;
        q.push(source);
        while (!q.empty()) {
            auto v = q.front(); q.pop();
            for (const auto& e : adj_[v])
                if (e.cap > 0 && !visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
        }
        return visited;
    }

private:
    bool bfs(std::size_t source, std::size_t sink,
             std::vector<int>& level) {
        level.assign(adj_.size(), -1);
        std::queue<std::size_t> q;
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            auto v = q.front(); q.pop();
            for (const auto& e : adj_[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[sink] >= 0;
    }

    FlowUnit dfs(std::size_t v, std::size_t sink, FlowUnit f,
                 std::vector<int>& level,
                 std::vector<std::size_t>& ptr) {
        if (v == sink) return f;
        for (auto& i = ptr[v]; i < adj_[v].size(); ++i) {
            auto& e = adj_[v][i];
            if (e.cap > 0 && level[v] + 1 == level[e.to]) {
                auto pushed = dfs(e.to, sink, std::min(f, e.cap),
                                  level, ptr);
                if (pushed > 0) {
                    e.cap -= pushed;
                    adj_[e.to][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return FlowUnit{};
    }

    std::vector<std::vector<edge>> adj_;
};

#endif
