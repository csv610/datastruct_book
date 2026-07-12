class veb_tree {
    static constexpr int BITS = 16;
    int universe;
    int min = -1, max = -1;
    std::unique_ptr<veb_tree> summary;
    std::vector<std::unique_ptr<veb_tree>> clusters;

    int high(int x) const { return x >> (BITS / 2); }
    int low(int x) const  { return x & ((1 << (BITS / 2)) - 1); }
    int index(int h, int l) const { return (h << (BITS / 2)) | l; }

    void init_clusters() {
        int sq = 1 << (BITS / 2);
        summary = std::make_unique<veb_tree>(sq);
        clusters.resize(sq);
        for (auto& c : clusters) c = std::make_unique<veb_tree>(sq);
    }

public:
    veb_tree(int u) : universe(u) {
        if (u <= 2) return;
        init_clusters();
    }

    bool is_empty() const { return min == -1; }

    void insert(int x) {
        if (min == -1) { min = max = x; return; }
        if (x < min) std::swap(x, min);
        if (x > max) max = x;
        int h = high(x), l = low(x);
        if (clusters[h]->is_empty()) {
            summary->insert(h);
            clusters[h]->min = clusters[h]->max = l;
        } else {
            clusters[h]->insert(l);
        }
    }

    bool find(int x) const {
        if (x == min || x == max) return true;
        if (universe <= 2) return false;
        return clusters[high(x)]->find(low(x));
    }

    int successor(int x) const {
        if (universe <= 2) {
            if (x == 0 && max == 1) return 1;
            return -1;
        }
        int h = high(x), l = low(x);
        if (!clusters[h]->is_empty() && clusters[h]->max >= l) {
            int s = clusters[h]->successor(l);
            return index(h, s);
        }
        int next_cluster = summary->successor(h);
        if (next_cluster == -1) return -1;
        return index(next_cluster, clusters[next_cluster]->min);
    }

    void erase(int x) {
        if (min == max) { min = max = -1; return; }
        if (universe <= 2) {
            if (x == 0) min = 1; else min = 0;
            max = min; return;
        }
        if (x == min) {
            int first_cluster = summary->min;
            min = index(first_cluster, clusters[first_cluster]->min);
            x = min;
        }
        int h = high(x), l = low(x);
        clusters[h]->erase(l);
        if (clusters[h]->is_empty()) {
            summary->erase(h);
            if (x == max) {
                if (summary->is_empty()) max = min;
                else {
                    int last = summary->max;
                    max = index(last, clusters[last]->max);
                }
            }
        } else if (x == max) {
            max = index(h, clusters[h]->max);
        }
    }
};
