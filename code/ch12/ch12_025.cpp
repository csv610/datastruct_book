struct BDDNode {
    int var;         // variable index (0-based), -1 for terminal
    int low;         // child index when var = 0
    int high;        // child index when var = 1
    bool operator==(const BDDNode& o) const {
        return var == o.var && low == o.low && high == o.high;
    }
};

class BDD {
    vector<BDDNode> nodes;
    map<tuple<int,int,int>, int> uniqueTable;  // (var, low, high) -> node index
    int terminal0, terminal1;

    int getOrMakeNode(int var, int low, int high) {
        if (low == high) return low;       // reduction rule: merge identical children
        auto key = make_tuple(var, low, high);
        auto it = uniqueTable.find(key);
        if (it != uniqueTable.end()) return it->second;
        int idx = nodes.size();
        nodes.push_back({var, low, high});
        uniqueTable[key] = idx;
        return idx;
    }

public:
    BDD() {
        terminal0 = 0;  // node 0: terminal 0
        terminal1 = 1;  // node 1: terminal 1
        nodes.push_back({-1, -1, -1});  // placeholder for terminal 0
        nodes.push_back({-1, -1, -1});  // placeholder for terminal 1
    }

    // ITE (if-then-else): builds f = ite(G, H, E)
    //   if G=1 then H else E
    int ite(int G, int H, int E) {
        if (G == terminal1) return H;
        if (G == terminal0) return E;
        if (H == terminal1 && E == terminal0) return G;  // f = G
        if (H == terminal0 && E == terminal1) {
            // f = NOT G -- create a new node or find existing
            return getOrMakeNode(nodes[G].var,
                                 ite(nodes[G].low, terminal0, terminal1),
                                 ite(nodes[G].high, terminal0, terminal1));
        }
        // General case: recursive construction
        int v = nodes[G].var;  // highest-order variable among G,H,E
        int low  = ite(nodes[G].low,  nodes[H].low,  nodes[E].low);
        int high = ite(nodes[G].high, nodes[H].high, nodes[E].high);
        return getOrMakeNode(v, low, high);
    }

    // Build AND: f = G AND H
    int AND(int G, int H) {
        return ite(G, H, terminal0);
    }

    // Build OR: f = G OR H
    int OR(int G, int H) {
        return ite(G, terminal1, H);
    }

    // Build NOT: f = NOT G
    int NOT(int G) {
        return ite(G, terminal0, terminal1);
    }

    // Evaluate BDD for a given variable assignment
    bool evaluate(int node, const vector<bool>& assignment) const {
        if (node == terminal0) return false;
        if (node == terminal1) return true;
        return assignment[nodes[node].var]
            ? evaluate(nodes[node].high, assignment)
            : evaluate(nodes[node].low, assignment);
    }

    // Check equivalence of two BDDs
    static bool equivalent(const BDD& a, const BDD& b, int na, int nb) {
        if (na == nb) return true;
        if (a.nodes[na].var != b.nodes[nb].var) return false;
        return equivalent(a, b, a.nodes[na].low,  b.nodes[nb].low)
            && equivalent(a, b, a.nodes[na].high, b.nodes[nb].high);
    }

    int nodeCount() const { return nodes.size(); }
    int getTerminal1() const { return terminal1; }
    const BDDNode& getNode(int idx) const { return nodes[idx]; }
};
