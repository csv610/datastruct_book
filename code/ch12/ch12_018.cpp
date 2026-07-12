std::vector<int> eulerian_circuit(std::vector<std::list<int>>& adj, int n) {
    std::vector<int> circuit;
    std::stack<int> stk;
    stk.push(0);

    while (!stk.empty()) {
        int u = stk.top();
        if (!adj[u].empty()) {
            int v = adj[u].front();
            adj[u].pop_front();
            // Remove reverse edge
            adj[v].remove(u);
            stk.push(v);
        } else {
            circuit.push_back(u);
            stk.pop();
        }
    }
    std::reverse(circuit.begin(), circuit.end());
    return circuit;
}
