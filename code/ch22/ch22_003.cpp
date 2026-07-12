struct wavelet_node {
    std::vector<bool> bits;
    std::vector<uint64_t> block_rank;
    std::unique_ptr<wavelet_node> left, right;
    char lo, hi;

    uint64_t rank0(uint64_t i) const {
        uint64_t r = 0;
        for (uint64_t j = 0; j < i; ++j)
            if (!bits[j]) ++r;
        return r;
    }

    uint64_t rank1(uint64_t i) const {
        return i - rank0(i);
    }
};

class wavelet_tree {
public:
    wavelet_tree(const std::string& s) {
        std::set<char> alpha(s.begin(), s.end());
        root_ = build(s, alpha.begin(), alpha.end());
    }

    char access(uint64_t i) const {
        return access(root_.get(), i);
    }

    uint64_t rank(char c, uint64_t i) const {
        return rank(root_.get(), c, i);
    }

    uint64_t select(char c, uint64_t k) const {
        return select(root_.get(), c, k);
    }

    uint64_t range_count(uint64_t l, uint64_t r,
                         char lo, char hi) const {
        return range_count(root_.get(), l, r, lo, hi);
    }

private:
    std::unique_ptr<wavelet_node> root_;

    std::unique_ptr<wavelet_node> build(
        const std::string& s,
        std::set<char>::iterator lo,
        std::set<char>::iterator hi)
    {
        if (lo == hi) return nullptr;
        if (std::next(lo) == hi) {
            auto node = std::make_unique<wavelet_node>();
            node->lo = node->hi = *lo;
            node->bits.assign(s.size(), false);
            return node;
        }
        auto mid = lo;
        std::advance(mid, std::distance(lo, hi) / 2);
        auto node = std::make_unique<wavelet_node>();
        node->lo = *lo;
        node->hi = *std::prev(hi);

        std::string left_str, right_str;
        for (char c : s) {
            bool go_left = (c < *mid);
            node->bits.push_back(go_left);
            if (go_left) left_str += c;
            else right_str += c;
        }
        node->left = build(left_str, lo, mid);
        node->right = build(right_str, mid, hi);
        return node;
    }

    char access(const wavelet_node* node, uint64_t i) const {
        if (!node) return node->lo;
        if (node->left == nullptr && node->right == nullptr)
            return node->lo;
        if (!node->bits[i]) {
            uint64_t new_i = node->rank0(i);
            return access(node->left.get(), new_i);
        } else {
            uint64_t new_i = node->rank1(i);
            return access(node->right.get(), new_i);
        }
    }

    uint64_t rank(const wavelet_node* node, char c,
                  uint64_t i) const {
        if (!node) return 0;
        if (node->left == nullptr && node->right == nullptr)
            return i;
        if (c <= node->left->hi) {
            uint64_t new_i = node->rank0(i);
            return rank(node->left.get(), c, new_i);
        } else {
            uint64_t new_i = node->rank1(i);
            return rank(node->right.get(), c, new_i);
        }
    }

    uint64_t select(const wavelet_node* node, char c,
                    uint64_t k) const {
        if (!node) return k;
        if (node->left == nullptr && node->right == nullptr)
            return k;
        uint64_t child_sel;
        if (c <= node->left->hi)
            child_sel = select(node->left.get(), c, k);
        else
            child_sel = select(node->right.get(), c, k);
        return node->bits[child_sel] ? node->rank1(child_sel + 1)
                                     : node->rank0(child_sel + 1) - 1;
    }

    uint64_t range_count(const wavelet_node* node,
                         uint64_t l, uint64_t r,
                         char lo, char hi) const {
        if (!node || l >= r) return 0;
        if (hi < node->lo || lo > node->hi) return 0;
        if (lo <= node->lo && node->hi <= hi)
            return r - l;
        uint64_t l0 = node->rank0(l), r0 = node->rank0(r);
        uint64_t l1 = node->rank1(l), r1 = node->rank1(r);
        uint64_t total = 0;
        if (lo <= node->left->hi)
            total += range_count(node->left.get(), l0, r0, lo, hi);
        if (hi >= node->right->lo)
            total += range_count(node->right.get(), l1, r1, lo, hi);
        return total;
    }
};
