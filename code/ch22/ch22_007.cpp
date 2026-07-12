class succinct_tree_bp {
public:
    succinct_tree_bp(const std::vector<std::vector<int>>& adj,
                     int root) {
        n_ = 0;
        encode(adj, root);
        build_rank();
    }

    int parent(int u) const {
        int match = find_matching(u);
        return select0(rank0(match)) / 2;
    }

    int first_child(int u) const {
        if (parens_[u + 1] == 1) return u + 1;
        return -1;
    }

    int subtree_size(int u) const {
        int match = find_matching(u);
        return (match - u + 1) / 2;
    }

    int depth() const { return max_depth_; }

    size_t memory_bits() const {
        return parens_.size() + block_rank_.size() * 64;
    }

private:
    int n_;
    std::vector<bool> parens_;
    std::vector<uint64_t> block_rank_;
    int max_depth_ = 0;

    void encode(const std::vector<std::vector<int>>& adj,
                int u) {
        parens_.push_back(true);
        ++n_;
        int depth = 0;
        for (int v : adj[u]) {
            encode(adj, v);
            ++depth;
        }
        parens_.push_back(false);
        max_depth_ = std::max(max_depth_, depth);
    }

    void build_rank() {
        uint64_t blocks = (parens_.size() + 63) / 64;
        block_rank_.resize(blocks, 0);
        uint64_t running = 0;
        for (uint64_t i = 0; i < blocks; ++i) {
            block_rank_[i] = running;
            uint64_t start = i * 64;
            uint64_t end = std::min(start + 64, parens_.size());
            for (uint64_t j = start; j < end; ++j)
                if (parens_[j]) ++running;
        }
    }

    uint64_t rank0(uint64_t i) const {
        return i - rank1(i);
    }

    uint64_t rank1(uint64_t i) const {
        uint64_t block = i / 64;
        uint64_t pos = i % 64;
        uint64_t r = block > 0 ? block_rank_[block - 1] : 0;
        uint64_t start = block * 64;
        uint64_t mask = (pos == 64) ? ~0ULL : (1ULL << pos) - 1;
        uint64_t word = 0;
        for (uint64_t j = 0; j < 64 && start + j < parens_.size(); ++j)
            if (parens_[start + j]) word |= 1ULL << j;
        r += __builtin_popcountll(word & mask);
        return r;
    }

    uint64_t select0(uint64_t k) const {
        uint64_t lo = 0, hi = parens_.size();
        while (lo < hi) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (rank0(mid) <= k) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    int find_matching(int pos) const {
        int depth = 0;
        for (int i = pos; i < (int)parens_.size(); ++i) {
            if (parens_[i]) ++depth;
            else {
                --depth;
                if (depth == 0) return i;
            }
        }
        return -1;
    }
};
