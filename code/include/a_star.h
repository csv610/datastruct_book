#pragma once

#include "graph.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

namespace dsa {

template <typename Weight>
struct a_star_result {
    Weight distance{};
    std::vector<std::size_t> path;
};

template <typename Weight, typename Heuristic>
std::optional<a_star_result<Weight>> a_star(
    const graph<Weight>& g, std::size_t start, std::size_t goal,
    Heuristic heuristic) {
    if (start >= g.vertex_count() || goal >= g.vertex_count()) return std::nullopt;
    using entry = std::pair<Weight, std::size_t>;
    std::priority_queue<entry, std::vector<entry>, std::greater<entry>> open;
    const Weight inf = std::numeric_limits<Weight>::max();
    std::vector<Weight> distance(g.vertex_count(), inf);
    std::vector<std::size_t> parent(g.vertex_count(), g.vertex_count());
    distance[start] = Weight{};
    open.push({heuristic(start, goal), start});

    while (!open.empty()) {
        auto [estimated, v] = open.top();
        open.pop();
        const Weight expected = distance[v] + heuristic(v, goal);
        if (estimated != expected) continue;
        if (v == goal) {
            std::vector<std::size_t> path;
            for (std::size_t cur = goal;; cur = parent[cur]) {
                path.push_back(cur);
                if (cur == start) break;
            }
            std::reverse(path.begin(), path.end());
            return a_star_result<Weight>{distance[goal], std::move(path)};
        }
        for (const auto& edge : g.neighbors(v)) {
            if (edge.weight < Weight{}) continue;
            if (distance[v] > inf - edge.weight) continue;
            const Weight candidate = distance[v] + edge.weight;
            if (candidate < distance[edge.to]) {
                distance[edge.to] = candidate;
                parent[edge.to] = v;
                open.push({candidate + heuristic(edge.to, goal), edge.to});
            }
        }
    }
    return std::nullopt;
}

template <typename Weight>
std::optional<a_star_result<Weight>> a_star(
    const graph<Weight>& g, std::size_t start, std::size_t goal) {
    return a_star(g, start, goal,
                  [](std::size_t, std::size_t) { return Weight{}; });
}

} // namespace dsa
