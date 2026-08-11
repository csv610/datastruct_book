#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename Flow = int, typename Cost = long long>
class min_cost_max_flow {
public:
    struct result { Flow flow{}; Cost cost{}; };

    explicit min_cost_max_flow(std::size_t n) : graph_(n) {}

    void add_edge(std::size_t from, std::size_t to, Flow capacity, Cost cost) {
        graph_[from].push_back({to, graph_[to].size(), capacity, cost});
        graph_[to].push_back({from, graph_[from].size() - 1, Flow{}, -cost});
    }

    result solve(std::size_t source, std::size_t sink,
                 Flow limit = std::numeric_limits<Flow>::max()) {
        const std::size_t n = graph_.size();
        if (source >= n || sink >= n) throw std::out_of_range("min-cost flow vertex");
        if (source == sink || limit <= 0) return {};
        const Cost inf = std::numeric_limits<Cost>::max() / 4;
        std::vector<Cost> potential = initial_potentials(source, inf), distance(n);
        std::vector<std::size_t> pv(n), pe(n);
        result answer;

        while (answer.flow < limit) {
            std::fill(distance.begin(), distance.end(), inf);
            distance[source] = 0;
            using item = std::pair<Cost, std::size_t>;
            std::priority_queue<item, std::vector<item>, std::greater<item>> pq;
            pq.push({0, source});
            while (!pq.empty()) {
                auto [d, v] = pq.top(); pq.pop();
                if (d != distance[v]) continue;
                for (std::size_t i = 0; i < graph_[v].size(); ++i) {
                    const auto& e = graph_[v][i];
                    if (e.capacity <= 0) continue;
                    const Cost reduced = e.cost + potential[v] - potential[e.to];
                    if (distance[e.to] > d + reduced) {
                        distance[e.to] = d + reduced;
                        pv[e.to] = v;
                        pe[e.to] = i;
                        pq.push({distance[e.to], e.to});
                    }
                }
            }
            if (distance[sink] == inf) break;
            for (std::size_t v = 0; v < n; ++v)
                if (distance[v] != inf) potential[v] += distance[v];

            Flow pushed = limit - answer.flow;
            for (std::size_t v = sink; v != source; v = pv[v])
                pushed = std::min(pushed, graph_[pv[v]][pe[v]].capacity);
            for (std::size_t v = sink; v != source; v = pv[v]) {
                auto& e = graph_[pv[v]][pe[v]];
                e.capacity -= pushed;
                graph_[v][e.reverse].capacity += pushed;
            }
            answer.flow += pushed;
            answer.cost += static_cast<Cost>(pushed) * potential[sink];
        }
        return answer;
    }

private:
    struct edge { std::size_t to, reverse; Flow capacity; Cost cost; };

    std::vector<Cost> initial_potentials(std::size_t source, Cost inf) const {
        std::vector<Cost> distance(graph_.size(), inf);
        distance[source] = 0;
        for (std::size_t pass = 0; pass + 1 < graph_.size(); ++pass) {
            bool changed = false;
            for (std::size_t from = 0; from < graph_.size(); ++from) {
                if (distance[from] == inf) continue;
                for (const auto& e : graph_[from]) {
                    if (e.capacity > 0 && distance[e.to] > distance[from] + e.cost) {
                        distance[e.to] = distance[from] + e.cost;
                        changed = true;
                    }
                }
            }
            if (!changed) break;
        }
        for (auto& value : distance) if (value == inf) value = 0;
        return distance;
    }

    std::vector<std::vector<edge>> graph_;
};

} // namespace dsa
