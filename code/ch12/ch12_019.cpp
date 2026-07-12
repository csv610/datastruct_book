struct astar_result {
    std::vector<int> path;
    int cost;
};

astar_result astar(const graph_list<>& g, int start, int goal,
        std::function<int(int, int)> heuristic, int n) {
    std::vector<int> dist(n, INT_MAX), parent(n, -1);
    std::vector<bool> closed(n, false);
    // Min-heap: (f_cost, vertex)
    std::priority_queue<std::pair<int,int>,
        std::vector<std::pair<int,int>>, std::greater<>> open;

    dist[start] = 0;
    open.push({heuristic(start, goal), start});

    while (!open.empty()) {
        auto [f, u] = open.top();
        open.pop();

        if (u == goal) break;
        if (closed[u]) continue;
        closed[u] = true;

        for (auto [v, w] : g.neighbors(u)) {
            int new_g = dist[u] + w;
            if (new_g < dist[v]) {
                dist[v] = new_g;
                parent[v] = u;
                open.push({new_g + heuristic(v, goal), v});
            }
        }
    }

    // Reconstruct path
    astar_result result;
    result.cost = dist[goal];
    for (int v = goal; v != -1; v = parent[v])
        result.path.push_back(v);
    std::reverse(result.path.begin(), result.path.end());
    return result;
}
