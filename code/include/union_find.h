#pragma once
#include <numeric>
#include <vector>

namespace dsa {

// Disjoint-set (union-find) with union-by-size and path compression
// Brass Ch6 §6.1
class union_find {
public:
    explicit union_find(int n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int x) {
        int root = x;
        while (parent_[root] != root) root = parent_[root];
        while (parent_[x] != root) {
            int next = parent_[x];
            parent_[x] = root;
            x = next;
        }
        return root;
    }

    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (size_[x] < size_[y]) std::swap(x, y);
        parent_[y] = x;
        size_[x] += size_[y];
        --count_;
        return true;
    }

    bool connected(int x, int y) { return find(x) == find(y); }

    int size(int x) { return size_[find(x)]; }

    int count() const { return count_; }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
    int count_ = static_cast<int>(parent_.size());
};

}  // namespace dsa
