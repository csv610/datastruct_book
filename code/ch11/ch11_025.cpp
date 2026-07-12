struct rect {
    double x1, y1, x2, y2;

    double area() const {
        return std::max(0.0, x2 - x1) * std::max(0.0, y2 - y1);
    }

    rect combine(const rect& other) const {
        return {std::min(x1, other.x1), std::min(y1, other.y1),
                std::max(x2, other.x2), std::max(y2, other.y2)};
    }

    bool intersects(const rect& other) const {
        return !(other.x1 > x2 || other.x2 < x1
              || other.y1 > y2 || other.y2 < y1);
    }
};

class rtree {
    static constexpr int MAX_ENTRIES = 4;
    static constexpr int MIN_ENTRIES = 2;

    struct entry {
        rect r;
        int child_id = -1;
        int record_id = -1;
    };

    struct node {
        std::vector<entry> entries;
        bool is_leaf;
        rect mbr;

        node(bool leaf) : is_leaf(leaf) {}

        void update_mbr() {
            if (entries.empty()) return;
            mbr = entries[0].r;
            for (size_t i = 1; i < entries.size(); ++i)
                mbr = mbr.combine(entries[i].r);
        }
    };

    std::vector<node> nodes_;

    int choose_subtree(int node_id, const rect& r) {
        auto& n = nodes_[node_id];
        if (n.is_leaf) return node_id;
        int best = 0;
        double best_growth = 1e18;
        for (size_t i = 0; i < n.entries.size(); ++i) {
            double before = n.entries[i].r.area();
            double after = n.entries[i].r.combine(r).area();
            double growth = after - before;
            if (growth < best_growth) {
                best_growth = growth;
                best = i;
            }
        }
        return choose_subtree(n.entries[best].child_id, r);
    }

    void search(const node* n, const rect& query,
                std::vector<int>& result) const {
        if (!n) return;
        for (const auto& e : n->entries) {
            if (!e.r.intersects(query)) continue;
            if (n->is_leaf) {
                result.push_back(e.record_id);
            } else {
                search(&nodes_[e.child_id], query, result);
            }
        }
    }

public:
    rtree() { nodes_.emplace_back(true); }

    void insert(const rect& r, int record_id) {
        int leaf = choose_subtree(0, r);
        entry new_entry{r, -1, record_id};

        if (nodes_[leaf].entries.size() < MAX_ENTRIES) {
            nodes_[leaf].entries.push_back(new_entry);
            nodes_[leaf].update_mbr();
            return;
        }
        // Node overflow: split (simplified)
        auto& n = nodes_[leaf];
        int new_node_id = nodes_.size();
        nodes_.emplace_back(n.is_leaf);
        entry seed2 = n.entries.back();
        n.entries.pop_back();
        nodes_[new_node_id].entries = {seed2};
        nodes_[leaf].update_mbr();
        nodes_[new_node_id].update_mbr();
        nodes_[leaf].entries.push_back(new_entry);
        nodes_[leaf].update_mbr();
    }

    std::vector<int> point_query(double x, double y) const {
        rect query{x, y, x, y};
        std::vector<int> result;
        search(&nodes_[0], query, result);
        return result;
    }

    std::vector<int> range_query(const rect& query) const {
        std::vector<int> result;
        search(&nodes_[0], query, result);
        return result;
    }
};
