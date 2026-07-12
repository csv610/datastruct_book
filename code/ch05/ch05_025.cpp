struct SparseEntry {
    int index;
    int value;
};

class SparseArray {
    std::vector<SparseEntry> entries;
    int size;
public:
    SparseArray(int n) : size(n) {}

    void set(int idx, int val) {
        for (auto& e : entries) {
            if (e.index == idx) {
                e.value = val;
                return;
            }
        }
        entries.push_back({idx, val});
    }

    int get(int idx) const {
        for (const auto& e : entries) {
            if (e.index == idx) return e.value;
        }
        return 0;
    }

    int nnz() const { return entries.size(); }
};
