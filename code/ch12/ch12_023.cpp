struct Hypergraph {
    int n;  // number of vertices
    vector<int> vertices;          // vertex IDs: 0..n-1
    vector<vector<int>> edges;     // each hyperedge is a set of vertex IDs

    Hypergraph(int n) : n(n) {
        for (int i = 0; i < n; i++) vertices.push_back(i);
    }

    void addEdge(const vector<int>& hyperedge) {
        edges.push_back(hyperedge);
    }

    // Degree of a vertex: number of hyperedges containing it
    int degree(int v) const {
        int deg = 0;
        for (const auto& e : edges)
            if (find(e.begin(), e.end(), v) != e.end())
                deg++;
        return deg;
    }

    // Size of a hyperedge: number of vertices it connects
    int edgeSize(int eIdx) const {
        return edges[eIdx].size();
    }

    // Incidence matrix as 2D vector
    vector<vector<int>> incidenceMatrix() const {
        vector<vector<int>> M(n, vector<int>(edges.size(), 0));
        for (int e = 0; e < (int)edges.size(); e++)
            for (int v : edges[e])
                M[v][e] = 1;
        return M;
    }

    // Two vertices are adjacent if they share at least one hyperedge
    bool adjacent(int u, int v) const {
        for (const auto& e : edges) {
            bool hasU = find(e.begin(), e.end(), u) != e.end();
            bool hasV = find(e.begin(), e.end(), v) != e.end();
            if (hasU && hasV) return true;
        }
        return false;
    }

    // k-uniform: all hyperedges have exactly k vertices
    bool isKUniform() const {
        if (edges.empty()) return true;
        int k = edges[0].size();
        for (const auto& e : edges)
            if ((int)e.size() != k) return false;
        return true;
    }
};
