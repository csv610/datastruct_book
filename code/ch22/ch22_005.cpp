class fm_index {
public:
    fm_index(const std::string& text) : n_(text.size()) {
        build_bwt(text);
    }

    int count(const std::string& pattern) const {
        int lo = 0, hi = n_;
        for (int i = (int)pattern.size() - 1; i >= 0; --i) {
            char c = pattern[i];
            lo = C_[c] + rank(c, lo);
            hi = C_[c] + rank(c, hi);
            if (lo >= hi) return 0;
        }
        return hi - lo;
    }

    std::vector<int> find(const std::string& pattern) const {
        int lo = 0, hi = n_;
        for (int i = (int)pattern.size() - 1; i >= 0; --i) {
            char c = pattern[i];
            lo = C_[c] + rank(c, lo);
            hi = C_[c] + rank(c, hi);
            if (lo >= hi) return {};
        }
        std::vector<int> occ;
        for (int i = lo; i < hi; ++i)
            occ.push_back(locate(i));
        return occ;
    }

private:
    int n_;
    std::vector<char> bwt_;
    std::vector<int> C_;
    std::vector<std::vector<int>> occ_;

    void build_bwt(const std::string& text) {
        n_ = text.size();
        std::vector<int> sa(n_);
        std::iota(sa.begin(), sa.end(), 0);
        std::sort(sa.begin(), sa.end(), [&](int a, int b) {
            return text.compare(a, n_ - a, text, b, n_ - b) < 0;
        });

        bwt_.resize(n_);
        for (int i = 0; i < n_; ++i)
            bwt_[i] = text[(sa[i] + n_ - 1) % n_];

        C_.assign(256, 0);
        for (char c : bwt_) ++C_[c];
        int sum = 0;
        for (int i = 0; i < 256; ++i) {
            int tmp = C_[i];
            C_[i] = sum;
            sum += tmp;
        }

        occ_.assign(256, std::vector<int>(n_ + 1, 0));
        for (int i = 0; i < n_; ++i) {
            for (int c = 0; c < 256; ++c)
                occ_[c][i + 1] = occ_[c][i];
            ++occ_[(int)bwt_[i]][i + 1];
        }
    }

    int rank(char c, int i) const {
        return occ_[(int)c][i];
    }

    int locate(int pos) const {
        int steps = 0;
        while (steps < n_) {
            char c = bwt_[pos];
            pos = C_[c] + occ_[(int)c][pos];
            ++steps;
        }
        return (pos + 1) % n_;
    }
};
