class percolation {
public:
    percolation(size_t n) : n_(n), uf_(n * n + 2), open_(n * n, false) {
        top_ = n * n;       // virtual top node
        bottom_ = n * n + 1; // virtual bottom node
    }

    void open(size_t row, size_t col) {
        size_t idx = row * n_ + col;
        if (open_[idx]) return;
        open_[idx] = true;

        if (row == 0) uf_.union_sets(idx, top_);
        if (row == n_ - 1) uf_.union_sets(idx, bottom_);

        // Connect to open neighbors
        if (row > 0 && open_[idx - n_]) uf_.union_sets(idx, idx - n_);
        if (row < n_ - 1 && open_[idx + n_]) uf_.union_sets(idx, idx + n_);
        if (col > 0 && open_[idx - 1]) uf_.union_sets(idx, idx - 1);
        if (col < n_ - 1 && open_[idx + 1]) uf_.union_sets(idx, idx + 1);
    }

    bool is_open(size_t row, size_t col) const {
        return open_[row * n_ + col];
    }

    bool percolates() const {
        return uf_.same_set(top_, bottom_);
    }

private:
    size_t n_;
    union_find uf_;
    std::vector<bool> open_;
    size_t top_, bottom_;
};
