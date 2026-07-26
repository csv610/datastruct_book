#pragma once
#include <algorithm>
#include <cassert>
#include <climits>
#include <memory>

namespace dsa {

// van Emde Boas tree: O(log log U) insert, delete, find, successor, predecessor.
// U must be a power of 2 >= 2. Keys are integers in [0, U).
// Uses recursive decomposition with bit-vector clusters.
template <int U>
class veb_tree {
    static_assert(U >= 2 && (U & (U - 1)) == 0, "U must be a power of 2");

    static constexpr int SQ = [] {
        int s = 1;
        while (s * s < U) s *= 2;
        return s;
    }();
    static constexpr int CLUSTERS = U / SQ;

public:
    veb_tree() = default;

    void insert(int x) {
        assert(x >= 0 && x < U);
        if (min_ == -1) {
            min_ = max_ = x;
            ++count_;
            return;
        }
        if (x < min_) std::swap(x, min_);
        if (x > max_) max_ = x;
        int ci = x / SQ, co = x % SQ;
        if (!clusters_[ci]) clusters_[ci] = std::make_unique<veb_tree_sub>();
        if (clusters_[ci]->empty()) summary_.insert(ci);
        clusters_[ci]->insert(co);
        ++count_;
    }

    void erase(int x) {
        assert(contains(x));
        if (min_ == max_) {
            min_ = max_ = -1;
            --count_;
            return;
        }
        if (x == min_) {
            int ci = summary_.min_val();
            min_ = ci * SQ + clusters_[ci]->min_val();
            x = min_;
        }
        int ci = x / SQ, co = x % SQ;
        clusters_[ci]->erase(co);
        if (clusters_[ci]->empty()) summary_.erase(ci);
        if (x == max_) {
            if (summary_.empty()) {
                max_ = min_;
            } else {
                int ci2 = summary_.max_val();
                max_ = ci2 * SQ + clusters_[ci2]->max_val();
            }
        }
        --count_;
    }

    bool contains(int x) const {
        if (x == min_ || x == max_) return true;
        int ci = x / SQ;
        return clusters_[ci] && clusters_[ci]->contains(x % SQ);
    }

    int successor(int x) const {
        if (min_ == -1) return -1;
        if (x < min_) return min_;
        int ci = x / SQ, co = x % SQ;
        if (clusters_[ci]) {
            int s = clusters_[ci]->successor(co);
            if (s != -1) return ci * SQ + s;
        }
        int nci = summary_.successor(ci + 1);
        if (nci == -1) return -1;
        return nci * SQ + clusters_[nci]->min_val();
    }

    int predecessor(int x) const {
        if (min_ == -1) return -1;
        if (x > max_) return max_;
        int ci = x / SQ, co = x % SQ;
        if (clusters_[ci]) {
            int p = clusters_[ci]->predecessor(co);
            if (p != -1) return ci * SQ + p;
        }
        int pci = summary_.predecessor(ci - 1);
        if (pci == -1) {
            return (x > min_) ? min_ : -1;
        }
        return pci * SQ + clusters_[pci]->max_val();
    }

    int min_val() const { return min_; }
    int max_val() const { return max_; }
    bool empty() const { return min_ == -1; }
    std::size_t size() const { return count_; }

private:
    // Sub-structure for cluster and summary: bit-vector based
    struct veb_tree_sub {
        bool bits[U] = {};
        int lo = -1, hi = -1;

        void insert(int x) {
            bits[x] = true;
            if (lo == -1 || x < lo) lo = x;
            if (hi == -1 || x > hi) hi = x;
        }
        void erase(int x) {
            bits[x] = false;
            if (x == lo) {
                lo = -1;
                for (int i = 0; i < U; ++i)
                    if (bits[i]) { lo = i; break; }
            }
            if (x == hi) {
                hi = -1;
                for (int i = U - 1; i >= 0; --i)
                    if (bits[i]) { hi = i; break; }
            }
        }
        bool empty() const { return lo == -1; }
        bool contains(int x) const { return x >= 0 && x < U && bits[x]; }
        int min_val() const { return lo; }
        int max_val() const { return hi; }
        int successor(int x) const {
            for (int i = x + 1; i < U; ++i)
                if (bits[i]) return i;
            return -1;
        }
        int predecessor(int x) const {
            for (int i = x - 1; i >= 0; --i)
                if (bits[i]) return i;
            return -1;
        }
    };

    // Summary uses same bit-vector over cluster indices
    struct summary_type {
        bool bits[CLUSTERS] = {};
        int lo = -1, hi = -1;

        void insert(int x) {
            bits[x] = true;
            if (lo == -1 || x < lo) lo = x;
            if (hi == -1 || x > hi) hi = x;
        }
        void erase(int x) {
            bits[x] = false;
            if (x == lo) {
                lo = -1;
                for (int i = 0; i < CLUSTERS; ++i)
                    if (bits[i]) { lo = i; break; }
            }
            if (x == hi) {
                hi = -1;
                for (int i = CLUSTERS - 1; i >= 0; --i)
                    if (bits[i]) { hi = i; break; }
            }
        }
        bool empty() const { return lo == -1; }
        int min_val() const { return lo; }
        int max_val() const { return hi; }
        int successor(int x) const {
            for (int i = x; i < CLUSTERS; ++i)
                if (bits[i]) return i;
            return -1;
        }
        int predecessor(int x) const {
            for (int i = x; i >= 0; --i)
                if (bits[i]) return i;
            return -1;
        }
    };

    int min_ = -1, max_ = -1;
    std::size_t count_ = 0;
    summary_type summary_;
    std::unique_ptr<veb_tree_sub> clusters_[CLUSTERS] = {};
};

}  // namespace dsa
