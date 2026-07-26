#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace dsa {

// Min-max heap: O(1) min and max access (Brass Ch5 §5.10)
// Root at min level. Levels alternate: min at even depth, max at odd depth.
template <typename T, typename Compare = std::less<T>>
class min_max_heap {
public:
    min_max_heap() = default;

    explicit min_max_heap(std::vector<T> data, Compare cmp = {})
        : data_(std::move(data)), cmp_(std::move(cmp)) {
        for (int i = static_cast<int>(data_.size()) / 2 - 1; i >= 0; --i)
            sift_down(i);
    }

    void push(const T& val) {
        data_.push_back(val);
        push_bubble_up(static_cast<int>(data_.size()) - 1);
    }

    void push(T&& val) {
        data_.push_back(std::move(val));
        push_bubble_up(static_cast<int>(data_.size()) - 1);
    }

    const T& min() const {
        if (empty()) throw std::underflow_error("heap is empty");
        return data_[0];
    }

    const T& max() const {
        if (empty()) throw std::underflow_error("heap is empty");
        if (data_.size() == 1) return data_[0];
        int m = 1;
        if (data_.size() > 2 && cmp_(data_[m], data_[2])) m = 2;
        return data_[m];
    }

    void pop_min() {
        if (empty()) throw std::underflow_error("heap is empty");
        if (data_.size() <= 2) { data_.pop_back(); return; }
        data_[0] = std::move(data_.back());
        data_.pop_back();
        if (!empty()) sift_down(0);
    }

    void pop_max() {
        if (empty()) throw std::underflow_error("heap is empty");
        if (data_.size() <= 2) { data_.pop_back(); return; }
        int mx = 1;
        if (data_.size() > 2 && cmp_(data_[mx], data_[2])) mx = 2;
        data_[mx] = std::move(data_.back());
        data_.pop_back();
        if (mx < static_cast<int>(data_.size())) sift_down(mx);
    }

    bool empty() const noexcept { return data_.empty(); }
    std::size_t size() const noexcept { return data_.size(); }

private:
    static int parent(int i) { return (i - 1) / 2; }

    // depth = floor(log2(i+1)); min at even depth, max at odd depth
    static bool is_min_level(int i) {
        int depth = 31 - __builtin_clz(i + 1);
        return depth % 2 == 0;
    }

    static bool has_grandparent(int i) { return parent(i) > 0; }
    static int grandparent(int i) { return parent(parent(i)); }

    // ---- Push ----
    void push_bubble_up(int i) {
        if (i == 0) return;
        int p = parent(i);
        if (is_min_level(i)) {
            // p is at max level. If p < i, max-level violation → swap, fix max ancestors.
            if (cmp_(data_[p], data_[i])) {
                std::swap(data_[i], data_[p]);
                bubble_up_max(p);
            } else {
                bubble_up_min(i);
            }
        } else {
            // i at max level, p at min level. If p > i, min-level violation → swap, fix min ancestors.
            if (cmp_(data_[i], data_[p])) {
                std::swap(data_[i], data_[p]);
                bubble_up_min(p);
            } else {
                bubble_up_max(i);
            }
        }
    }

    // i at max level: ensure all max-level ancestors above are ≥ data[i]
    void bubble_up_max(int i) {
        while (has_grandparent(i)) {
            int gp = grandparent(i);
            if (cmp_(data_[gp], data_[i])) {
                std::swap(data_[i], data_[gp]);
                i = gp;
            } else break;
        }
    }

    // i at min level: ensure all min-level ancestors above are ≤ data[i]
    void bubble_up_min(int i) {
        while (has_grandparent(i)) {
            int gp = grandparent(i);
            if (cmp_(data_[i], data_[gp])) {
                std::swap(data_[i], data_[gp]);
                i = gp;
            } else break;
        }
    }

    // ---- Sift down ----
    void sift_down(int i) {
        int n = static_cast<int>(data_.size());
        if (is_min_level(i)) sift_down_min(i, n);
        else sift_down_max(i, n);
    }

    void sift_down_min(int i, int n) {
        while (true) {
            int m = min_descendant(i, n);
            if (m == i) break;
            std::swap(data_[i], data_[m]);
            if (m == 2*i+1 || m == 2*i+2) break; // swapped with child, done
            // Swapped with grandchild: fix intermediate child
            int pm = (m - 1) / 2;
            // pm is at max level — ensure it's ≥ its children
            {
                int c1 = 2*pm+1, c2 = 2*pm+2;
                int worst = pm;
                if (c1 < n && cmp_(data_[worst], data_[c1])) worst = c1;
                if (c2 < n && cmp_(data_[worst], data_[c2])) worst = c2;
                if (worst != pm) std::swap(data_[pm], data_[worst]);
            }
            i = m;
        }
    }

    void sift_down_max(int i, int n) {
        while (true) {
            int m = max_descendant(i, n);
            if (m == i) break;
            std::swap(data_[i], data_[m]);
            if (m == 2*i+1 || m == 2*i+2) break;
            int pm = (m - 1) / 2;
            // pm is at min level — ensure it's ≤ its children
            {
                int c1 = 2*pm+1, c2 = 2*pm+2;
                int worst = pm;
                if (c1 < n && cmp_(data_[c1], data_[worst])) worst = c1;
                if (c2 < n && cmp_(data_[c2], data_[worst])) worst = c2;
                if (worst != pm) std::swap(data_[pm], data_[worst]);
            }
            i = m;
        }
    }

    int min_descendant(int i, int n) const {
        int m = i;
        for (int c = 2*i+1; c <= 2*i+2 && c < n; ++c)
            if (cmp_(data_[c], data_[m])) m = c;
        for (int gc = 4*i+3; gc <= 4*i+6 && gc < n; ++gc)
            if (cmp_(data_[gc], data_[m])) m = gc;
        return m;
    }

    int max_descendant(int i, int n) const {
        int m = i;
        for (int c = 2*i+1; c <= 2*i+2 && c < n; ++c)
            if (cmp_(data_[m], data_[c])) m = c;
        for (int gc = 4*i+3; gc <= 4*i+6 && gc < n; ++gc)
            if (cmp_(data_[m], data_[gc])) m = gc;
        return m;
    }

    std::vector<T> data_;
    Compare cmp_;
};

}  // namespace dsa
